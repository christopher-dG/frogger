#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <pthread.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#include "console.h"
#include "frogger.h"
#include "globals.h"


static char *PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};

struct player {
  pthread_t tid;  // Thread id.
  pthread_t blinker;  // Thread for making the frog blink.
  pthread_t keyboard;  // Thread for handling user input to the frog.
  int x;  // x position (top left corner).
  int y;  // y position (top left corner).
  int blinking;  // Are the frogs eyes closed?
  int on_log;  // Is the frog on a log?
};

void *init_player(void *args);
void *blink(void *args);
void *input (void *args);
void draw(struct player *frog);
void move(struct player *frog, int y, int x);
int valid_move(int y, int x, int height, int width);

#endif
