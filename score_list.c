#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "score_list.h"
#include "shared.h"
#include "limits.h"
#include "player.h"

/**
 * returns a malloced list of the scores loaded in from disk
 **/
struct score_list *load_scores(const char *filename) {
    FILE *fp;
    char line[MAXWIDTH + EXTRASPACES];
    BOOLEAN validation = TRUE;
    struct score_list *letters;
    int *array_pointer = NULL;
    array_pointer = init_validation_list();

    /*Allocate memory for score list*/
    letters = calloc(1, sizeof(struct score_list));
    if (letters == NULL) {
        perror("Score List memory allocation failed.");
        free(letters);
        validation = FALSE;
    }
    letters->total_count = 0;


    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("'nError opening score file");
        letters = NULL;
        return letters;
    }

    while (fgets(line, MAXWIDTH + EXTRASPACES, fp) != NULL) {

        int temp = 0;
        char linecopy[MAXWIDTH + EXTRASPACES];
        char *token;
        int current_token = 0;
        struct score score_line;

        /*Remove the newline from scorelist line read in*/
        line[strlen(line) - 1] = 0;

        /*Line copied for error message if one occurs*/
        memcpy(linecopy, line, MAXWIDTH + EXTRASPACES);

        /*Begin tokenisation*/

        token = strtok(line, ",");
        while (token && validation) {
            switch (current_token) {
                case 0:
                    /*Hold result in temp variables for validation*/
                    temp = char_to_int_letter(token, array_pointer);
                    if (temp < 0) {
                        validation = FALSE;
                        break;
                    }
                    /*Passed validation, now assign to struct*/
                    score_line.letter = temp;
                    break;
                case 1:
                    temp = char_to_int(token);
                    if (temp < 0) {
                        validation = FALSE;
                        break;
                    }
                    score_line.score = temp;
                    break;
                case 2:
                    temp = char_to_int(token);
                    if (temp < 0) {
                        validation = FALSE;
                        break;
                    }
                    score_line.count = temp;
                    break;
                default:
                    /*To many tokens (issue with score file formating)*/
                    fprintf(stderr, "Problem with line in score file: %s\n",
                            linecopy);
                    validation = FALSE;
                    break;
            }

            /*Continue with next token*/
            token = strtok(NULL, ",");
            ++current_token;
        }

        /*Insert the score struct into the score_list struct */

        if (!scorelist_add_item(letters, score_line)) {
            validation = FALSE;
        }
    }

    if (!validation) {
        printf("\nQuiting due to an error reading in score list...\n");
        free(letters);
        fclose(fp);
        letters = NULL;
        return letters;
    }
    fclose(fp);
    return letters;
}

/*Char to integer function to convert values read in for scorelist*/

int char_to_int(char *string) {
    int result = 0;
    char *endptr;
    result = strtol(string, &endptr, 10);
    /*if conversion not successfull return -1*/
    if (*endptr != '\0') {
        return -1;
    }
    /*Check valid int range*/
    if (result > INT_MAX || result < INT_MIN) {
        return -1;
    }
    return result;
}

/* This function converts letter to integer, checks that it is in ragne
 * and that it is the first occurance */

int char_to_int_letter(char *string, int *array_pointer) {
    int letter = 0;
    int count = 0;
    letter = *string;
    /*Check letter is in valid range defined in score_list.h*/
    if (letter < START_LETTER_VALUE || letter > END_LETTER_VALUE) {
        return -1;
    }

    /*Check letter has not allready been read in*/
    while (count < NUM_SCORES) {
        if (array_pointer[count] == letter) {
            array_pointer[count] = 0;
            return letter;
        }
        count++;
    }
    printf("\nError: Duplicate letter detected: '%s'", string);
    return -1;
}

/* This function adds a score struct (letter) to the score_list */

BOOLEAN scorelist_add_item(struct score_list *letters, struct score item) {

    /*Check not at maxiumum letters*/
    if (letters->num_scores == NUM_SCORES) {
        return FALSE;
    }
    /*Insert letter at end of letters list (score_list)*/
    letters->scores[letters->num_scores++] = item;
    /*Update letter (score) count*/
    letters->total_count += item.count;
    return TRUE;
}

/* Simple array that holds all values for valid alphabet. used in validation
 * later on*/

int *init_validation_list() {
    static int letter_array[NUM_SCORES];
    int letter = START_LETTER_VALUE;
    int count = 0;

    while (letter <= END_LETTER_VALUE) {
        letter_array[count] = letter;
        letter++;
        count++;
    }
    return letter_array;
}

/**
 * deal letters from the score list in random order, so that the player hand has
 *five letters at the end.
 **/
void deal_letters(struct score_list *score_list,
                  struct score_list *player_hand) {

    int letter_index;
    int letter;
    int score;

    /*Loop through until hand full*/
    while (player_hand->total_count < HAND_SIZE &&
           score_list->total_count > 0) {

        /*Get random letter index*/
        letter_index = random_letter_index(score_list);
        letter = score_list->scores[letter_index].letter;
        score = score_list->scores[letter_index].score;

        /*add to player hand and increase count*/
        player_hand->scores[player_hand->total_count].letter = letter;
        player_hand->scores[player_hand->total_count].score = score;
        player_hand->total_count++;
        /*Decrease the letters count in scorelist*/
        score_list->scores[letter_index].count--;
        score_list->total_count--;
    }
}

/*Get a random letter, if its count is 0 try for another one*/
int random_letter_index(struct score_list *score_list) {
    BOOLEAN letter_available = FALSE;
    int letter;
    while (!letter_available) {
        letter = rand() % (NUM_SCORES) + 0;
        if (score_list->scores[letter].count > 0) {
            letter_available = TRUE;
        }
    }
    return letter;
}
