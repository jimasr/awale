// common/game_logic.h

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define NUM_PITS 12       // Nombre total de trous sur le plateau
#define PLAYER_PITS 6     // Nombre de trous par joueur
#define INITIAL_SEEDS 4   // Nombre initial de graines par trou

#define BUF_SIZE 1024     // Taille du buffer pour les messages

typedef struct {
    int pits[NUM_PITS];   // Les trous du plateau
    int captured[2];      // Graines capturées par chaque joueur [joueur1, joueur2]
    int current_player;   // 0 pour Joueur 1, 1 pour Joueur 2
    int game_over;        // 0 si le jeu est en cours, 1 si terminé
} GameState;

// Prototypes des fonctions
void init_game(GameState *game);
int is_valid_move(const GameState *game, int pit_index, int player_number);
int make_move(GameState *game, int pit_index);
void serialize_game_state(const GameState *game, char *buffer);
void deserialize_game_state(const char *buffer, GameState *game);
void display_board(const GameState *game, const char *player1_name, const char *player2_name, int my_player_number);

#endif // GAME_LOGIC_H
