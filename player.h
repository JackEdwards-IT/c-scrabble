/******************************************************************************
 * Student Name    :    Jack Edwards
 * RMIT Student ID :    S3727853
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * Assignment 2, study period 4, 2020.
 *****************************************************************************/
#include "shared.h"
#include "rules-b.h"

#ifndef PLAYER_H
#define PLAYER_H
/* the maximum length of a player's name */
#define NAMELEN 40
#define EXTRACHARS 2

#define HAND_SIZE 5
/**
 * enumeration that defines the colours that can be used for players
 **/
enum color {
    COL_RED,
    COL_GREEN,
    COL_YELLOW,
    COL_BLUE,
    COL_MAGENTA,
    COL_CYAN,
    COL_RESET
};

/*Enum for input operations*/
enum input_result {
    IR_FAILURE,
    IR_SUCCESS,
    IR_EMPTYLINE,
    IR_EOF = -1
};

/* global variable that holds the colour codes that can be used for formatting
 * strings */
extern char *color_strings[];

/**
 * definition of the player structure.
 **/
struct player {
    char name[NAMELEN + 1];
    enum color color;
    struct game *curgame;
    struct score_list *hand;
    int score;
};

void print_hand(struct player ptr);
char *strdup(const char *);
BOOLEAN add_players();
enum input_result read_int_keyboard(int *ptr);
enum input_result read_string_keyboard(char *string[]);
BOOLEAN player_init(struct player *, const char *, struct game *);
enum move_result player_turn(struct player *);
#endif
