# Scrabble Game in C

This is a two player text based (command line) scrabble game. The focus of the project is on proper memory managment and adhering to the ANSI C90 standards.

Two files are to be passed into the game on startup:
    
    - The word scores. This is essentially a text file equivilant of all of the scrabble tiles   containg the amount of each letters to be present in the game and the amount of points each one is worth.

    - Dictionary file, all players words are checked against this file which is loaded into the game as a linked list. If the players word does not exist in the dictionary then the word is demmed invalid and the player will need to try a differn't word.

## Compiling manually

To compile the game use: `gcc -ansi -Wall -pedantic -g board.c game.c player.c rules-b.c score_list.c wuzzle.c -o c-scrabble`

Note the following arguments can be ommitted:

-Wall  (Show all warnings)
-pedantic (Issue warnings for strict ISO C standards)
-g (Debugging symbols)

## Useing the makefile



## Checking for memory leaks with Valgrind

`valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./c-scrabble word_scores word_list`

## Running the game

Once the game is compiled to run `./c-scrabble word_scores word_list` where word_scores and word_list are the two files passed into the game. This could be a path to the files if they are in a differn't folder.
