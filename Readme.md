# Scrabble Like Game in C

This is a two player text based (command line) scrabble like game. The focus of the project is on proper memory management and adhering to the ANSI C90 standards.

Please note this is more a technical excercise than a fully featured game. See game rules for more info.

Two files are to be passed into the game on startup:
    
The word scores. This is essentially a text file equivalent to all of the scrabble tiles for the game, containing the number of each letter to be present in the game and the amount of points each one is worth.

Dictionary file, all players words are checked against this file which is loaded into the game as a linked list. If the players word does not exist in the dictionary then the word is deemed invalid and the player will need to try a different word.

## Game Rules

The initial prompts require the players names and the board size. The minimum 
size is 5, and the max size is defined in board.h (50).

A player is chosen at random to start the game. To skip a turn press enter on
an emptyline. To exit press CTRL-D (EOF).

The initial word can be placed anywhere on the board as long as it fits and the
player has all of the letters for it in their hand.

Subsequent turns require that the first letter of the word they wish to place 
already exist on the board. (it is therefore not required to be in their hand)

Words can only flow horizontally to the left of existing letters or down
vertically from existing letters. 

NOTE: due to assignment time constraints the game only validates the first
letter is on the board and will overwrite letters in its path. It is upto the 
player at this stage to ensure their word does not intersect any letters other
than the first letter.

## Compiling Manually

To compile the game use: `gcc -ansi -Wall -pedantic -g board.c game.c player.c io.c rules-b.c score_list.c word_list.c wuzzle.c commands.c -o c-scrabble`

Note the following arguments can be omitted:

-Wall  (Show all warnings)
-pedantic (Issue warnings for strict ISO C standards)
-g (Debugging symbols)

## Using the makefile

To compile the game automatically type in `make` in the projects root. This builds the executable with the name c-scrabble and runs the same build command as above complete with debuging symbols.

To clean the directory and remove all object files and the executable type `make clean`

Note: You must have the make build automation utility installed to use the make command.

## Checking for memory leaks with Valgrind

`valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./c-scrabble word_scores words5_100`

## Running the game

Once the game is compiled to run `./c-scrabble word_scores word_list` where word_scores and words5_100 are the two files passed into the game. This could be a path to the files if they are in a different folder.
