/******************************************************************************
 * Student Name    :    Jack Edwards
 * RMIT Student ID :    S3727853
 *
 * Startup code provided by Paul Miller for use in "Programming in C",
 * Assignment 2, study period 2, 2020.
 *****************************************************************************/
How do we use your program? 
---------------------------

Once the program is compiled it should be started via its compiled name passing
in a comma seperated file containing a list of letters for the game to load
and utilise. (as per assignment specs).

The initial prompts require the players names and the board size. The minimum 
size is 5, and the max size is defined in board.h (50).

A player is chosen to random to start the game. To skip a turn press enter on
an emptyline. To exit press CTRL-D (EOF).

The initial word can be placed anywere on the board as long as it fits and the
player has all of the letters for it in their hand.

Subsequent turns require that the first letter of the word they wish to place 
to already exist on the board. (it is therfore not required to be in their hand)

Words can only flow horizontall to the left of existing letters or down
vertically from existing letters.

NOTE: due to assignment time constraints the game only validates the first
letter is on the board and will overwrite letters in its path. It is upto the 
player at this stage to ensure their word does not intersect any letters other
than the first letter.



Explain your reasoning behind the modifications you have made to the design of 
the program
------------------------------------------------------------------------------

The game struct in game.h has had the letter_list element changed to score_list
as I belive that was what was intended and letter_list is a typo.

I have added in an extra field to the cell struct in board.h. This new field is 
an integer that holds the cells letter score. This has been done to
simplify a players score calculation, as every cell that a player owns now
has its point value associated with it.

