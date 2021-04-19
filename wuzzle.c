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

    play_game(argv[1], argv[2]);
    /* check the command line args and then pass to game*/
    return EXIT_SUCCESS;
}
