#include "persistence.h"

#include <stdio.h>

#include "constants.h"

int LoadBestScore(void) {
    FILE *file = fopen(BEST_SCORE_FILE, "r");
    int best = 0;

    if (file == NULL) {
        return 0;
    }

    if (fscanf(file, "%d", &best) != 1) {
        best = 0;
    }

    fclose(file);
    return best;
}

void SaveBestScore(int score) {
    FILE *file = fopen(BEST_SCORE_FILE, "w");

    if (file == NULL) {
        return;
    }

    fprintf(file, "%d\n", score);
    fclose(file);
}
