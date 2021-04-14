/******************************************************************************
 * Student Name    :    Jack Edwards
 * RMIT Student ID :    S3727853
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * Assignment 2, study period 4, 2020.
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"

int main(int argc, char* argv[]) {
    /*Seed rand for the app*/
    srand(time(NULL));

    if (argc != NUMARGS) {
        fprintf(stderr, "Error: Incorect number of files passed in.\n\n");
        return EXIT_FAILURE;
    }

    play_game(argv[1]);
    /* check the command line args and then pass the appropriate arg to
     * play_game()
     **/
    return EXIT_SUCCESS;
}
