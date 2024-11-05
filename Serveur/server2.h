#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32
#include <winsock2.h>
#elif defined(linux) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#else
#error not defined for this platform
#endif

#define CRLF "\r\n"
#define PORT 1977
#define MAX_CLIENTS 100
#define BUF_SIZE 1024

#include "../common/game_logic.h"

typedef struct {
    SOCKET sock;
    char name[BUF_SIZE];
} Client;

typedef struct {
    Client *player1;
    Client *player2;
    GameState game_state;
} Game;

// Prototypes de fonctions
void init(void);
void end(void);
void app(void);
int init_connection(void);
void end_connection(int sock);
int read_client(SOCKET sock, char *buffer);
void write_client(SOCKET sock, const char *buffer);
void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server);
void remove_client(Client *clients, int to_remove, int *actual);
void clear_clients(Client *clients, int actual);
Game *create_game(Client *player1, Client *player2);
void handle_client_message(Client *clients, int client_index, char *message, Game **games, int *game_count, int actual);
void send_game_state(Game *game);
Game *find_game_by_client(Game **games, int game_count, Client *client);
void remove_game(Game **games, int *game_count, int index);
void announce_winner(Game *game);
void serialize_game_state(const GameState *game, char *buffer);

#endif /* SERVER_H */
