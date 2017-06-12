all:	frog
frog:	globals.h console.c console.h frogger.c frogger.h player.c player.h log.c log.h llist.c llist.h
	gcc -Wall -lcurses -lpthread frogger.c console.c player.c log.c llist.c -o frogger
clean:
	rm -f frogger
