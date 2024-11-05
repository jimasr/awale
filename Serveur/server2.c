#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "server2.h"

#define MAX_GAMES 50
Game *games[MAX_GAMES];
int game_count = 0;

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

int init_connection(void) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("socket()");
        exit(errno);
    }

    SOCKADDR_IN sin = { 0 };
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if (bind(sock, (SOCKADDR *)&sin, sizeof sin) == SOCKET_ERROR) {
        perror("bind()");
        exit(errno);
    }

    if (listen(sock, MAX_CLIENTS) == SOCKET_ERROR) {
        perror("listen()");
        exit(errno);
    }

    return sock;
}

void end_connection(int sock) {
    closesocket(sock);
}

int read_client(SOCKET sock, char *buffer) {
    int n = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (n < 0) {
        perror("recv()");
        return 0;
    }
    buffer[n] = '\0';
    return n;
}

void write_client(SOCKET sock, const char *buffer) {
    char message[BUF_SIZE];
    snprintf(message, BUF_SIZE, "%s\n", buffer);
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("send()");
        exit(errno);
    }
}

void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server) {
    int i;
    char message[BUF_SIZE];
    message[0] = 0;

    for (i = 0; i < actual; i++) {
        if (sender.sock != clients[i].sock) {
            if (from_server == 0) {
                strncpy(message, sender.name, BUF_SIZE - 1);
                strncat(message, " : ", sizeof message - strlen(message) - 1);
            }
            strncat(message, buffer, sizeof message - strlen(message) - 1);
            write_client(clients[i].sock, message);
        }
    }
}

void remove_client(Client *clients, int to_remove, int *actual) {
    closesocket(clients[to_remove].sock);
    memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
    (*actual)--;
}

void clear_clients(Client *clients, int actual) {
    int i;
    for (i = 0; i < actual; i++) {
        closesocket(clients[i].sock);
    }
}

Game *create_game(Client *player1, Client *player2) {
    if (game_count >= MAX_GAMES) return NULL;
    Game *new_game = (Game *)malloc(sizeof(Game));
    new_game->player1 = player1;
    new_game->player2 = player2;
    init_game(&(new_game->game_state));
    games[game_count++] = new_game;
    return new_game;
}

Game *find_game_by_client(Game **games, int game_count, Client *client) {
    for (int i = 0; i < game_count; i++) {
        if (games[i]->player1 == client || games[i]->player2 == client) {
            return games[i];
        }
    }
    return NULL;
}

void remove_game(Game **games, int *game_count, int index) {
    free(games[index]);
    for (int i = index; i < (*game_count) - 1; i++) {
        games[i] = games[i + 1];
    }
    (*game_count)--;
}

void send_game_state(Game *game) {
    char buffer[BUF_SIZE];
    serialize_game_state(&(game->game_state), buffer);
    write_client(game->player1->sock, buffer);
    write_client(game->player2->sock, buffer);
}

void announce_winner(Game *game) {
    int score1 = game->game_state.captured[0];
    int score2 = game->game_state.captured[1];
    char message[BUF_SIZE];

    if (score1 > score2) {
        sprintf(message, "Game over! %s a gagné avec %d graines.", game->player1->name, score1);
    } else if (score2 > score1) {
        sprintf(message, "Game over! %s a gagné avec %d graines.", game->player2->name, score2);
    } else {
        sprintf(message, "Game over! Match nul avec %d graines chacun.", score1);
    }

    write_client(game->player1->sock, message);
    write_client(game->player2->sock, message);
}

void send_connected_players(Client *clients, int actual, SOCKET sock) {
    char buffer[BUF_SIZE] = "Connected players: ";
    for (int i = 0; i < actual; i++) {
        if (clients[i].sock != sock) {
            strncat(buffer, clients[i].name, BUF_SIZE - strlen(buffer) - 1);
            strncat(buffer, ", ", BUF_SIZE - strlen(buffer) - 1);
        }
    }
    if (actual > 1) {
        buffer[strlen(buffer) - 2] = '\0';
    }
    write_client(sock, buffer);
}

