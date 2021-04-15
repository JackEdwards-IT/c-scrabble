#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "score_list.h"
#include "io.h"

#include "game.h"
/* the color_strings array that defines the color codes for the printing
 * out colour in the terminal. The order of the values in this array is the
 * same as the color enum
 **/
char *color_strings[] = {"\033[0;31m", "\033[0;32m", "\033[0;33m", "\033[0;34m",
                         "\033[0;35m", "\033[0;36m", "\033[0m"};


/**
 * prompt the user for their name and deal the letters for the player to start
 * with. Also assign the game pointer so we can access it later.
 **/

BOOLEAN add_players(struct game *thegame) {
    int count = 0;
    char *name[NAMELEN + EXTRACHARS];
    BOOLEAN player_error = FALSE;

    while (count < MAX_PLAYERS && !player_error) {

        /*struct player *theplayer;*/
        struct score_list *hand;
        enum input_result result = IR_FAILURE;

        printf("\nEnter name for player %d:", count + 1);

        while (result != IR_SUCCESS) {
            result = read_string_keyboard(name);
            if (result == IR_EOF) {
                return FALSE;
            }
        }

        /*allocate memory for players hand*/
        hand = calloc(1, sizeof(struct score_list));
        thegame->players[count].hand = hand;

        /*Initialise the player*/
        if (!player_init(*name, thegame, count)) {
            player_error = TRUE;
            break;
        }
        count++;
        /*Free user input that was malloced with strdup*/
        free(*name);
    }

    if (!player_error) {
        return TRUE;
    };
    return FALSE;
}

/* Initialise the player*/
BOOLEAN player_init(const char *name, struct game *thegame, int playerNum) {
#define MYRANDMAX 5
    enum color randomcolor;
    /*set the random colour to random number from 0 to MYRANDMAX*/
    randomcolor = rand() % (MYRANDMAX + 1) + 0;
    /*Copy player details to player struct*/
    strcpy(thegame->players[playerNum].name, name);
    thegame->players[playerNum].color = randomcolor;
    /*Link player to game*/
    thegame->players[playerNum].curgame = thegame;

    /*Need to deal initial hand*/
    deal_letters(thegame->score_list, thegame->players[playerNum].hand);
    /*Set starting score*/
    thegame->players[playerNum].score = 0;

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



/**
 * play a move for this player. 
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
            case IR_COMMAND:
                /**
                 * TODO
                 * **/
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
        "\nPlease enter the orientation for the word, h for horizontall,"
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
        free(*orientation);
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

