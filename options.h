#ifndef OPTIONS_H 
#define OPTIONS_H 

#define USERNAME_SIZE 40 // Taille maximale du nom d'utilisateur
#define BIO_SIZE 160 // Taille maximale de la biographie
#define MAX_CLIENTS 25 // Nombre maximal de clients
#define INVALID_SOCKET -1 // Valeur pour un socket invalide
#define SOCKET_ERROR -1 // Valeur pour une erreur de socket
#define closesocket(s) close(s) // Redéfinition de closesocket pour utiliser close

typedef int SOCKET; // Définition de SOCKET comme un entier
typedef struct sockaddr_in SOCKADDR_IN; // Définition de SOCKADDR_IN comme struct sockaddr_in
typedef struct sockaddr SOCKADDR; // Définition de SOCKADDR comme struct sockaddr
typedef struct in_addr IN_ADDR; // Définition de IN_ADDR comme struct in_addr

#define CRLF "\r\n" // Définition de CRLF comme retour chariot et saut de ligne
#define PORT 7777 // Définition du port à 7777

#define BUF_SIZE 1024 // Taille du buffer, à adapter au nombre de clients pour la fonction get_client_list()

#endif 