void handle_client_message(Client *clients, int client_index, char *message, Game **games, int *game_count, int actual) {
    Client *client = &clients[client_index];

    if (strncmp(message, "CHALLENGE", 9) == 0) {
        char opponent_name[BUF_SIZE];
        sscanf(message + 10, "%s", opponent_name);

        int opponent_index = -1;
        for (int i = 0; i < actual; i++) {
            if (strcmp(clients[i].name, opponent_name) == 0) {
                opponent_index = i;
                break;
            }
        }

        if (opponent_index != -1) {
            char challenge_message[BUF_SIZE];
            sprintf(challenge_message, "CHALLENGE_FROM %s", client->name);
            write_client(clients[opponent_index].sock, challenge_message);
        } else {
            write_client(client->sock, "Opponent not found.");
        }
    } else if (strncmp(message, "ACCEPT", 6) == 0) {
        char opponent_name[BUF_SIZE];
        sscanf(message + 7, "%s", opponent_name);

        int opponent_index = -1;
        for (int i = 0; i < actual; i++) {
            if (strcmp(clients[i].name, opponent_name) == 0) {
                opponent_index = i;
                break;
            }
        }

        if (opponent_index != -1) {
            Game *game = create_game(&clients[opponent_index], client);
            if (game != NULL) {
                char start_message[BUF_SIZE];
                sprintf(start_message, "Game started. %s commence la partie.",
                        (game->game_state.current_player == 0) ? clients[opponent_index].name : client->name);
                write_client(clients[opponent_index].sock, start_message);
                write_client(client->sock, start_message);

                char names_message[BUF_SIZE];
                sprintf(names_message, "PLAYER_NAMES %s %s", clients[opponent_index].name, client->name);
                write_client(clients[opponent_index].sock, names_message);
                write_client(client->sock, names_message);

                write_client(clients[opponent_index].sock, "PLAYER_NUMBER 0");
                write_client(client->sock, "PLAYER_NUMBER 1");

                send_game_state(game);
            } else {
                write_client(client->sock, "Cannot create a new game at the moment.");
            }
        } else {
            write_client(client->sock, "Opponent not found.");
        }
    } else if (strncmp(message, "MOVE", 4) == 0) {
        int pit_index = atoi(message + 5);

        Game *game = find_game_by_client(games, *game_count, client);
        if (game != NULL) {
            int player_number = (game->player1 == client) ? 0 : 1;
            int start = player_number * PLAYER_PITS;
            int end = start + PLAYER_PITS - 1;

            if (pit_index < start || pit_index > end) {
                write_client(client->sock, "Invalid move: out of range.");
                return;
            }

            if (game->game_state.current_player == player_number) {
                if (is_valid_move(&(game->game_state), pit_index, player_number)) {
                    int result = make_move(&(game->game_state), pit_index);
                    if (result == -1) {
                        write_client(client->sock, "Invalid move: pit is empty.");
                    } else if (result == -2) {
                        write_client(client->sock, "Invalid move: starving opponent.");
                    } else {
                        send_game_state(game);
                        if (game->game_state.game_over) {
                            announce_winner(game);
                            for (int i = 0; i < *game_count; i++) {
                                if (games[i] == game) {
                                    remove_game(games, game_count, i);
                                    break;
                                }
                            }
                        }
                    }
                } else {
                    write_client(client->sock, "Invalid move.");
                }
            } else {
                write_client(client->sock, "It's not your turn.");
            }
        } else {
            write_client(client->sock, "You are not currently in a game.");
        }
    } else {
        send_message_to_all_clients(clients, *client, actual, message, 0);
    }
}

void app(void) {
    SOCKET sock = init_connection();
    char buffer[BUF_SIZE];
    Client clients[MAX_CLIENTS];
    int actual = 0;
    int max = sock;
    fd_set rdfs;

    while (1) {
        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(sock, &rdfs);

        for (int i = 0; i < actual; i++) {
            FD_SET(clients[i].sock, &rdfs);
        }

        if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1) {
            perror("select()");
            exit(errno);
        }

        if (FD_ISSET(STDIN_FILENO, &rdfs)) {
            break;
        } else if (FD_ISSET(sock, &rdfs)) {
            SOCKADDR_IN csin = { 0 };
            socklen_t sinsize = sizeof csin;
            int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
            if (csock == SOCKET_ERROR) {
                perror("accept()");
                continue;
            }

            if (read_client(csock, buffer) == -1) {
                continue;
            }

            max = csock > max ? csock : max;
            FD_SET(csock, &rdfs);

            Client c = { csock };
            strncpy(c.name, buffer, BUF_SIZE - 1);
            clients[actual++] = c;

            printf("%s s'est connecté.\n", c.name);
            send_connected_players(clients, actual, csock);

            for (int i = 0; i < actual; i++) {
                if (clients[i].sock != csock) {
                    char notify_message[BUF_SIZE];
                    snprintf(notify_message, BUF_SIZE, "%s s'est connecté.", c.name);
                    write_client(clients[i].sock, notify_message);
                }
            }
        } else {
            for (int i = 0; i < actual; i++) {
                if (FD_ISSET(clients[i].sock, &rdfs)) {
                    int c = read_client(clients[i].sock, buffer);
                    if (c == 0) {
                        printf("%s s'est déconnecté.\n", clients[i].name);
                        remove_client(clients, i, &actual);

                        for (int j = 0; j < actual; j++) {
                            char notify_message[BUF_SIZE];
                            snprintf(notify_message, BUF_SIZE, "%s s'est déconnecté.", clients[i].name);
                            write_client(clients[j].sock, notify_message);
                        }
                        break;
                    } else {
                        handle_client_message(clients, i, buffer, games, &game_count, actual);
                    }
                }
            }
        }
    }

    clear_clients(clients, actual);
    end_connection(sock);
}

int main() {
    init();
    app();
    end();
    return EXIT_SUCCESS;
}
