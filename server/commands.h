#ifndef COMMANDS_H
#define COMMANDS_H
#include "server_client.h"
#include "game.h"

/**
 * @brief Récupère la liste des clients actifs.
 * 
 * @param clients Liste des clients actifs.
 * @param client Pointeur vers le client.
 * @param buffer Tampon pour stocker la liste des clients.
 */
void get_client_list(ActiveClients, Client *, char *);

/**
 * @brief Envoie une invitation à un autre client.
 * 
 * @param clients Liste des clients actifs.
 * @param client Pointeur vers le client.
 * @param recipient_username Nom d'utilisateur du destinataire.
 * @param buffer Tampon pour stocker l'invitation.
 * @param status Pointeur vers le statut de l'invitation.
 */
void send_invite(ActiveClients, Client *, const char *, char *, int *);

/**
 * @brief Joue une partie de jeu.
 * 
 * @param client Pointeur vers le client.
 * @param move Mouvement à jouer.
 * @param games Liste des jeux.
 */
void play_game(Client *, int, Games * games);

/**
 * @brief Récupère la liste des jeux disponibles.
 * 
 * @param clients Liste des clients actifs.
 * @param client Pointeur vers le client.
 * @param buffer Tampon pour stocker la liste des jeux.
 */
void get_game_list(ActiveClients clients, Client *client, char *buffer);

/**
 * @brief Regarde un utilisateur.
 * 
 * @param clients Liste des clients actifs.
 * @param client Pointeur vers le client.
 * @param username Nom d'utilisateur à regarder.
 * @param buffer Tampon pour stocker les informations de l'utilisateur.
 */
void watch_user(ActiveClients clients, Client *client, char *username, char *buffer);

/**
 * @brief Change la biographie du client.
 * 
 * @param client Pointeur vers le client.
 * @param bio Nouvelle biographie.
 */
void change_bio(Client *client, char *bio);

/**
 * @brief Récupère la biographie d'un utilisateur.
 * 
 * @param clients Liste des clients actifs.
 * @param client Pointeur vers le client.
 * @param username Nom d'utilisateur dont on veut la biographie.
 */
void get_bio(ActiveClients clients, Client *client, char *username);

/**
 * @brief Envoie une demande d'ami à un autre client.
 * 
 * @param clients Liste des clients actifs.
 * @param client Pointeur vers le client.
 * @param recipient_username Nom d'utilisateur du destinataire.
 */
void send_friend_request(ActiveClients clients, Client *client, const char *recipient_username);

/**
 * @brief Envoie la liste d'amis du client.
 * 
 * @param client Pointeur vers le client.
 */
void send_friend_list(Client *client);

/**
 * @brief Active ou désactive le mode privé du client.
 * 
 * @param client Pointeur vers le client.
 */
void toggle_private_mode(Client *client);

/**
 * @brief Quitte une partie de jeu.
 * 
 * @param client Pointeur vers le client.
 * @param games Liste des jeux.
 */
void leave_game(Client *client, Games * games);

/**
 * @brief Termine une partie de jeu.
 * 
 * @param client Pointeur vers le client.
 * @param games Liste des jeux.
 */
void end_game(Client * client, Games * games);

/**
 * @brief Récupère l'historique des jeux du client.
 * 
 * @param client Pointeur vers le client.
 * @param games Liste des jeux.
 * @param buffer Tampon pour stocker l'historique des jeux.
 */
void get_games_history(Client * client, Games *games, char *buffer);

/**
 * @brief Trouve un jeu par son identifiant.
 * 
 * @param games Liste des jeux.
 * @param game_id Identifiant du jeu.
 * @return Pointeur vers le jeu trouvé.
 */
Game * find_game_by_id(Games *games, int game_id);

/**
 * @brief Revoir une partie de jeu.
 * 
 * @param client Pointeur vers le client.
 * @param games Liste des jeux.
 * @param game_id Identifiant du jeu à revoir.
 * @param buffer Tampon pour stocker les informations du jeu.
 */
void rewatch_game(Client * client, Games *games,int game_id, char *buffer);

#endif