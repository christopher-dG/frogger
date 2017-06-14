#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#include "console.h"
#include "frogger.h"
#include "globals.h"

#define UP_DOWN_STEP 4

extern struct player *frog;

struct player {
  pthread_t tid;  // Thread id.
  pthread_t blinker;  // Thread for making the frog blink.
  pthread_t keyboard;  // Thread for handling user input to the frog.
  int x;  // x position (top left corner).
  int y;  // y position (top left corner).
  int blinking;  // Are the frog's eyes closed?
  int on_log;  // Is the frog on a log?
};

void *init_player(void *args);
void *blink(void *args);
void *input (void *args);
void pause_game();
void draw_frog();
void move_frog(int y, int x);
int on_screen(int y, int x, int height, int width);

#endif
