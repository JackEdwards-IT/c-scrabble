#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "score_list.h"
#include "word_list.h"
#include "io.h"
#include "game.h"

/* Process a command that a user has typed in */
void command(char* user_input, struct game* thegame){
    char user_word[WORD_LENGTH + EXTRA_CHARS];
    enum commands command_result;
    command_result = process_command(user_input, user_word);
     switch (command_result) {
                case CI_HELP:
                    print_help();
                    free(user_input);
                    break;
                case CI_ADD:
                    if (list_add(thegame->dictionary_words, user_word)) {
                        printf("\nWord added to dictionary.\n");
                    }
                    free(user_input);
                    break;
                case CI_CHECK:
                    to_upper(user_word);
                    if(is_in_dictionary(thegame->dictionary_words, user_word)){
                        printf("\nThe word '%s' is in the dictionary.\n", user_word);
                    }
                    break;
                case CI_DELETE:
                    if (list_delete(thegame->dictionary_words, user_word)) {
                        printf("\n'%s' was deleted from the dictionary.\n",
                               user_word);
                    }
                    free(user_input);
                    break;
                case CI_SAVE:
                    if (save_out(thegame->dictionary_words, user_word)) {
                        printf("\nDictionary saved to file '%s'\n", user_word);
                    }
                    free(user_input);
                    break;
                case CI_ERROR:
                    printf(
                        "\nError, please check that the word you entered "
                        "contains valid characters only (letters)\n");
                    break;
            }
}

/*This function validates commands, if a command is passed in with a second
 * word (delete or add) this word is copied into the return_words location
 * for caller to pass to required word list function*/

enum commands process_command(char *command, char *return_word) {
    char *token;
    char command_word[WORD_LENGTH + EXTRA_CHARS];
    char action_word[WORD_LENGTH + EXTRA_CHARS];
    BOOLEAN action_word_present = FALSE;
    int i = 0;

    /*Get the first part of the string read in (the command) and convert to
     * uppercase. Strtok is used to split on space, the first half is ignored
     * (the command) and the second half is saved*/

    token = strtok(command, " ");

    while (token != NULL) {
        if (i == 0) {
            strcpy(command_word, token);
        }
        if (i == 1) {
            strcpy(action_word, token);
            action_word_present = TRUE;
        }
        token = strtok(NULL, " ");
        i++;
    }

    /*Check command has valid letters*/
    if (!is_alpha(command_word)) {
        printf("\nError: command is invalid.");
        return CI_ERROR;
    }

    /*Check if command is any of the single word commands*/
    to_upper(command_word);
    if (strcmp(command_word, "HELP") == 0) {
        return CI_HELP;
    }

    /*If save command we don't wanto validate save name as it may be a
     * complete file path*/
    if (strcmp(command_word, "SAVE") == 0) {
        strcpy(return_word, action_word);
        return CI_SAVE;
    }

    /*check second part of command is present and letters valid*/
    if (!action_word_present || !is_alpha(action_word)) {
        return CI_ERROR;
    }

    to_upper(action_word);

    /*Check word against commands TODO don't hardcode this */

    if (strcmp(command_word, "ADD") == 0) {
        strcpy(return_word, action_word);
        return CI_ADD;
    }   
    if (strcmp(command_word, "CHECK") == 0){
        strcpy(return_word, action_word);
        return CI_CHECK;
    }
    if (strcmp(command_word, "DELETE") == 0) {
        strcpy(return_word, action_word);
        return CI_DELETE;
    }

    printf("\nError: unrecognised command.\n");
    return CI_ERROR;
}

void print_help(void) {
    printf(
        "\n------------------ Game Help -------------------------"
        "\nTo play a word simply type the word and press enter. "
        "\nTo exit press enter on an emptyline or CTRL-D"
        "\n---------------------- Commands -----------------------------\n"
        "Please note 'word' is your input after the command.\n"
        "To check if a word is in the dictionary type :check word\n"
        "To add word to dictionary type :add word\n"
        "To delete a word from dictionary type :delete word\n"
        "To save the dictionary type :save filename\n"
        "To access this help info again type :help\n:");
}
