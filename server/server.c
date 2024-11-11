#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#include "server.h"
#include "server_client.h"
#include "server_package.h"

// Fonction principale de l'application
static void app(void) {
  SOCKET sock = init_connection(); // On récupère le FD de la socket du serveur.

  char buffer[BUF_SIZE]; // Buffer pour les messages.

  int max = sock; // Le descripteur de fichier avec le plus grand numéro, nécessaire pour l'appel à select()

  ActiveClients clients; // Structure pour gérer les clients actifs

  clients.first = NULL;
  clients.last = NULL;
  clients.nb = 0;

  Games games; // Structure pour gérer les jeux
  games.first = NULL;
  games.last = NULL;
  load_games(&games); // Charger les jeux existants
  int current_gm_id = get_last_id(&games); // Obtenir le dernier ID de jeu

  fd_set rdfs; // Ensemble de descripteurs de fichiers. Avant select : tous les descripteurs de fichiers. Après select : seulement les descripteurs de fichiers prêts à être lus.

  while (1) {
    Client *client_iterator = clients.first;
    FD_ZERO(&rdfs); // Effacer l'ensemble

    FD_SET(STDIN_FILENO, &rdfs); // Ajouter STDIN à l'ensemble

    FD_SET(sock, &rdfs); // Ajouter la socket du serveur à l'ensemble

    while (client_iterator) { // Ajouter la socket de chaque client
      FD_SET(client_iterator->socket, &rdfs);
      client_iterator = client_iterator->next;
    }

    if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1) { // Bloquer jusqu'à ce qu'il y ait une socket lisible, effacer toutes les sockets de l'ensemble sauf celles lisibles
      perror("select()");
      exit(errno);
    }

    if (FD_ISSET(STDIN_FILENO, &rdfs)) { // Si stdin est lisible, on arrête l'application
      break;
    } else if (FD_ISSET(sock, &rdfs)) { // Si la socket du serveur est lisible, il y a un nouveau client
      SOCKADDR_IN csin = {0};
      size_t sinsize = sizeof csin;
      int csock = accept(sock, (SOCKADDR *)&csin, (socklen_t *)&sinsize); // Accepter la connexion du nouveau client
      if (csock == SOCKET_ERROR) {
        perror("accept()");
        continue;
      }

      if (read_client(csock, buffer) == 0) {
        continue; // Le client s'est déconnecté
      }

      char username[USERNAME_SIZE];
      strncpy(username, buffer, USERNAME_SIZE);
      if (is_already_used(clients, username)) {
        write_client(csock, "Ce nom d'utilisateur est déjà utilisé, veuillez revenir avec un autre");
        closesocket(csock);
        continue;
      }

      Client *c = malloc(sizeof(Client));
      Invites *invites = malloc(sizeof(Invites));
      Invites *friend_requests_sent = malloc(sizeof(Invites));
      Observers *observers = malloc(sizeof(Observers));
      FriendList *friendList = malloc(sizeof(FriendList));
      c->socket = csock;
      c->game = NULL;
      c->opponent = NULL;
      c->connected = 0;
      c->next = NULL;
      c->previous = NULL;
      c->watching = NULL;
      c->invites = invites;
      c->invites->first = NULL;
      c->friend_requests_sent = friend_requests_sent;
      c->friend_requests_sent->first = NULL;
      c->observers = observers;
      c->observers->first = NULL;
      c->observers->last = NULL;
      c->friends = friendList;
      c->friends->first = NULL;
      c->friends->last = NULL;
      c->priv = 0;

      strcpy(c->username, username);
      strcpy(c->bio, "Cet utilisateur n'a pas encore écrit sa bio.");

      if (!add_client(&clients, c)) {
        continue;
      }

      max = csock > max ? csock : max; // Mettre à jour la valeur max si nécessaire

      FD_SET(csock, &rdfs); // Ajouter la socket du client à l'ensemble

    } else { // Dans ce cas, au moins une socket client est lisible.
      client_iterator = clients.first;
      while (client_iterator) {
        if (FD_ISSET(client_iterator->socket, &rdfs)) {
          int c = read_client(client_iterator->socket, buffer);
          if (c == 0) { // Le client s'est déconnecté
            closesocket(client_iterator->socket);
            strncpy(buffer, client_iterator->username, BUF_SIZE - 1);
            strncat(buffer, " déconnecté !", BUF_SIZE - strlen(buffer) - 1);
            send_message_to_all_clients(clients, *client_iterator, buffer, 1);

            if (client_iterator->game) {
              strcpy(client_iterator->game->winner, client_iterator->opponent->username);
              end_game(client_iterator, &games);
              client_iterator->opponent->opponent = NULL;
              client_iterator->opponent = NULL;
            }

            remove_client(&clients, client_iterator);
          } else { // INFO: C'est ici que nous allons dans handle_incomming_package();
            handle_incomming_package(clients, client_iterator, buffer, &games, &current_gm_id);
          }
          break; // ISSUE: Cela peut causer une famine si le premier processus continue de parler ?
        }
        client_iterator = client_iterator->next;
      }
    }
  }

  clear_clients(&clients); // Nettoyer les clients
  end_connection(sock); // Terminer la connexion
}

