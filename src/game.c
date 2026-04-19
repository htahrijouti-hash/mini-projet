#include "game.h"

#include <stdlib.h>

static bool IsTrapCard(int value) {
    return value == TRAP_CARD_VALUE;
}

static int LivesForDifficulty(Difficulty difficulty) {
    if (difficulty == DIFF_EASY) {
        return 6;
    }
    if (difficulty == DIFF_HARD) {
        return 3;
    }
    return 4;
}

static void ShuffleDeck(GameState *state) {
    int deck[CARD_COUNT];
    int i;
    int normal_pairs = TARGET_PAIRS;

    for (i = 0; i < normal_pairs; i++) {
        deck[i * 2] = i + 1;
        deck[i * 2 + 1] = i + 1;
    }

    deck[CARD_COUNT - 1] = TRAP_CARD_VALUE;
    deck[CARD_COUNT - 2] = TRAP_CARD_VALUE;

    for (i = CARD_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }

    for (i = 0; i < CARD_COUNT; i++) {
        state->values[i] = deck[i];
        state->revealed[i] = false;
        state->matched[i] = false;
    }
}

void GameSetDifficulty(GameState *state, Difficulty difficulty) {
    state->difficulty = difficulty;
    state->starting_lives = LivesForDifficulty(difficulty);
    if (state->phase == PHASE_MENU) {
        state->lives = state->starting_lives;
    }
}

void GameInit(GameState *state, int best_score) {
    state->best_score = best_score;
    state->difficulty = DIFF_NORMAL;
    state->starting_lives = LivesForDifficulty(state->difficulty);
    state->phase = PHASE_MENU;
    state->won = false;
    state->event_pair = false;
    state->event_mismatch = false;
    state->event_trap = false;
    state->event_game_over = false;
    state->trap_feedback_seconds = 0.0f;

    GameStart(state);
    state->phase = PHASE_MENU;
}

void GameStart(GameState *state) {
    ShuffleDeck(state);

    state->cursor = 0;
    state->first_pick = -1;
    state->second_pick = -1;
    state->resolving_mismatch = false;
    state->mismatch_seconds = 0.0f;
    state->score = 0;
    state->lives = state->starting_lives;
    state->pairs_found = 0;
    state->won = false;
    state->phase = PHASE_PLAYING;

    state->event_pair = false;
    state->event_mismatch = false;
    state->event_trap = false;
    state->event_game_over = false;
    state->trap_feedback_seconds = 0.0f;
}

void GameMoveCursor(GameState *state, int dx, int dy) {
    int row;
    int col;

    if (state->phase != PHASE_PLAYING || state->resolving_mismatch) {
        return;
    }

    row = state->cursor / BOARD_COLS;
    col = state->cursor % BOARD_COLS;

    row += dy;
    col += dx;

    if (row < 0) {
        row = BOARD_ROWS - 1;
    }
    if (row >= BOARD_ROWS) {
        row = 0;
    }
    if (col < 0) {
        col = BOARD_COLS - 1;
    }
    if (col >= BOARD_COLS) {
        col = 0;
    }

    state->cursor = row * BOARD_COLS + col;
}

void GameSelectCard(GameState *state) {
    int idx;

    if (state->phase != PHASE_PLAYING || state->resolving_mismatch) {
        return;
    }

    idx = state->cursor;

    if (state->matched[idx] || state->revealed[idx]) {
        return;
    }

    state->revealed[idx] = true;

    if (state->first_pick < 0) {
        state->first_pick = idx;
        return;
    }

    state->second_pick = idx;

    if (IsTrapCard(state->values[state->first_pick]) || IsTrapCard(state->values[state->second_pick])) {
        state->lives -= TRAP_LIFE_PENALTY;
        state->event_trap = true;
        state->trap_feedback_seconds = FLIP_BACK_SECONDS;

        if (state->lives <= 0) {
            state->lives = 0;
            state->won = false;
            state->phase = PHASE_GAME_OVER;
            state->event_game_over = true;
            if (state->score > state->best_score) {
                state->best_score = state->score;
            }
            return;
        }

        state->resolving_mismatch = true;
        state->mismatch_seconds = FLIP_BACK_SECONDS;
        return;
    }

    if (state->values[state->first_pick] == state->values[state->second_pick]) {
        state->matched[state->first_pick] = true;
        state->matched[state->second_pick] = true;
        state->pairs_found++;
        state->score += SCORE_PER_PAIR;
        state->event_pair = true;

        state->first_pick = -1;
        state->second_pick = -1;

        if (state->pairs_found == TARGET_PAIRS) {
            state->won = true;
            state->phase = PHASE_GAME_OVER;
            state->event_game_over = true;
            if (state->score > state->best_score) {
                state->best_score = state->score;
            }
        }
        return;
    }

    state->lives--;
    state->event_mismatch = true;

    if (state->lives <= 0) {
        state->won = false;
        state->phase = PHASE_GAME_OVER;
        state->event_game_over = true;
        if (state->score > state->best_score) {
            state->best_score = state->score;
        }
        return;
    }

    state->resolving_mismatch = true;
    state->mismatch_seconds = FLIP_BACK_SECONDS;
}

void GameUpdate(GameState *state, float delta_seconds) {
    state->event_pair = false;
    state->event_mismatch = false;
    state->event_trap = false;
    state->event_game_over = false;

    if (state->phase != PHASE_PLAYING) {
        return;
    }

    if (state->resolving_mismatch) {
        state->mismatch_seconds -= delta_seconds;
        if (state->mismatch_seconds <= 0.0f) {
            if (state->first_pick >= 0) {
                state->revealed[state->first_pick] = false;
            }
            if (state->second_pick >= 0) {
                state->revealed[state->second_pick] = false;
            }
            state->first_pick = -1;
            state->second_pick = -1;
            state->resolving_mismatch = false;
            state->mismatch_seconds = 0.0f;
        }
    }

    if (state->trap_feedback_seconds > 0.0f) {
        state->trap_feedback_seconds -= delta_seconds;
        if (state->trap_feedback_seconds < 0.0f) {
            state->trap_feedback_seconds = 0.0f;
        }
    }
}
