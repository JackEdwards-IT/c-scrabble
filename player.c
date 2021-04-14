/******************************************************************************
 * Student Name    :    Jack Edwards
 * RMIT Student ID :    S3727853
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * Assignment 2, study period 4, 2020.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "player.h"
#include "score_list.h"

#include "game.h"
/* the color_strings array that defines the color codes for the printing
 * out colour in the terminal. The order of the values in this array is the
 * same as the color enum
 **/
char *color_strings[] = {"\033[0;31m", "\033[0;32m", "\033[0;33m", "\033[0;34m",
                         "\033[0;35m", "\033[0;36m", "\033[0m"};

/*clear buffer method*/
static void clear_buffer(void) {
    int ch;
    while (ch = getc(stdin), ch != EOF && ch != '\n')
        ;
    clearerr(stdin);
}
/**
 * prompt the user for their name and deal the letters for the player to start
 * with. Also assign the game pointer so we can access it later.
 **/

BOOLEAN add_players(struct game *thegame) {
    int count = 0;
    char *name[NAMELEN + EXTRACHARS];
    BOOLEAN player_error = FALSE;

    while (count < MAX_PLAYERS && !player_error) {

        struct player *theplayer;
        struct score_list *hand;
        enum input_result result = IR_FAILURE;

        printf("\nEnter name for player %d:", count + 1);

        while (result != IR_SUCCESS) {
            result = read_string_keyboard(name);
            if (result == IR_EOF) {
                return FALSE;
            }
        }

        /*allocate memory for player*/
        theplayer = calloc(1, sizeof(struct player));
        if (theplayer == NULL) {
            player_error = TRUE;
        };

        /*allocate memory for players hand*/
        hand = calloc(1, sizeof(struct score_list));
        theplayer->hand = hand;

        /*Initialise the player*/
        if (!player_init(theplayer, *name, thegame)) {
            player_error = TRUE;
            break;
        }

        /*add player to game struct*/
        thegame->players[count] = *theplayer;
        count++;
    }

    if (!player_error) {
        return TRUE;
    };
    return FALSE;
}

/* Initialise the player*/
BOOLEAN player_init(struct player *theplayer, const char *name,
                    struct game *thegame) {
#define MYRANDMAX 5
    enum color randomcolor;
    /*set the random colour to random number from 0 to MYRANDMAX*/
    randomcolor = rand() % (MYRANDMAX + 1) + 0;
    /*Copy player details to player struct*/
    strcpy(theplayer->name, name);
    theplayer->color = randomcolor;
    /*Link player to game*/
    theplayer->curgame = thegame;

    /*Need to deal initial hand*/
    deal_letters(thegame->score_list, theplayer->hand);
    theplayer->score = 0;

    return TRUE;
}

/*Print the players hand*/
void print_hand(struct player theplayer) {
    int count = 0;
    printf("\n%s's Turn. This is your %scolor%s. Current score: %d\n",
           theplayer.name, color_strings[theplayer.color],
           color_strings[COL_RESET], theplayer.score);
    printf("Your hand contains:\n");

    for (count = 0; count < theplayer.hand->total_count; ++count) {
        printf("%4c\t|", theplayer.hand->scores[count].letter);
    }
    printf("\n");
}

/*Method for copying strings*/
char *strdup(const char *orig) {
    char *copy = malloc(sizeof(char) * (strlen(orig) + 1));
    if (!copy) {
        perror("strdup mem alloc failed");
        return NULL;
    }
    strcpy(copy, orig);
    return copy;
}

/**
 * play a move for this player. please see the assignment specification for the
 * details of this.
 **/
enum move_result player_turn(struct player *theplayer) {
    char *word[NAMELEN + EXTRACHARS];
    char *orientation[1 + EXTRACHARS];
    enum orientation orientation_value;
    struct coord xycoord;
    BOOLEAN move_valid = FALSE;
    int row_number, column_number;
    enum input_result result = IR_FAILURE;

    printf("\nPlease enter a new word:");

