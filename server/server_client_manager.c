#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server_client_manager.h"
#include "util/libbcrypt/bcrypt.h"

// Ajoute un client à la liste des clients actifs
int add_active_client(ActiveClients *clients, Client *client) {
  if (clients->nb == MAX_CLIENTS) {
    return 0;
  }
  if (!clients->first) {
    clients->first = client;
    clients->last = client;
    ++clients->nb;
    return 1;
  } else {
    clients->last->next = client;
    client->previous = clients->last;
    clients->last = client;
    ++clients->nb;
    return 1;
  }
}

// Supprime un client de la liste des clients actifs
void remove_active_client(ActiveClients *clients, Client *client) {
  if (clients->first == client) {
    clients->first = clients->first->next;
    --clients->nb;
  } else if (clients->last == client) {
    clients->last = clients->last->previous;
    clients->last->next = NULL;
    --clients->nb;
  } else {
    client->previous->next = client->next;
    client->next->previous = client->previous;
    --clients->nb;
  }
  clear_client_invites(client);
  if (client->opponent) {
    assert(client->game &&
           "If a client has an opponent, he should have a game");
    client->opponent->opponent = NULL;
    client->opponent->game = NULL;
    send_message_to_client(client->opponent->socket,
                 "\nYour opponent left, you won the game.");
    char message_to_observers[200];
    strcpy(message_to_observers, "\n");
    strcat(message_to_observers, client->opponent->username);
    strcat(message_to_observers, " won the game, his opponent left");
    broadcast_to_observers(client->opponent->observers,
                                  message_to_observers);
    // free(client->game);
    delete_game(client->game);
  }
  if (client->watching) {
    remove_observer_from_list(client->watching->observers, client);
  }
  Observer *observer = client->observers->first;
  while (observer) {
    observer->watcher->watching = NULL;
    send_message_to_client(observer->watcher->socket,
                 "The client you were watching went offline, you are no longer "
                 "watching anyone.");
    Observer *temp = observer;
    observer = observer->next;
    free(temp);
  }
  Friend *friend_it = client->friends->first;
  while (friend_it) {
    remove_friend_from_list(friend_it->friend_of_client->friends, client);
    Friend *temp = friend_it;
    friend_it = friend_it->next;
    free(temp);
  }

  free(client->friends);
  free(client->observers);
  free(client);
}

// Ajoute un observateur à la liste des observateurs
int add_observer_to_list(Observers *observers, Observer *observer) {
  if (!observers->first) {
    observers->first = observer;
    observers->last = observer;
    return 1;
  } else {
    observers->last->next = observer;
    observer->previous = observers->last;
    observers->last = observer;
    return 1;
  }
}

// Supprime un observateur de la liste des observateurs
void remove_observer_from_list(Observers *observers, Client *client) {
  Observer *observer = observers->first;
  while (observer) {
    if (!strcmp(observer->watcher->username, client->username)) {
      break;
    }
    observer = observer->next;
  }
  if (!observer) {
    printf("Client not found in observers\n");
    return;
  }
  if (observers->first == observer) {
    observers->first = observers->first->next;
  } else if (observers->last == observer) {
    observers->last = observers->last->previous;
    observers->last->next = NULL;
  } else {
    observer->previous->next = observer->next;
    observer->next->previous = observer->previous;
  }
  free(observer);
}

// Ajoute un ami à la liste des amis
int add_friend_to_list(FriendList *friends, Friend *new_friend) {
  if (!friends->first) {
    friends->first = new_friend;
    friends->last = new_friend;
    return 1;
  } else {
    friends->last->next = new_friend;
    new_friend->previous = friends->last;
    friends->last = new_friend;
    return 1;
  }
}


// Supprime un ami de la liste des amis
void remove_friend_from_list(FriendList *friends, Client *client) {
  Friend *friend_it = friends->first;
  while (friend_it) {
    if (!strcmp(friend_it->friend_of_client->username, client->username)) {
      break;
    }
    friend_it = friend_it->next;
  }
  if (!friend_it) {
    printf("Client not found in friendlist\n");
    return;
  }
  if (friends->first == friend_it) {
    friends->first = friends->first->next;
  } else if (friends->last == friend_it) {
    friends->last = friends->last->previous;
    friends->last->next = NULL;
  } else {
    friend_it->previous->next = friend_it->next;
    friend_it->next->previous = friend_it->previous;
  }
  free(friend_it);
}

// Ajoute une invitation à la liste des invitations
int add_invite_to_list(Invites *invites, Client *recipient) {
  if (invites->first == NULL) {
    Invite *invite = malloc(sizeof(Invite));
    invites->first = invite;
    invite->recipient = recipient;
    invite->next = NULL;
    return 1;
  } else {
    Invite *it_invite = invites->first;
    while (it_invite->next != NULL) {
      if (!strcmp(it_invite->recipient->username, recipient->username)) {
        return 0;
      }
      it_invite = it_invite->next;
    }
    if (!strcmp(it_invite->recipient->username, recipient->username)) {
      return 0;
    }
    Invite *invite = malloc(sizeof(Invite));
    it_invite->next = invite;
    invite->recipient = recipient;
    invite->next = NULL;
    return 1;
  }
}

// Supprime une invitation de la liste des invitations
void remove_invite_from_list(Invites *invites, Invite *invite) {
  Invite *it_invite = invites->first;
  if (it_invite == invite) {
    invites->first = it_invite->next;
    free(it_invite);
    return;
  }
  while (it_invite->next != invite) {
    it_invite = it_invite->next;
  }
  Invite *invite_to_remove = it_invite->next;
  it_invite->next = it_invite->next->next;
  free(invite_to_remove);
}