// Fonction pour nettoyer les clients
static void clear_clients(ActiveClients *clients) {
  clients->nb = 0;
  Client *client_iterator = clients->first;
  while (client_iterator) {
    Invite *invite_it = client_iterator->invites->first;
    while (invite_it) {
      Invite *previous = invite_it;
      invite_it = invite_it->next;
      free(previous);
    }
    closesocket(client_iterator->socket);
    Client *previous = client_iterator;
    client_iterator = client_iterator->next;
    free(previous);
  }
}

// Fonction pour initialiser la connexion
static int init_connection(void) {
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // Socket IPV4 TCP
  SOCKADDR_IN sin = {0};

  if (sock == INVALID_SOCKET) {
    perror("socket()");
    exit(errno);
  }

  sin.sin_addr.s_addr = htonl(INADDR_ANY); // Toutes les interfaces du PC
  sin.sin_port = htons(PORT); // PORT défini dans server.h
  sin.sin_family = AF_INET; // IPV4

  if (bind(sock, (SOCKADDR *)&sin, sizeof sin) == SOCKET_ERROR) { // Lier la socket aux paramètres définis.
    perror("bind()");
    exit(errno);
  }

  if (listen(sock, MAX_CLIENTS) == SOCKET_ERROR) { // Marquer la socket comme socket passive. Cet appel est non bloquant
    perror("listen()");
    exit(errno);
  }

  return sock;
}

// Fonction pour terminer la connexion
static void end_connection(int sock) {
  closesocket(sock);
}

// Fonction pour lire les données du client
static int read_client(SOCKET sock, char *buffer) {
  int n = 0;

  if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0) {
    perror("recv()");
    n = 0;
  }

  buffer[n] = '\0';

  return n;
}

// Fonction pour charger les jeux
void load_games(Games *games) {
  FILE *file = fopen("../game_data.csv", "r");

  fseek(file, 0, SEEK_END);
  // Vérifier si le fichier est vide
  long taille = ftell(file);
  if (taille == 0) {
    return;
  }
  fseek(file, 0, SEEK_SET);
  while (!feof(file)) {
    Game *game = parseCSVToGame(file);
    if (game == NULL) {
      break;
    }
    if (!games->first) {
      games->first = game;
      games->last = game;
    } else {
      games->last->next = game;
      game->previous = games->last;
      games->last = game;
    }
    game->next = NULL;
  }
}

// Fonction pour obtenir le dernier ID de jeu
int get_last_id(Games *games) {
  if (games->first == NULL) {
    return 0;
  }
  int id = 0;
  Game *current = games->first;
  while (current != NULL) {
    if (current->game_id > id) {
      id = current->game_id;
    }
    current = current->next;
  }
  id++;
  return id;
}

// Fonction principale
int main(int argc, char **argv) {
  app(); // Appeler la fonction principale de l'application
  return EXIT_SUCCESS; // Retourner le succès
}
