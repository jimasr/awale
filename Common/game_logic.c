#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void init_game(GameState *game) {
    srand(time(NULL));
    for (int i = 0; i < NUM_PITS; i++) {
        game->pits[i] = INITIAL_SEEDS;
    }
    game->captured[0] = 0;
    game->captured[1] = 0;
    game->current_player = 0;
    game->game_over = 0;
}

int is_valid_move(const GameState *game, int pit_index, int player_number) {
    int start = player_number * PLAYER_PITS;
    int end = start + PLAYER_PITS - 1;

    if (pit_index < start || pit_index > end || game->pits[pit_index] == 0) {
        return 0;
    }
    return 1;
}

int opponent_can_play(const GameState *game) {
    int opponent = 1 - game->current_player;
    int start = opponent * PLAYER_PITS;
    int end = start + PLAYER_PITS - 1;

    for (int i = start; i <= end; i++) {
        if (game->pits[i] > 0) {
            return 1;
        }
    }
    return 0;
}

void perform_captures(GameState *game, int last_position) {
    int opponent_start = (1 - game->current_player) * PLAYER_PITS;
    int opponent_end = opponent_start + PLAYER_PITS - 1;
    int captures = 0;

    while (last_position >= opponent_start && last_position <= opponent_end) {
        int seeds = game->pits[last_position];
        if (seeds == 2 || seeds == 3) {
            captures += seeds;
            game->pits[last_position] = 0;
            last_position--;
        } else {
            break;
        }
    }
    game->captured[game->current_player] += captures;
    printf("Captures après mouvement : Joueur %d a capturé %d graines. Total : %d\n",
           game->current_player + 1, captures, game->captured[game->current_player]);
}

int make_move(GameState *game, int pit_index) {
    if (!is_valid_move(game, pit_index, game->current_player)) {
        return -1;
    }

    int seeds = game->pits[pit_index];
    game->pits[pit_index] = 0;
    int position = pit_index;

    printf("Avant distribution, état du plateau :\n");
    for (int i = 0; i < NUM_PITS; i++) {
        printf("Trou %d : %d graines\n", i, game->pits[i]);
    }

    while (seeds > 0) {
        position = (position + 1) % NUM_PITS;
        if (position != pit_index) {
            game->pits[position]++;
            seeds--;
        }
    }

    printf("Après distribution, état du plateau :\n");
    for (int i = 0; i < NUM_PITS; i++) {
        printf("Trou %d : %d graines\n", i, game->pits[i]);
    }

    perform_captures(game, position);

    if (!opponent_can_play(game)) {
        game->game_over = 1;
        for (int i = 0; i < NUM_PITS; i++) {
            game->captured[game->current_player] += game->pits[i];
            game->pits[i] = 0;
        }
        return 0;
    }

    game->current_player = 1 - game->current_player;
    return 0;
}

void serialize_game_state(const GameState *game, char *buffer) {
    int index = sprintf(buffer, "GAME_STATE");
    for (int i = 0; i < NUM_PITS; i++) {
        index += sprintf(buffer + index, " %d", game->pits[i]);
    }
    sprintf(buffer + index, " %d %d %d %d", game->captured[0], game->captured[1], game->current_player, game->game_over);
}

void deserialize_game_state(const char *buffer, GameState *game) {
    char temp_buffer[BUF_SIZE];
    strncpy(temp_buffer, buffer, BUF_SIZE - 1);
    temp_buffer[BUF_SIZE - 1] = '\0';

    char *tokens[NUM_PITS + 4];
    int token_count = 0;
    char *token = strtok(temp_buffer, " ");

    if (strcmp(token, "GAME_STATE") != 0) {
        printf("Erreur : message incorrect.\n");
        return;
    }

    while ((token = strtok(NULL, " ")) != NULL && token_count < NUM_PITS + 4) {
        tokens[token_count++] = token;
    }

    if (token_count < NUM_PITS + 4) {
        printf("Erreur : données de l'état du jeu incomplètes.\n");
        return;
    }

    for (int i = 0; i < NUM_PITS; i++) {
        game->pits[i] = atoi(tokens[i]);
    }
    game->captured[0] = atoi(tokens[NUM_PITS]);
    game->captured[1] = atoi(tokens[NUM_PITS + 1]);
    game->current_player = atoi(tokens[NUM_PITS + 2]);
    game->game_over = atoi(tokens[NUM_PITS + 3]);
}

void display_board(const GameState *game, const char *player1_name, const char *player2_name, int my_player_number) {
    if (!game || !player1_name || !player2_name) {
        printf("Erreur : paramètres invalides dans display_board.\n");
        return;
    }

    printf("\nPlateau actuel :\n\n");

    // Afficher les trous de l'adversaire
    printf("%s :\n", (my_player_number == 0) ? player2_name : player1_name);
    printf("  ");
    for (int i = NUM_PITS - 1; i >= PLAYER_PITS; i--) {
        printf(" [%2d] ", game->pits[i]);
    }

    printf("\n---------------------------------------------\n");

    // Afficher vos trous
    printf("%s :\n", (my_player_number == 0) ? player1_name : player2_name);
    printf("  ");
    for (int i = 0; i < PLAYER_PITS; i++) {
        printf(" [%2d] ", game->pits[i]);
    }

    // Afficher les graines capturées
    printf("\n\nGraines capturées :\n");
    printf("%s : %d\n", player1_name, game->captured[0]);
    printf("%s : %d\n", player2_name, game->captured[1]);

    // Indiquer à qui c'est le tour
    if (game->game_over) {
        printf("\nLa partie est terminée.\n");
    } else if (game->current_player == my_player_number) {
        printf("\nC'est à vous de jouer.\n");
    } else {
        printf("\nC'est au tour de l'adversaire.\n");
    }
}