// Supprime toutes les invitations d'un client
void clear_client_invites(Client *client) {
  Invite *invite = client->invites->first;
  while (invite) {
    Invite *previous_invite = invite;
    invite = invite->next;
    free(previous_invite);
  }
  client->invites->first = NULL;
  invite = client->friend_requests_sent->first;
  while (invite) {
    Invite *previous_invite = invite;
    invite = invite->next;
    free(previous_invite);
  }
}

// Vérifie si un client est dans la liste des invitations
int is_client_in_invite_list(const Invites *invites, const Client *recipient) {
  Invite *it_invite = invites->first;
  while (it_invite) {
    if (!strcmp(it_invite->recipient->username, recipient->username)) {
      return 1;
    }
    it_invite = it_invite->next;
  }
  return 0;
}

// Trouve un client par son nom d'utilisateur
Client *find_active_client_by_username(const ActiveClients clients,
                                const char *username) {
  Client *client_iterator = clients.first;
  while (client_iterator != NULL) {
    if (!strcmp(client_iterator->username, username)) {
      break;
    }
    client_iterator = client_iterator->next;
  }
  return client_iterator;
}

// Envoie un message à tous les clients
void broadcast_to_all_clients(ActiveClients clients, Client sender,
                                 const char *buffer, char from_server) {
  char message[BUF_SIZE];
  Client *client_iterator = clients.first;
  while (client_iterator) {
    message[0] = 0;
    {
      /* we don't send message to the sender */
      if (sender.socket != client_iterator->socket) {
        if (from_server == 0) {
          strncpy(message, sender.username, BUF_SIZE - 1);
          strncat(message, " : ", sizeof message - strlen(message) - 1);
        }
        strncat(message, buffer, sizeof message - strlen(message) - 1);
        send_message_to_client(client_iterator->socket, message);
      }
    }
    client_iterator = client_iterator->next;
  }
}

// Envoie un message à tous les observateurs
void broadcast_to_observers(Observers *observers, const char *message) {
  Observer *observer_iterator = observers->first;
  while (observer_iterator) {
    send_message_to_client(observer_iterator->watcher->socket, message);
    observer_iterator = observer_iterator->next;
  }
}

// Écrit un message à un client
void send_message_to_client(SOCKET sock, const char *buffer) {
  if (send(sock, buffer, strlen(buffer), 0) < 0) {
    perror("send()");
    exit(errno);
  }
}

// Vérifie si un client est dans la liste des clients actifs
int is_client_in_active_list(Client *client, ActiveClients list_of_clients) {
  Client *client_iterator = list_of_clients.first;
  while (client_iterator) {
    if (!strcmp(client_iterator->username, client->username)) {
      return 1;
    }
    client_iterator = client_iterator->next;
  }
  return 0;
}

// Supprime une invitation d'un nouvel ami
void remove_invite_after_friendship(Client *client, Client *new_friend_client) {
  Invite *invite_it = client->friend_requests_sent->first;
  while (invite_it) {
    if (!strcmp(invite_it->recipient->username, new_friend_client->username)) {
      break;
    }
    invite_it = invite_it->next;
  }
  if (!invite_it) {
    printf("Invite not found in friendlist, should not be happening\n");
    return;
  }
  remove_invite_from_list(client->friend_requests_sent, invite_it);
}

// Vérifie si deux clients sont amis
int are_clients_friends(Client *client1, Client *client2) {
  Friend *friend = client1->friends->first;
  while (friend) {
    if (!strcmp(friend->friend_of_client->username, client2->username)) {
      return 1;
    }
    friend = friend->next;
  }
  return 0;
}

int is_already_connected(ActiveClients clients, char *username) {
  Client *client_iterator = clients.first;
  while (client_iterator) {
    if (!strcmp(client_iterator->username, username)) {
      return 1;
    }
    client_iterator = client_iterator->next;
  }
  return 0;
}

// Vérifie si un nom d'utilisateur est déjà utilisé
int is_username_already_used(ActiveClients clients, char *username) {
  FILE *file = fopen("users.csv", "r");
  if (!file) {
    perror("fopen()");
    return 0;
  }
  char line[100];
  while (fgets(line, 100, file)) {
    char *token = strtok(line, ",");
    if (!strcmp(token, username)) {
      fclose(file);
      return 1;    }
  }
  fclose(file);
  return 0;
}

int register_client(char *username, char *password) {
  FILE *file = fopen("users.csv", "a");
  if (!file) {
    perror("fopen()");
    return 0;
  }

  char salt[BCRYPT_HASHSIZE];
  char hash[BCRYPT_HASHSIZE];

  int ret = bcrypt_gensalt(12, salt);

  ret = bcrypt_gensalt(12, salt);
  assert(ret == 0);
  ret = bcrypt_hashpw(password, salt, hash);
  assert(ret == 0);

  printf("%s, hashed: %s", password, hash);
  fflush(stdout);
  fprintf(file, "%s,%s\n", username, hash);
  fclose(file);
  return 1;
}

int login_client(char *username, char *password) {
  FILE *file = fopen("users.csv", "r");
  if (!file) {
    perror("fopen()");
    return 0;
  }
  char line[100];
  while (fgets(line, 100, file)) {
    char *token = strtok(line, ",");
    if (!strcmp(token, username)) {
      token = strtok(NULL, ",");

      // Remove newline character
      char *pos;
      if ((pos = strchr(token, '\n')) != NULL) {
        *pos = '\0';
      }

      int ret = bcrypt_checkpw(password, token);
      assert(ret != -1);

      if (!ret) { // Passwords match
        fclose(file);
        return 1;
      }

    }
  }
  fclose(file);
  return 0;
}