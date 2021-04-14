#include "rules-b.h"
#include "game.h"
#include "score_list.h"
#include "shared.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

BOOLEAN validate_move(struct player* theplayer, const char* word,
                      const struct coord* coords, enum orientation orient) {

    int i, word_length;
    char uppercase_word[NAMELEN + EXTRACHARS];
    BOOLEAN first_move = TRUE;
    /*set these for ease of use*/
    int board_height = theplayer->curgame->theboard->height;
    int board_width = theplayer->curgame->theboard->width;
    /*Check if first move*/
    for (i = 0; i < MAX_PLAYERS; ++i) {
        if (theplayer->curgame->players[i].score > 0) {
            first_move = FALSE;
        }
    }

    /* Get the length of the word */
    word_length = strlen(word);

    /*Check word will fit on board*/
    switch (orient) {
        case VERT:
            if (coords->x + (word_length - 1) > board_height) {
                printf("\nWord does not fit");
                return FALSE;
            }
            break;

        case HORIZ:
            if (coords->y + (word_length - 1) > board_width) {
                printf("\nWord does not fit");
                return FALSE;
            }
            break;
    }

    /*convert players word to uppercase if not allready*/
    for (i = 0; i < word_length; ++i) {
        uppercase_word[i] = toupper(word[i]);
    }
    uppercase_word[word_length] = '\0';

    /*Word placement if first word of the game*/
    if (first_move) {
        if (!play_first_move(theplayer, uppercase_word, coords, orient,
                             word_length)) {
            return FALSE;
        } else {
            return TRUE;
        }
    }

    /*Word placement if its not the first move*/
    if (!first_move) {
        if (!play_normal_move(theplayer, uppercase_word, coords, orient,
                              word_length)) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
    return FALSE;
}

/* This function handles the play of the first word on the board*/

BOOLEAN play_first_move(struct player* theplayer, const char* uppercase_word,
                        const struct coord* coords, enum orientation orient,
                        int word_length) {
    int hand_index = 0;
    int word_index = 0;
    int match_count = 0;
    char temp_word[NAMELEN + EXTRACHARS];
    int word_scores[NAMELEN];

    /*Make a copy of the word, check that all letters are in players hand*/
    strcpy(temp_word, uppercase_word);

    /*Iterate througth the players hand and compare to word*/
    for (hand_index = 0; hand_index < HAND_SIZE; ++hand_index) {
        for (word_index = 0; word_index < word_length; ++word_index) {

            if (theplayer->hand->scores[hand_index].letter ==
                (int)temp_word[word_index]) {
                match_count++;
                /* remove element so it doesn't get counted twice*/
                temp_word[word_index] = 0;
                break;
            }
        }
    }
    /*Ensure player hand contains ALL the words letters for the first move*/
    if (match_count != word_length) {
        printf("\nWords letters do not match what is in current hand");
        return FALSE;
    }

    /*### Move is valid proceed with logic to place word ###*/

    /* Remove the letters from players hand*/
    strcpy(temp_word, uppercase_word);
    /*Iterate througth the players hand and compare to word*/
    for (hand_index = 0; hand_index < HAND_SIZE; ++hand_index) {
        for (word_index = 0; word_index < word_length; ++word_index) {
            if (theplayer->hand->scores[hand_index].letter ==
                (int)temp_word[word_index]) {
                /*set letter in hand to null*/
                theplayer->hand->scores[hand_index].letter = 0;
                /*Temp store for the letters score*/
                word_scores[word_index] =
                    theplayer->hand->scores[hand_index].score;
                /* reomove element so it doesn't get counted twice*/
                temp_word[word_index] = 0;
                break;
            }
        }
    }

    /*Place the first letter of the word*/
    theplayer->curgame->theboard->matrix[coords->x - 1][coords->y - 1].letter =
        uppercase_word[0];
    /*decrease letter count*/
    theplayer->hand->total_count--;
    /*set score value in cell*/
    theplayer->curgame->theboard->matrix[coords->x - 1][coords->y - 1].score =
        word_scores[0];
    /*set owner for first letter in cell*/
    theplayer->curgame->theboard->matrix[coords->x - 1][coords->y - 1].owner =
        theplayer;
    /*place the rest of the word*/
    for (word_index = 1; word_index < word_length; ++word_index) {
        switch (orient) {
            /*Vertical placement*/
            case VERT:
                /*Place each letter on board*/
                theplayer->curgame->theboard->matrix[coords->x + word_index - 1]
                                                    [coords->y - 1].letter =
                    uppercase_word[word_index];
                /*decrease hand letter count*/
                theplayer->hand->total_count--;
                /*set letters score value*/
                theplayer->curgame->theboard->matrix[coords->x + word_index - 1]
                                                    [coords->y - 1].score =
                    word_scores[word_index];
                /*set letters owner*/
                theplayer->curgame->theboard->matrix[coords->x + word_index - 1]
                                                    [coords->y - 1].owner =
                    theplayer;
                break;

            /*Horizontal placement*/
            case HORIZ:
                /*place letter*/
                theplayer->curgame->theboard->matrix
                    [coords->x - 1][coords->y + word_index - 1].letter =
                    uppercase_word[word_index];
                /*decreae hand letter count*/
                theplayer->hand->total_count--;
                /*set letters score value*/
                theplayer->curgame->theboard->matrix
                    [coords->x - 1][coords->y + word_index - 1].score =
                    word_scores[word_index];
                /*set owner*/
                theplayer->curgame->theboard->matrix
                    [coords->x - 1][coords->y + word_index - 1].owner =
                    theplayer;
                break;
        }
    }

    /*Call calculate score*/
    calculate_score(theplayer);
    /*shift letters left in hand to front of array*/
    shift_letters(theplayer);
    /*Replentish letters in hand*/
    deal_letters(theplayer->curgame->score_list, theplayer->hand);
    return TRUE;
}

/* This function handles all moves after the first move */
BOOLEAN play_normal_move(struct player* theplayer, const char* uppercase_word,
                         const struct coord* coords, enum orientation orient,
                         int word_length) {
    int hand_index = 0;
    int word_index = 0;
    int match_count = 0;
    char temp_word[NAMELEN + EXTRACHARS];
    int word_scores[NAMELEN];

    /*Make a copy of the word, check that all letters other than the first
     * are in players hand*/
    strcpy(temp_word, uppercase_word);

    /*Iterate througth the players hand and compare to word*/
    for (hand_index = 0; hand_index < HAND_SIZE; ++hand_index) {
        for (word_index = 1; word_index < word_length; ++word_index) {

            if (theplayer->hand->scores[hand_index].letter ==
                (int)temp_word[word_index]) {
                match_count++;
                /* remove element so it doesn't get counted twice*/
                temp_word[word_index] = 0;
                break;
            }
        }
    }
    /*Ensure player hand contains the words letters
     * (other than the first letter)*/

    if (match_count != word_length - 1) {
        printf("\nWords letters do not match what is in current hand");
        return FALSE;
    }
    /*Check that the first letter of the word is currently in
     * place on the board*/
    if (theplayer->curgame->theboard->matrix[coords->x - 1][coords->y - 1]
            .letter != uppercase_word[0]) {
        printf(
            "\nThe first letter of your word does not match the letter"
            " on the board.");
        return FALSE;
    }

    /*### Move is valid proceed with logic to place word ###*/

    /*change owner of the letter allready on the board*/
    theplayer->curgame->theboard->matrix[coords->x - 1][coords->y - 1].owner =
        theplayer;

    /* Remove the letters from players hand*/
    strcpy(temp_word, uppercase_word);
    /*Iterate througth the players hand and compare to word*/
    for (hand_index = 0; hand_index < HAND_SIZE; ++hand_index) {
        for (word_index = 1; word_index < word_length; ++word_index) {
            if (theplayer->hand->scores[hand_index].letter ==
                (int)temp_word[word_index]) {
                /*set letter in hand to null*/
                theplayer->hand->scores[hand_index].letter = 0;
                /*Temp store for the letters score*/
                word_scores[word_index] =
                    theplayer->hand->scores[hand_index].score;
                /* reomove element so it doesn't get counted twice*/
                temp_word[word_index] = 0;
                break;
            }
        }
    }

    /*place the word*/
    for (word_index = 1; word_index < word_length; ++word_index) {
        switch (orient) {
            /*Vertical placement*/
            case VERT:
                /*Place each letter on board*/
                theplayer->curgame->theboard->matrix[coords->x + word_index - 1]
                                                    [coords->y - 1].letter =
                    uppercase_word[word_index];
                /*decrease hand letter count*/
                theplayer->hand->total_count--;
                /*set letters score value*/
                theplayer->curgame->theboard->matrix[coords->x + word_index - 1]
                                                    [coords->y - 1].score =
                    word_scores[word_index];
                /*set letters owner*/
                theplayer->curgame->theboard->matrix[coords->x + word_index - 1]
                                                    [coords->y - 1].owner =
                    theplayer;
                break;

            /*Horizontal placement*/
            case HORIZ:
                /*place letter*/
                theplayer->curgame->theboard->matrix
                    [coords->x - 1][coords->y + word_index - 1].letter =
                    uppercase_word[word_index];
                /*decreae hand letter count*/
                theplayer->hand->total_count--;
                /*set letters score value*/
                theplayer->curgame->theboard->matrix
                    [coords->x - 1][coords->y + word_index - 1].score =
                    word_scores[word_index];
                /*set owner*/
                theplayer->curgame->theboard->matrix
                    [coords->x - 1][coords->y + word_index - 1].owner =
                    theplayer;
                break;
        }
    }

    /*Call calculate score*/
    calculate_score(theplayer);
    /*shift letters left in hand to front of array*/
    shift_letters(theplayer);

    /*Testing*/
    /* print_hand(*theplayer);*/

    /*Replentish letters in hand*/
    deal_letters(theplayer->curgame->score_list, theplayer->hand);

    /*Testing*/
    /* print_hand(*theplayer);*/
    return TRUE;
}

int calculate_score(struct player* theplayer) {

    /*Iterate over entire gameboard, add letters value for cells owned by the
     * player*/
    int score = 0;
    unsigned wcount, hcount;
    struct board* the_board = theplayer->curgame->theboard;

    for (hcount = 0; hcount < the_board->height; ++hcount) {

        for (wcount = 0; wcount < the_board->width; ++wcount) {
            /*Check if cell has a letter set for the player*/
            if (the_board->matrix[hcount][wcount].owner == theplayer) {
                score += the_board->matrix[hcount][wcount].score;
            }
        }
    }
    printf("Score: %d", score);
    theplayer->score = score;
    return score;
}

/*Shift remaing letters in hand to the left ready for new letters to be dealt
 * in*/
void shift_letters(struct player* theplayer) {
    int i;
    int temp_letter[HAND_SIZE];
    int temp_score[HAND_SIZE];
    int array_count = 0;

    /*Initilise temp arrays*/
    for (i = 0; i < HAND_SIZE; ++i) {
        temp_letter[i] = 0;
        temp_score[i] = 0;
    }
    /*Loop through hand and store letters in temp arrays*/

    for (i = 0; i < HAND_SIZE; ++i) {
        if (theplayer->hand->scores[i].letter != 0) {
            temp_letter[array_count] = theplayer->hand->scores[i].letter;
            temp_score[array_count] = theplayer->hand->scores[i].score;
            array_count++;
        }
    }
    /*Place the letters stored back into the hand starting*/
    for (i = 0; i < HAND_SIZE; ++i) {
        theplayer->hand->scores[i].letter = temp_letter[i];
        theplayer->hand->scores[i].score = temp_score[i];
    }
}
