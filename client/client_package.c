#include "client_package.h" 
#include <ctype.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "../options.h" // Inclusion du fichier d'en-tête options.h

int process(char *buffer) { // Déclaration de la fonction process qui prend un pointeur vers un buffer de caractères
  char *it = buffer; // Initialisation d'un pointeur it au début du buffer

  if (*it++ != '/') { // Vérifie si le premier caractère du buffer n'est pas '/'
    char *p = NULL; // Déclare un pointeur p initialisé à NULL
    p = strstr(buffer, "\n"); // Recherche la première occurrence de '\n' dans le buffer
    if (p != NULL) { // Si '\n' est trouvé
      *p = 0; // Remplace '\n' par '\0'
    } else { // Si '\n' n'est pas trouvé
      buffer[BUF_SIZE - 1] = 0; // Assure que le dernier caractère du buffer est '\0'
    }
    return 1; // Retourne 1 pour indiquer que le traitement est terminé
  } else { // Si le premier caractère est '/'
    char command[6]; // Déclare un tableau de caractères pour stocker la commande
    strncpy(command, it, 5); // Copie les 5 caractères suivants dans command
    command[5] = '\0'; // Ajoute le caractère de fin de chaîne

    if (!strcmp(command, "users")) { // Si la commande est "users"
      strcpy(buffer, "/000"); // Remplace le buffer par "/000"
      return 1; // Retourne 1 pour indiquer que le traitement est terminé
    } else if (!strcmp(command, "fight")) { // Si la commande est "fight"
      it += 5; // Avance le pointeur it de 5 positions
      if (*(it++) != ' ') { // Vérifie si le caractère suivant n'est pas un espace
        puts("Expected cmd /fight <username>"); // Affiche un message d'erreur
        return 0; // Retourne 0 pour indiquer une erreur
      }
      char *beginning_of_username = it; // Pointeur vers le début du nom d'utilisateur
      char *end_of_username; // Pointeur vers la fin du nom d'utilisateur
      while (!isspace(*it)) { // Parcourt les caractères jusqu'à un espace
        ++it;
      }
      end_of_username = it; // Définit la fin du nom d'utilisateur
      int length_of_username = end_of_username - beginning_of_username; // Calcule la longueur du nom d'utilisateur
      if (length_of_username >= USERNAME_SIZE) { // Vérifie si la longueur dépasse USERNAME_SIZE
        puts("Username's length has to be less than USERNAME_SIZE"); // Affiche un message d'erreur
        return 0; // Retourne 0 pour indiquer une erreur
      }
      strcpy(buffer, "/001 "); // Remplace le buffer par "/001 "
      char username[USERNAME_SIZE]; // Déclare un tableau pour le nom d'utilisateur
      strncpy(username, beginning_of_username, length_of_username); // Copie le nom d'utilisateur dans username
      for (int i = length_of_username; i < USERNAME_SIZE; ++i) { // Remplit le reste de username avec '\0'
        username[i] = '\0';
      }
      strcat(buffer, username); // Concatène le nom d'utilisateur au buffer
      return 1; // Retourne 1 pour indiquer que le traitement est terminé
    } else if (!strcmp(command, "games")) { // Si la commande est "games"
      strcpy(buffer, "/002"); // Remplace le buffer par "/002"
      return 1; // Retourne 1 pour indiquer que le traitement est terminé
    } else if (!strcmp(command, "plays")) { // Si la commande est "plays"
      int num; // Déclare une variable pour le numéro de pit
      if (sscanf(buffer, "%s %d", command, &num) == 2) { // Analyse le buffer pour extraire la commande et le numéro
        if (num >= 1 && num <= 6) { // Vérifie si le numéro est entre 1 et 6
          sprintf(buffer, "%s %d", "/003", num); // Formate le buffer avec la commande et le numéro
          return 1; // Retourne 1 pour indiquer que le traitement est terminé
        } else {
          printf("The number must be between 1 and 6.\n"); // Affiche un message d'erreur
          return 0; // Retourne 0 pour indiquer une erreur
        }
      } else {
        printf("Expected cmd /plays <pit_num>. \n"); // Affiche un message d'erreur
        return 0; // Retourne 0 pour indiquer une erreur
      }
    } else if (!strcmp(command, "watch")) { // Si la commande est "watch"
      it += 5; // Avance le pointeur it de 5 positions
      if (*(it++) != ' ') { // Vérifie si le caractère suivant n'est pas un espace
        puts("Expected cmd /watch <username>"); // Affiche un message d'erreur
        return 0; // Retourne 0 pour indiquer une erreur
      }
      char *beginning_of_username = it; // Pointeur vers le début du nom d'utilisateur
      char *end_of_username; // Pointeur vers la fin du nom d'utilisateur
      while (!isspace(*it)) { // Parcourt les caractères jusqu'à un espace
        ++it;
      }
      end_of_username = it; // Définit la fin du nom d'utilisateur
      int length_of_username = end_of_username - beginning_of_username; // Calcule la longueur du nom d'utilisateur
      if (length_of_username >= USERNAME_SIZE) { // Vérifie si la longueur dépasse USERNAME_SIZE
        puts("Username's length has to be less than USERNAME_SIZE"); // Affiche un message d'erreur
        return 0; // Retourne 0 pour indiquer une erreur
      }
      strcpy(buffer, "/004 "); // Remplace le buffer par "/004 "
      char username[USERNAME_SIZE]; // Déclare un tableau pour le nom d'utilisateur
      strncpy(username, beginning_of_username, length_of_username); // Copie le nom d'utilisateur dans username
      for (int i = length_of_username; i < USERNAME_SIZE; ++i) { // Remplit le reste de username avec '\0'
        username[i] = '\0';
      }
      strcat(buffer, username); // Concatène le nom d'utilisateur au buffer
      return 1; // Retourne 1 pour indiquer que le traitement est terminé
    } else if (!strcmp(command, "chbio")) { // Si la commande est "chbio"
      it += 5; // Avance le pointeur it de 5 positions
      if (*(it++) != ' ') { // Vérifie si le caractère suivant n'est pas un espace
        puts("Expected cmd /chbio <bio>"); // Affiche un message d'erreur
        return 0;
      }
      char *beginning_of_bio = it;
      char *end_of_bio;
      while (*it != '\n') {
        ++it;
      }
      end_of_bio = it;
      int length_of_bio = end_of_bio - beginning_of_bio;
      if (length_of_bio >= BIO_SIZE) {
        puts("Bio's length has to be less than BIO_SIZE");
        return 0;
      }
      strcpy(buffer, "/005 ");
      char bio[BIO_SIZE];
      strncpy(bio, beginning_of_bio, length_of_bio);
      for (int i = length_of_bio; i < BIO_SIZE; ++i) {
        bio[i] = '\0';
      }
      strcat(buffer, bio);
      return 1;
    } else if (!strcmp(command, "shbio")) {
      it += 5;
      if (*(it++) != ' ') {
        puts("Expected cmd /shbio <username>");
        return 0;
      }
      char *beginning_of_username = it;
      char *end_of_username;
      while (!isspace(*it)) {
        ++it;
      }
      end_of_username = it;
      int length_of_username = end_of_username - beginning_of_username;
      if (length_of_username >= USERNAME_SIZE) {
        puts("Username's length has to be less than USERNAME_SIZE");
        return 0;
      }
      strcpy(buffer, "/006 ");
      char username[USERNAME_SIZE];
      strncpy(username, beginning_of_username, length_of_username);
      for (int i = length_of_username; i < USERNAME_SIZE; ++i) {
        username[i] = '\0';
      }
      strcat(buffer, username);
      return 1;
    } else if (!strcmp(command, "addfr")) {
      it += 5;
      if (*(it++) != ' ') {
        puts("Expected cmd /addfr <username>");
        return 0;
      }
      char *beginning_of_username = it;
      char *end_of_username;
      while (!isspace(*it)) {
        ++it;
      }
      end_of_username = it;
      int length_of_username = end_of_username - beginning_of_username;
      if (length_of_username >= USERNAME_SIZE) {
        puts("Username's length has to be less than USERNAME_SIZE");
        return 0;
      }
      strcpy(buffer, "/007 ");
      char username[USERNAME_SIZE];
      strncpy(username, beginning_of_username, length_of_username);
      for (int i = length_of_username; i < USERNAME_SIZE; ++i) {
        username[i] = '\0';
      }
      strcat(buffer, username);
      return 1;
    } else if (!strcmp(command, "frnds")) {
      strcpy(buffer, "/008");
      return 1;
    } else if (!strcmp(command, "privt")) {
      strcpy(buffer, "/009");
      return 1;
    } else if (!strcmp(command, "leave")) {
      strcpy(buffer, "/010");
      return 1;
    } else if(!strcmp(command, "hgame")) {
      strcpy(buffer, "/011");
      return 1;  
    } else if(!strcmp(command, "rgame")) {
      int num;
      if (sscanf(buffer, "%s %d", command, &num) == 2) {
          sprintf(buffer, "%s %d", "/012", num);
          return 1;        
      } else {
        printf("Expected cmd /rgame <game_id>. \n");
        return 0; // Retourne 0 si le format est invalide
      }
      return 1;  
    } else  {
      puts("Command does not exist..");
      return 0;
    }
    return 0;
  }
}
