#ifndef _LOG_H_
#define _LOG_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "console.h"
#include "frogger.h"
#include "player.h"
#include "llist.h"

#define LOG_WIDTH 24
#define LOG_HEIGHT 4
#define N_LOG_GRAPHICS 2

struct log {
  pthread_t tid;
  int row;
  int direction;
  int y;
  int x;
  int active;
};

void *init_producer(void *args);
void *init_log(void *args);
void move_log(struct log *log, int direction);
void delete_log(struct log *log, struct node *head);
void draw_log(struct log *log);
int has_frog(struct log *log);

#endif
