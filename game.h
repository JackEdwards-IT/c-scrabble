#include "board.h"
#include "player.h"
#ifndef GAME_H
#define GAME_H
#define MAX_PLAYERS 2
#define NUMARGS 2

struct game {
    struct player players[MAX_PLAYERS];
    int curr_player_num;
    struct board* theboard;
    struct score_list* score_list;
    /*I believe the below is a typo*/
    /*struct letter_list* score_list;*/
};

void free_memory(struct game* thegame);
BOOLEAN game_init(struct game*);
void play_game(const char*);
#endif
