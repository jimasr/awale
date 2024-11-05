#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "client2.h"
#include "../common/game_logic.h"

void init(void) {
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (err < 0) {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

void end(void) {
#ifdef WIN32
    WSACleanup();
#endif
}

int init_connection(const char *address) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("socket()");
        exit(errno);
    }

    SOCKADDR_IN sin = {0};
    struct hostent *hostinfo = gethostbyname(address);
    if (hostinfo == NULL) {
        fprintf(stderr, "Hôte inconnu %s.\n", address);
        exit(EXIT_FAILURE);
    }

    sin.sin_addr = *(IN_ADDR *)hostinfo->h_addr;
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if (connect(sock, (SOCKADDR *)&sin, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        perror("connect()");
        exit(errno);
    }

    return sock;
}

void end_connection(int sock) {
    closesocket(sock);
}

int read_server(SOCKET sock, char *buffer) {
    int n = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (n < 0) {
        perror("recv()");
        return -1;
    }
    buffer[n] = '\0';
    return n;
}

void write_server(SOCKET sock, const char *buffer) {
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("send()");
        exit(errno);
    }
}

void handle_server_message(const char *message, int sock, int *my_player_number, const char *my_name, char *player1_name, char *player2_name) {
    if (strncmp(message, "GAME_STATE", 10) == 0) {
        GameState game_state;
        deserialize_game_state(message, &game_state);

        // Afficher le plateau avec les noms des joueurs
        display_board(&game_state, player1_name, player2_name, *my_player_number);

        if (game_state.game_over) {
            printf("La partie est terminée.\n");
        } else if (game_state.current_player == *my_player_number) {
            printf("Votre tour. Entrez l'indice du trou à jouer : ");
            char input[BUF_SIZE];
            fgets(input, BUF_SIZE - 1, stdin);
            input[strcspn(input, "\n")] = 0;

            char move_command[BUF_SIZE];
            sprintf(move_command, "MOVE %s", input);
            write_server(sock, move_command);
        } else {
            printf("En attente du coup de l'adversaire...\n");
        }
    } else if (strncmp(message, "CHALLENGE_FROM", 14) == 0) {
        char opponent_name[BUF_SIZE];
        sscanf(message + 15, "%s", opponent_name);
        printf("Vous avez été défié par %s. Accepter ? (oui/non) : ", opponent_name);
        char response[BUF_SIZE];
        fgets(response, BUF_SIZE, stdin);
        response[strcspn(response, "\n")] = 0;

        if (strcmp(response, "oui") == 0) {
            char accept_command[BUF_SIZE];
            sprintf(accept_command, "ACCEPT %s", opponent_name);
            write_server(sock, accept_command);
        } else {
            printf("Défi refusé.\n");
        }
    } else if (strncmp(message, "PLAYER_NUMBER", 13) == 0) {
        int player_num;
        sscanf(message + 14, "%d", &player_num);
        *my_player_number = player_num;
        printf("Vous êtes le joueur %d.\n", *my_player_number + 1);
    } else if (strncmp(message, "Game started.", 13) == 0) {
        printf("%s\n", message);
    } else if (strncmp(message, "Players:", 8) == 0) {
        sscanf(message, "Players: %s %s", player1_name, player2_name);
        printf("Les joueurs de la partie : %s (Joueur 1) et %s (Joueur 2)\n", player1_name, player2_name);
    } else {
        printf("%s\n", message);
    }
}

void app(const char *address, const char *name) {
    SOCKET sock = init_connection(address);
    char buffer[BUF_SIZE];
    fd_set rdfs;

    write_server(sock, name);
    int my_player_number = -1;
    char player1_name[BUF_SIZE] = {0};
    char player2_name[BUF_SIZE] = {0};

    while (1) {
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(sock, &rdfs);

        if (select(sock + 1, &rdfs, NULL, NULL, NULL) == -1) {
            perror("select()");
            exit(errno);
        }

        if (FD_ISSET(STDIN_FILENO, &rdfs)) {
            fgets(buffer, BUF_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            write_server(sock, buffer);
        } else if (FD_ISSET(sock, &rdfs)) {
            int n = read_server(sock, buffer);
            if (n == 0) {
                printf("Le serveur a été déconnecté !\n");
                break;
            }
            handle_server_message(buffer, sock, &my_player_number, name, player1_name, player2_name);
        }
    }

    end_connection(sock);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage : %s [adresse] [pseudo]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *address = argv[1];
    const char *name = argv[2];

    init();
    app(address, name);
    end();

    return EXIT_SUCCESS;
}
