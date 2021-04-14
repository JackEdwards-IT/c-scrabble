#include "board.h"
#include "player.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*clear buffer method*/
static void clear_buffer(void) {
    int ch;
    while (ch = getc(stdin), ch != EOF && ch != '\n')
        ;
    clearerr(stdin);
}

/*Read in int for board size and validate*/
BOOLEAN read_board_size(int *ptr_boardsize) {
    char *strtol_ptr;
    char input[INPUT_CHARS + EXTRA_CHARS];
    long input_result;

    if (fgets(input, INPUT_CHARS + EXTRA_CHARS, stdin) == NULL) {
        printf("\nEOF detected\n");
        return FALSE;
    }
    if (input[strlen(input) - 1] != '\n') {
        printf("\nBuffer overflow\n");
        clear_buffer();
        return FALSE;
    }
    if (input[0] == '\n') {
        printf("\nEnter pressed on empytline\n");
        return FALSE;
    }
    input[strlen(input) - 1] = '\0';
    /*Convert to long*/
    input_result = strtol(input, &strtol_ptr, 10);

    if (*strtol_ptr != '\0') {
        printf("\nInput is not a valid number\n");
        return FALSE;
    }
    /*Check number is range of board min max*/
    if (input_result < BOARD_MIN || input_result > BOARD_MAX) {
        printf("\nInput outside of board range. Min: %d Max: %d", BOARD_MIN,
               BOARD_MAX);
        return FALSE;
    }
    *ptr_boardsize = input_result;
    return TRUE;
}

/*Initilise the board*/

struct board *new_board(int width, int height) {
    int height_count, width_count;

    struct board *the_board = malloc(sizeof(struct board));
    /*set all elements to 0*/
    memset(the_board, 0, sizeof(struct board));

    /*TODO malloc returns need to be checked */
    /*Malloc once for height*/
    the_board->matrix = malloc(height * sizeof(struct cell *));

    /*and then malloc each row*/
    for (height_count = 0; height_count < height; ++height_count) {

        the_board->matrix[height_count] = malloc(width * sizeof(struct cell));

        /*set each cell to a null/eof value*/
        for (width_count = 0; width_count < width; ++width_count) {
            the_board->matrix[height_count][width_count].letter = 0;
            the_board->matrix[height_count][width_count].owner = NULL;
        }
    }

    the_board->width = width;
    the_board->height = height;

    return the_board;
}

/* Print the board */
void print_board(struct board *the_board) {
    unsigned wcount, hcount;
    /*Print top legend*/
    printf("\n\t|    1  ");
    for (wcount = 1; wcount < the_board->width; ++wcount) {
        printf("|   %d\t", wcount + 1);
    }
    printf("|\n--------");
    /*Print first row seperator ---*/
    for (wcount = 0; wcount < the_board->width; ++wcount) {
        printf("--------");
    }

    /*Print each cell*/
    for (hcount = 0; hcount < the_board->height; ++hcount) {
        printf("\n%4d\t|", hcount + 1);

        for (wcount = 0; wcount < the_board->width; ++wcount) {
            /*set defualt print color*/
            int color_num = 6;
            /*Check if cell has a color set*/
            if (the_board->matrix[hcount][wcount].owner != NULL) {
                color_num = the_board->matrix[hcount][wcount].owner->color;
            }

            printf("%s%4c%s\t|", color_strings[color_num],
                   the_board->matrix[hcount][wcount].letter,
                   color_strings[COL_RESET]);
        }
        /*Print line row seprators -----*/
        printf("\n--------");
        for (wcount = 0; wcount < the_board->width; ++wcount) {
            printf("--------");
        }
    }
    printf("\n");
}