    while (result != IR_SUCCESS) {
        result = read_string_keyboard(word);
        switch (result) {
            case IR_EMPTYLINE:
                return MOVE_SKIP;
            case IR_EOF:
                return MOVE_QUIT;
            case IR_SUCCESS:
                break;
            case IR_FAILURE:
                break;
        }
    }

    printf("\nPlease enter the row number for this word:");
    result = IR_FAILURE;
    while (result != IR_SUCCESS) {
        result = read_int_keyboard(&row_number);
        if (result == IR_SUCCESS) {
            if (row_number <= theplayer->curgame->theboard->height &&
                row_number > 0) {
                xycoord.x = row_number;
                break;
            } else {
                result = IR_FAILURE;
                printf("\nRow outside of gameboard, try again:");
            }
        }
    }

    printf("\nPlease enter the column number for this word:");
    result = IR_FAILURE;
    while (result != IR_SUCCESS) {
        result = read_int_keyboard(&column_number);
        if (result == IR_SUCCESS) {
            if (column_number <= theplayer->curgame->theboard->width &&
                column_number > 0) {
                xycoord.y = column_number;
                break;
            } else {
                result = IR_FAILURE;
                printf("\nColumn outside of gameboard, try again:");
            }
        }
    }

    printf(
        "\nPlease enter the orientation for the word, h for horizontal,"
        " v for vertical:");

    result = IR_FAILURE;
    while (result != IR_SUCCESS) {
        result = read_string_keyboard(orientation);
        if (result == IR_EOF) {
            return MOVE_QUIT;
        }
        if (*orientation[0] == 'v') {
            orientation_value = VERT;
        }
        if (*orientation[0] == 'h') {
            orientation_value = HORIZ;
        }
    }

    /*Pass data to rules-b.c validate move*/
    move_valid = validate_move(theplayer, *word, &xycoord, orientation_value);
    if (!move_valid) {
        player_turn(theplayer);
    }
    if (move_valid) {
        free(*word);
        return MOVE_SUCCESS;
    }
    return MOVE_SUCCESS;
}
/* IO Function for reading strings from stdin*/
enum input_result read_string_keyboard(char *word[]) {
    char input[NAMELEN + EXTRACHARS];

    if (fgets(input, NAMELEN + EXTRACHARS, stdin) == NULL) {
        printf("\nEOF detected.\n");
        return IR_EOF;
    }

    if (input[strlen(input) - 1] != '\n') {
        printf("\nBuffer overflow. Please try again:");
        clear_buffer();
        return IR_FAILURE;
    }
    if (input[0] == '\n') {
        printf("\nEnter pressed on emptyline.\n");
        return IR_EMPTYLINE;
    }
    input[strlen(input) - 1] = '\0';

    *word = strdup(input);
    return IR_SUCCESS;
}
/* IO Function for reading integers from stdin*/
enum input_result read_int_keyboard(int *number) {
    char *strtol_ptr;
    char input[NAMELEN + EXTRACHARS];
    long input_result;

    if (fgets(input, NAMELEN + EXTRACHARS, stdin) == NULL) {
        printf("\nEOF Detected");
        return IR_EOF;
    }
    if (input[strlen(input) - 1] != '\n') {
        printf("\nBuffer overflow. Please try again:");
        clear_buffer();
        return IR_FAILURE;
    }
    if (input[0] == '\n') {
        printf("Enter pressed on emptyline.\n");
        return IR_EOF;
    }
    input[strlen(input) - 1] = '\0';

    /*strtol to convert read in value to integer*/
    input_result = strtol(input, &strtol_ptr, 10);

    /*check that it converted*/
    if (*strtol_ptr != '\0') {
        printf("Input is not a valid number");
        return IR_FAILURE;
    }
    /*Check result is in int range*/
    if (input_result > INT_MAX || input_result < INT_MIN) {
        printf("Number is outside the valid integer range\n");
        return IR_FAILURE;
    }

    *number = input_result;
    return IR_SUCCESS;
}
