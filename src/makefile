all:	frog
frog:	console.c console.h frogger.c frogger.h player.c player.h globals.h
	gcc -Wall -lcurses -lpthread frogger.c console.c player.c -o frogger
clean:
	rm -f frogger *.o
