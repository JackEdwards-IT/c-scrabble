#include "shared.h"

#define MAX_STRING_LENGTH 80
#define EXTRA_CHARS 2
/* the maximum length of a player's name */
#define NAMELEN 40

enum input_result {
    IR_FAILURE,
    IR_EMPTYLINE,
    IR_SUCCESS,
    IR_COMMAND,
    IR_EOF = -1
};

enum input_result read_string_keyboard(char *string[]);
enum input_result read_int_keyboard(int *ptr);
char *strdup(const char *);
void to_upper(char *word);
BOOLEAN is_alpha(char *word);
