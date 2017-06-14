#ifndef _LOG_H_
#define _LOG_H_

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct log {
  pthread_t tid;
  int row;
  int direction;
  int y;
  int x;
  int active;
};

#include "console.h"
#include "wrappers.h"
#include "frogger.h"
#include "player.h"
#include "llist.h"

extern struct node *head;

void *init_producer(void *args);
void *init_log(void *args);
void *manage_logs(void *args);
void move_log(struct log *log, int direction);
void draw_log(struct log *log);

#endif
