#include "game.h"
#include "player.h"

#include "score_list.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * initialise the game
 **/
BOOLEAN game_init(struct game *thegame) {
    int board_width, board_height, coin;
    struct board *the_board;
    BOOLEAN board_size_valid = FALSE;
    /*get and intialise players*/
    if (!add_players()) {
        return FALSE;
    }

    /*get board size*/
    while (!board_size_valid) {
        printf("\nPlease enter board width:");
        if (read_board_size(&board_width)) {
            printf("\nPlease enter board height:");
            if (read_board_size(&board_height)) {
                board_size_valid = TRUE;
            }
        }
    }
    /*Init board*/
    the_board = new_board(board_width, board_height);
    thegame->theboard = the_board;

    /*"Flip coin" to decide who starts*/
    coin = rand() % (2) + 0;
    thegame->curr_player_num = coin;

    return TRUE;
}

/**
 * Initilise the game and manage players turns
 **/
void play_game(const char *scoresfile) {
    BOOLEAN quit = FALSE;
    BOOLEAN init = TRUE;
    struct game thegame;
    struct score_list *letters;
    int i;

    /*Load in letters*/
    letters = load_scores(scoresfile);
    if (letters == NULL) {
        quit = TRUE;
        init = FALSE;
    }
    thegame.score_list = letters;

    /* CALL GAME_INIT*/
    if (!quit) {
        if (!game_init(&thegame)) {
            printf("Error initialising game\n");
            quit = TRUE;
            init = FALSE;
        }
    }

    if (init) {
        printf("\nWelcome to Wuzzle.\n");
    }

    /* Game Loop*/
    while (!quit && thegame.score_list->total_count > 0) {
        print_board(thegame.theboard);

        /*Print current players hand*/
        print_hand(thegame.players[thegame.curr_player_num]);
        /*Get players input*/
        switch (player_turn(&thegame.players[thegame.curr_player_num])) {
            case MOVE_QUIT:
                printf("\nPlayer with the highest score wins\n");
                for (i = 0; i < MAX_PLAYERS; ++i) {
                    printf("%s's Score: %d\n", thegame.players[i].name,
                           thegame.players[i].score);
                }
                quit = TRUE;
                break;
            case MOVE_SKIP:
                break;
            case MOVE_SUCCESS:
                printf(
                    "\nWell done, that move was successful! %s's new score"
                    " is %d",
                    thegame.players[thegame.curr_player_num].name,
                    thegame.players[thegame.curr_player_num].score);
                break;
            case MOVE_BOARD_FULL:
                break;
        }

        /*Loop turns*/
        if (thegame.curr_player_num + 1 < MAX_PLAYERS) {
            thegame.curr_player_num++;
        } else {
            thegame.curr_player_num = 0;
        }
    }

    if (init) {
        free_memory(&thegame);
    }
}

void free_memory(struct game *thegame) {
    int height_count, i, height;

    height = thegame->theboard->height;
    /*Free score list*/
    free(thegame->score_list);
    /*free players hand*/
    for (i = 0; i < MAX_PLAYERS; i++) {
        free(thegame->players[i].hand);
        /* free(thegame->players[i].name); */
    }

    for (height_count = 0; height_count < height; ++height_count) {
        free(thegame->theboard->matrix[height_count]);
    }
}
