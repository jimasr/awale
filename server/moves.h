#ifndef MOVES_H
#define MOVES_H

/**
 * @struct Move
 * @brief Représente un mouvement dans le jeu.
 *
 * Cette structure contient les informations sur un mouvement effectué par un joueur.
 * 
 * @var Move::player
 * Nom du joueur qui a effectué le mouvement.
 * 
 * @var Move::value
 * Valeur associée au mouvement.
 * 
 * @var Move::next
 * Pointeur vers le mouvement suivant dans la liste.
 * 
 * @var Move::previous
 * Pointeur vers le mouvement précédent dans la liste.
 */
typedef struct Move {
  char player[10];
  int value;
  struct Move *next;
  struct Move *previous;
} Move;

/**
 * @struct Moves
 * @brief Représente une liste de mouvements.
 *
 * Cette structure contient une liste de mouvements effectués dans le jeu.
 * 
 * @var Moves::first
 * Pointeur vers le premier mouvement de la liste.
 * 
 * @var Moves::size
 * Taille de la liste de mouvements.
 * 
 * @var Moves::last
 * Pointeur vers le dernier mouvement de la liste.
 */
typedef struct Moves {
  Move *first;
  int size;
  Move *last;
} Moves;

#endif
