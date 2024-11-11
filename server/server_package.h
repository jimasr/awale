
#ifndef SERVER_PACKAGE_H
#define SERVER_PACKAGE_H

#include "server_client.h"

/**
 * @brief Structure représentant un paquet de données du serveur.
 */
typedef struct ServerPackage {
  int nb_args;      ///< Nombre d'arguments dans le paquet.
  char **args;      ///< Tableau de chaînes de caractères représentant les arguments.
} ServerPackage;

/**
 * @brief Gère un paquet de données entrant.
 * 
 * @param clients Liste des clients actifs.
 * @param client Pointeur vers le client qui a envoyé le message.
 * @param message Message reçu du client.
 * @param games Liste des jeux en cours.
 * @param current_gm_id Pointeur vers l'identifiant du jeu en cours.
 */
void handle_incomming_package(const ActiveClients clients, Client *client,
                              char *message, Games *games, int *current_gm_id);

/**
 * @brief Analyse un paquet de données entrant.
 * 
 * @param message Message reçu du client.
 * @return ServerPackage Structure contenant les données analysées.
 */
static ServerPackage parse_incomming_package(char *message);

#endif