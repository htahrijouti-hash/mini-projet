#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "constants.h"

typedef enum {
    PHASE_MENU,
    PHASE_PLAYING,
    PHASE_GAME_OVER
} GamePhase;

typedef enum {
    DIFF_EASY,
    DIFF_NORMAL,
    DIFF_HARD
} Difficulty;

typedef struct {
    int values[CARD_COUNT];
    bool revealed[CARD_COUNT];
    bool matched[CARD_COUNT];
    int cursor;
    int first_pick;
    int second_pick;
    bool resolving_mismatch;
    float mismatch_seconds;
    int score;
    int best_score;
    int lives;
    int starting_lives;
    int pairs_found;
    bool won;
    Difficulty difficulty;
    GamePhase phase;
    bool event_pair;
    bool event_mismatch;
    bool event_trap;
    bool event_game_over;
    float trap_feedback_seconds;
} GameState;

void GameInit(GameState *state, int best_score);
void GameStart(GameState *state);
void GameSetDifficulty(GameState *state, Difficulty difficulty);
void GameMoveCursor(GameState *state, int dx, int dy);
void GameSelectCard(GameState *state);
void GameUpdate(GameState *state, float delta_seconds);

#endif
