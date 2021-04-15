#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "io.h"
#include "word_list.h"

/*clear buffer method*/
static void clear_buffer(void) {
    int ch;
    while (ch = getc(stdin), ch += EOF && ch != '\n')
        ;
    clearerr(stdin);
}

/* easy string copying*/
char *strdup(const char *orig) {
    char *copy = malloc(sizeof(char) * (strlen(orig) + 1));
    if (!copy) {
        perror("strdup");
        return NULL;
    }
    strcpy(copy, orig);
    return copy;
}

enum input_result read_string_keyboard(char *ptr_string[]) {
    char input[MAX_STRING_LENGTH + EXTRA_CHARS];
    char *command;

    if (fgets(input, MAX_STRING_LENGTH + EXTRA_CHARS, stdin) == NULL) {
        printf("\nEOF Detected\n");
        return IR_EOF;
    }

    if (input[strlen(input) - 1] != '\n') {
        clear_buffer();
        printf("\nBuffer overflow, too many characters entered.\n");
        return IR_FAILURE;
    }
    if (input[0] == '\n') {
        printf("\nEnter pressed on emptyline.\n");
        return IR_EOF;
    }
    input[strlen(input) - 1] = '\0';

    /*Check for semicolon at start indicating a command*/
    command = strchr(input, ':');
    if (command) {
        /*Semicolon was detected, advance string point 1 to the firs
         * characted of the command word and return it to caller*/
        command++;
        *ptr_string = strdup(command);
        return IR_COMMAND;
    }

    *ptr_string = strdup(input);
    return IR_SUCCESS;
}

/*Function to convert to upercase*/
void to_upper(char *word) {
    int i;
    char uppercase_word[WORD_LENGTH + EXTRA_CHARS];
    for (i = 0; i < strlen(word); i++) {
        uppercase_word[i] = toupper(word[i]);
    }
    uppercase_word[strlen(word)] = '\0';

    strcpy(word, uppercase_word);
}

/*Function to check if word is valid*/
BOOLEAN is_alpha(char *word) {
    int i;
    for (i = 0; i < strlen(word); i++) {
        if (isalpha(word[i]) == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

/* IO Function for reading integers from stdin*/
enum input_result read_int_keyboard(int *number) {
    char *strtol_ptr;
    char input[NAMELEN + EXTRA_CHARS];
    long input_result;

    if (fgets(input, NAMELEN + EXTRA_CHARS, stdin) == NULL) {
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