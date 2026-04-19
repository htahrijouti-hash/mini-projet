#include "render.h"

#include <stdio.h>

#include "constants.h"

static const char *DifficultyText(Difficulty difficulty) {
    if (difficulty == DIFF_EASY) {
        return "Facile";
    }
    if (difficulty == DIFF_HARD) {
        return "Difficile";
    }
    return "Normal";
}

static void PrintSeparator(void) {
    printf("+----+----+----+----+\n");
}

void DrawMenu(const GameState *state) {
    printf("\n==============================\n");
    printf(" HAMZA TAHIRI ET MARWAN OUALI\n");
    printf(" Cartes Jumelles (Console C)\n");
    printf(" Idee: M. Tahiri Hamza\n");
    printf("==============================\n");
    printf("Difficulte actuelle: %s\n", DifficultyText(state->difficulty));
    printf("Meilleur score: %d\n", state->best_score);
    printf("1) Facile  2) Normal  3) Difficile\n");
    printf("4) Commencer  0) Quitter\n");
}

void DrawGame(const GameState *state) {
    int r;
    int c;

    printf("\nScore: %d | Meilleur: %d | Vies: %d | Paires: %d/%d\n",
           state->score,
           state->best_score,
           state->lives,
           state->pairs_found,
           TARGET_PAIRS);

    PrintSeparator();
    for (r = 0; r < BOARD_ROWS; r++) {
        for (c = 0; c < BOARD_COLS; c++) {
            int idx = r * BOARD_COLS + c;
            if (state->matched[idx] || state->revealed[idx]) {
                if (state->values[idx] == TRAP_CARD_VALUE) {
                    printf("|  ! ");
                } else {
                    printf("| %2d ", state->values[idx]);
                }
            } else {
                printf("| %2d ", idx + 1);
            }
        }
        printf("|\n");
        PrintSeparator();
    }

    if (state->phase == PHASE_GAME_OVER) {
        if (state->won) {
            printf("\nVICTOIRE!\n");
        } else {
            printf("\nGAME OVER\n");
        }
        printf("Score final: %d\n", state->score);
    }
}
