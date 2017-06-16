all:	frog

frog:	globals.h console.c console.h frogger.c frogger.h wrappers.c wrappers.h player.c player.h log.c log.h llist.c llist.h
	gcc -Wall -lcurses -lpthread frogger.c wrappers.c console.c player.c log.c llist.c -o frogger

debug:	globals.h console.c console.h frogger.c frogger.h wrappers.c wrappers.h player.c player.h log.c log.h llist.c llist.h
	gcc -g -Wall -lcurses -lpthread frogger.c wrappers.c console.c player.c log.c llist.c -o frogger

clean:
	rm -f *.o frogger
