#include "shared.h"
#include "game.h"

/*Command line options*/
enum commands {
    CI_HELP,
    CI_ADD,
    CI_DELETE,
    CI_SAVE,
    CI_CHECK,
    CI_ERROR
};

void command(char* user_input, struct game* thegame);
enum commands process_command(char* command, char* return_word);
void print_help(void);