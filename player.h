/*
   player.h

   Define's player.c's functions and the player struct.
*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "log.h"
#include "console.h"
#include "wrappers.h"
#include "frogger.h"
#include "globals.h"

extern struct player *frog;

struct player {
  int x;         // x position (top left corner).
  int y;         // y position (top left corner).
  int blinking;  // Are the frog's eyes closed?
};

// Create the frog and initialize its threads.
void *init_player(void *args);

// Continually blink the frog's eyes.
void *blink(void *args);

// Draw the frog in its current position.
void draw_frog();

// Move the frog back to the start and subtract a life.
void drown();

// Move the frog y and x units.
void move_frog(int y, int x);

// Determine if the frog is on screen or not.
int on_screen(int y, int x, int height, int width);

// Determine if the frog is in a safe space or not.
int is_safe();

// Determine if the frog is in a home slot.
int is_home();

// Determined which home slot the frog is in.
int get_slot();

// Determine if the frog is on a given log or not.
int on_log(struct log *log);

// Determine if the frog is on any log or not.
int on_any_log();

// Move the frog back to its starting position.
void reset_frog();

// Determine if the frog is on the last row of logs.
int on_last_row();

// Determine if the frog is in range of the goal row.
int can_reach_goal();
#endif
