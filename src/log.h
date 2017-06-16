#ifndef _LOG_H_
#define _LOG_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct log {
  pthread_t tid;  // Thread id.
  int y;          // y position (top left corner).
  int x;          // x position (top left corner).
  int row;        // Row of logs that this log occupies.
  int direction;  // Direction that this log is moving in.
  int active;     // Whether this log is still on screen or not.
};

#include "console.h"
#include "wrappers.h"
#include "frogger.h"
#include "player.h"
#include "llist.h"

extern struct node *head;

// Continually spawn new log threads.
void *init_producer(void *args);

// Create a new log and move it across the screen.
void *init_log(void *args);

// Clean up inactive logs.
void *manage_logs(void *args);

// Move a log.
void move_log(struct log *log, int direction);

// Draw a log.
void draw_log(struct log *log);

#endif
