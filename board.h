/******************************************************************************
 * Student Name    :    Jack Edwards
 * RMIT Student ID :    S3727853
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * Assignment 2, study period 4, 2020.
 *****************************************************************************/
#include "shared.h"
#ifndef BOARD_H
#define BOARD_H
#define BOARD_MIN 5
#define BOARD_MAX 50
#define INPUT_CHARS 11
#define EXTRA_CHARS 2

struct player;
struct cell {
    struct player* owner;
    int letter;
    /*adding in a score value to correspond to the letter to make score
     * calculations easier.*/
    int score;
};

struct board {
    struct cell** matrix;
    int width;
    int height;
};

void print_board(struct board* board);
BOOLEAN read_board_size(int* size);
struct board* new_board(int, int);
#endif
