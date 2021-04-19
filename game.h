#include "board.h"
#include "player.h"
#ifndef GAME_H
#define GAME_H
#define MAX_PLAYERS 2
#define NUMARGS 3

struct game {
    struct player players[MAX_PLAYERS];
    int curr_player_num;
    struct board* theboard;
    struct score_list* score_list;
    struct word_list* dictionary_words;
};

void free_memory(struct game* thegame);
BOOLEAN game_init(struct game*);
void play_game(const char*, const char*);
#endif
