#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

Score score;
int current_player;
int board[BOARD_SIZE];

void initialize_board() {
    for (int i = 0; i < BOARD_SIZE / 2; i++) {
        board[i] = 4;
    }

    for (int i = 6; i < BOARD_SIZE; i++) {
        board[i] = 0;
    }
}

void print_board() {
    printf("=============\n");
    printf("Player 2: ");
    for (int i = BOARD_SIZE / 2; i < BOARD_SIZE; i++) {
        printf("%d ", board[i]);
    }
    printf("\nPlayer 1: ");
    for (int i = 0; i < BOARD_SIZE / 2; i++) {
        printf("%d ", board[i]);
    }      
    printf("\n");
    printf("=============\n\n");

}

void print_score() {
    printf("Current score\n");
    printf("=============\n");
    printf("Player 1 : %d\n", score.score_player1);
    printf("Player 2 : %d\n", score.score_player2);
    printf("=============\n\n");

}

char is_game_over() {
    char over = 0;

    if(score.score_player1 == 24 && score.score_player2 == 24) {
        over = 1;
        printf("Draw");
    } else if(score.score_player2 >= 25) {
        over = 1;
        printf("Player 2 win");
    } else if(score.score_player1 >= 25) {
        over = 1;
        printf("Player 1 win");
    }

    return over;
}

void capture(int pit) {

    if(current_player == PLAYER1) { // P1
        if(pit > 5 && pit < 12) { // in P2 side
            if(board[pit] == 2 || board[pit] == 3) {
                score.score_player1 += board[pit];
                board[pit] = 0;

                // traverse backward from pit to capture seeds
                for (int i = pit - 1; i > 5; i--) {
                    if (board[i] == 2 || board[i] == 3) {
                        // capture seeds in this hole
                        score.score_player1 += board[i];
                        board[i] = 0;
                    } else {
                        // stop capture if the hole does not contain 2 or 3 seeds
                        break;
                    }
                }
            }
        }  
    } else { // P2
        if(pit >= 0 && pit < 6) { // in P1 side
            if(board[pit] == 2 || board[pit] == 3) {
                score.score_player2 += board[pit];
                board[pit] = 0;

                for (int i = pit - 1; i >= 0; i--) {
                    if (board[i] == 2 || board[i] == 3) {
                        // capture seeds in this hole
                        score.score_player2 += board[i];
                        board[i] = 0;
                    } else {
                        // stop capture if the hole does not contain 2 or 3 seeds
                        break;
                    }
                }
            }
        }
    }
} 

/**
 * Returns 1 if opponent doesn't have seed after simulated move, 0 if not
 */
char is_famine(int pit) {
    int simulated_board[BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) {
        simulated_board[i] = board[i];
    }

    int start = (current_player == PLAYER1) ? PLAYER2_START : PLAYER1_START;
    int end = (current_player == PLAYER1) ? PLAYER2_END : PLAYER1_END;

    //simulate board
    int seeds = simulated_board[pit];
    simulated_board[pit] = 0;
    int current_pit = pit;

    while (seeds > 0) {
        current_pit = (current_pit + 1) % BOARD_SIZE;
        if (current_pit != pit) {
            simulated_board[current_pit]++;
            seeds--;
        }
    }

    //  check if simulated board leaves opponent starving
    for (int i = start; i <= end; i++) {
        if (simulated_board[i] > 0) {
            return 0;  // opponent still has seeds
        }
    }
    printf("Don't starve your opponent!\n");
    return 1; 
}

void make_move(int pit) {
    int seeds = board[pit];
    board[pit] = 0;
    int current_pit = pit;

    while (seeds > 0) {
        current_pit = (current_pit + 1) % BOARD_SIZE;
        if (current_pit != pit) {
            board[current_pit]++;
            seeds--;
        }
    }
    
    printf("%d\n", current_pit);
    capture(current_pit);
}

int main(int argc, char **argv) {

    //init 
    score.score_player1 = 0;
    score.score_player2 = 0;

    initialize_board(board);

    while (!is_game_over()) {
        printf("================================================  Player %d  ========================================================\n", current_player + 1);
        print_score();
        print_board();

        printf("Player %d's turn. Choose a pit (0-5): ", current_player + 1);
        int pit;
        scanf("%d", &pit);

        while(pit < 0 || pit >= BOARD_SIZE / 2 || (current_player == PLAYER1 && board[pit] == 0) || (current_player == PLAYER2 && board[pit + 6] == 0) || (current_player == PLAYER2 && is_famine(pit + 6)) || (current_player == PLAYER1 && is_famine(pit))) {
            printf("Invalid pit. Try again.\n");
            printf("Player %d's turn. Choose a pit (0-5): ", current_player + 1);
            scanf("%d", &pit);
        }

        if (current_player == PLAYER2) {
            pit += 6;
        }

        make_move(pit);
        printf("===================================================================================================================\n");

        current_player = (current_player + 1) % 2;
    }

    printf("Game over!\n");
    return 0;
}