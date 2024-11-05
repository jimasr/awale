#ifndef CLIENT_H
#define CLIENT_H

#ifdef WIN32

#include <winsock2.h>

#elif defined(linux) || defined(__APPLE__)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
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

#define BUF_SIZE 1024

#include "../common/game_logic.h"

void init(void);
void end(void);
int init_connection(const char *address);
void end_connection(int sock);
int read_server(SOCKET sock, char *buffer);
void write_server(SOCKET sock, const char *buffer);
void handle_server_message(const char *message, int sock, int *my_player_number, const char *my_name, char *player1_name, char *player2_name);
void app(const char *address, const char *name);

#endif /* CLIENT_H */
