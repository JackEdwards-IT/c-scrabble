/******************************************************************************
 * Student Name    :    Jack Edwards
 * RMIT Student ID :    S3727853
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * Assignment 2, study period 4, 2020.
 *****************************************************************************/
#include "shared.h"
#ifndef RULES_B_H
#define RULES_B_H
struct player;
enum move_result {
    MOVE_QUIT,
    MOVE_SKIP,
    MOVE_SUCCESS,
    MOVE_BOARD_FULL
};
struct coord {
    int x, y;
};

enum orientation {
    HORIZ,
    VERT
};

BOOLEAN validate_move(struct player* theplayer, const char* word,
                      const struct coord* coords, enum orientation orient);

BOOLEAN play_first_move(struct player* theplayer, const char* word,
                        const struct coord* coords, enum orientation orient,
                        int word_length);

BOOLEAN play_normal_move(struct player* theplayer, const char* word,
                         const struct coord* coords, enum orientation orient,
                         int word_length);

int calculate_score(struct player* theplayer);

void shift_letters(struct player* theplayer);
#endif
