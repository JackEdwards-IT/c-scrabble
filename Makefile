CC=gcc
CFLAGS=-ansi -Wall -pedantic -g
TARGETS=c-scrabble
LFLAGS=
HEADERS=*.h

all:$(TARGETS)

c-scrabble:io.o game.o score_list.o word_list.o board.o player.o rules-b.o wuzzle.o commands.o 
	$(CC) $(LFLAGS) io.o game.o score_list.o word_list.o board.o player.o rules-b.o wuzzle.o commands.o -o c-scrabble

io.o:io.c io.h word_list.h
	$(CC) $(CFLAGS) -c io.c

game.o:game.c game.h word_list.h io.h score_list.h player.h
	$(CC) $(CFLAGS) -c game.c

score_list.o:score_list.c score_list.h shared.h player.h
	$(CC) $(CFLAGS) -c score_list.c

word_list.o:word_list.c word_list.h score_list.h shared.h
	$(CC) $(CFLAGS) -c word_list.c

board.o:board.c board.h player.h
	$(CC) $(CFLAGS) -c board.c

player.o:player.c player.h commands.h score_list.h word_list.h io.h game.h 
	$(CC) $(CFLAGS) -c player.c

rules-b.o:rules-b.c rules-b.h game.h score_list.h shared.h 
	$(CC) $(CFLAGS) -c rules-b.c

wuzzle.o:wuzzle.c game.h 
	$(CC) $(CFLAGS) -c wuzzle.c

commands.o:commands.c commands.h score_list.h word_list.h io.h game.h 
	$(CC) $(CFLAGS) -c commands.c

.PHONY:clean
clean:
	rm -f *.o c-scrabble *~
