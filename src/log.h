#ifndef _LOG_H_
#define _LOG_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "console.h"
#include "frogger.h"
#include "llist.h"

#define LOG_WIDTH 24
#define LOG_HEIGHT 4

struct log {
  pthread_t tid;
  int row;
  int y;
  int x;
  int active;
};

void *init_producer(void *args);
void *init_log(void *args);

#endif
