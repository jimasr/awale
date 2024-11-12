#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../options.h"

void clean_newline(char *str) {
    str[strcspn(str, "\n")] = '\0'; // Remplace le '\n' par '\0'
}

int process(char *buffer)
{
  char *it = buffer;      // Pointeur pour parcourir le buffer
  char command[BUF_SIZE]; // Stocke la commande extraite
  int i = 0;

  // Vérifie si le message ne commence pas par '/'
  if (*it++ != '/')
  {
    char *p = strstr(buffer, "\n"); // Trouve la fin de ligne
    if (p)
    {
      *p = '\0'; // Remplace par un terminateur nul
    }
    else
    {
      buffer[BUF_SIZE - 1] = '\0'; // Assure la fin de chaîne
    }
    return 1; // Message standard
  }

  // Lecture de la commande jusqu'à un espace ou fin de chaîne
  while (*it && !isspace(*it) && i < BUF_SIZE - 1)
  {
    command[i++] = *it++;
  }
  command[i] = '\0'; // Ajoute le terminateur nul

  // Sauter les espaces avant les arguments de la commande
  while (*it && isspace(*it))
    it++;

  // Logique de traitement des commandes
  if (!strcmp(command, "list_users"))
  {
    strcpy(buffer, "/000");
    return 1;
  }
  else if (!strcmp(command, "start_duel"))
  {
    if (*it == '\0')
    {
      puts("Expected cmd /start_duel <username>\n");
      return 0;
    }
    clean_newline(it);  // Nettoie le saut de ligne dans l'argument
    strcpy(buffer, "/001 ");
    strcat(buffer, it); // Concatène le nom d'utilisateur
    return 1;
  }
  else if (!strcmp(command, "list_games"))
  {
    strcpy(buffer, "/002");
    return 1;
  }
  else if (!strcmp(command, "make_move"))
  {
    int pit_num;
    if (sscanf(it, "%d", &pit_num) == 1 && pit_num >= 1 && pit_num <= 6)
    {
      clean_newline(it);  // Nettoie le saut de ligne dans l'argument
      sprintf(buffer, "/003 %d", pit_num);
      return 1;
    }
    else
    {
      puts("Invalid move. Usage: /make_move <pit_num>\n");
      return 0;
    }
  }
  else if (!strcmp(command, "watch_player"))
  {
    if (*it == '\0')
    {
      puts("Expected cmd /watch_player <username>\n");
      return 0;
    }
    clean_newline(it);  // Nettoie le saut de ligne dans l'argument
    strcpy(buffer, "/004 ");
    strcat(buffer, it);
    return 1;
  }
  else if (!strcmp(command, "change_bio"))
  {
    if (*it == '\0')
    { // Vérifie si l'argument est vide
      puts("Expected cmd /change_bio <new_bio>\n");
      return 0;
    }

    char bio[BIO_SIZE];
    if (strlen(it) >= BIO_SIZE)
    { // Vérifie la longueur de la bio
      puts("Bio's length exceeds maximum allowed size.\n");
      return 0;
    }

    strncpy(bio, it, BIO_SIZE - 1);
    bio[BIO_SIZE - 1] = '\0'; // Assure la terminaison correcte

    clean_newline(it);  // Nettoie le saut de ligne dans l'argument

    strcpy(buffer, "/005 ");
    strcat(buffer, bio); // Concatène la bio à la commande
    return 1;
  }
  else if (!strcmp(command, "show_bio"))
  {
    if (*it == '\0')
    {
      puts("Expected cmd /show_bio <username>\n");
      return 0;
    }
    clean_newline(it);  // Nettoie le saut de ligne dans l'argument
    strcpy(buffer, "/006 ");
    strcat(buffer, it);
    return 1;
  }
  else if (!strcmp(command, "add_friend"))
  {
    if (*it == '\0')
    {
      puts("Expected cmd /add_friend <username>\n");
      return 0;
    }
    clean_newline(it);  // Nettoie le saut de ligne dans l'argument
    strcpy(buffer, "/007 ");
    strcat(buffer, it);
    return 1;
  }
  else if (!strcmp(command, "list_friends"))
  {
    strcpy(buffer, "/008");
    return 1;
  }
  else if (!strcmp(command, "toggle_private"))
  {
    strcpy(buffer, "/009");
    return 1;
  }
  else if (!strcmp(command, "leave_game"))
  {
    strcpy(buffer, "/010");
    return 1;
  }
  else if (!strcmp(command, "game_history"))
  {
    strcpy(buffer, "/011");
    return 1;
  }
  else if (!strcmp(command, "replay_game"))
  {
    int game_id;
    if (sscanf(it, "%d", &game_id) == 1)
    {
      clean_newline(it);  // Nettoie le saut de ligne dans l'argument
      sprintf(buffer, "/012 %d", game_id);
      return 1;
    }
    else
    {
      puts("Invalid command. Usage: /replay_game <game_id>\n");
      return 0;
    }
  }
  else if (!strcmp(command, "help"))
  {
    strcpy(buffer, "/013");
    return 1;
  }
  else
  {
    puts("Unknown command. Type /help for a list of available commands.\n");
    return 0;
  }
}

