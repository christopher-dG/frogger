#include "log.h"

struct node *head = NULL;
static char *LOG_GRAPHIC[LOG_HEIGHT] = {
  "/======================\\",
  "|                      |",
  "|                      |",
  "\\======================/"
};

void *init_producer(void *args) {
  int row = *(int *)args;
  return NULL;
}

void *init_log(void *args) {
  struct log *log = malloc(sizeof(struct log));
  log->tid = pthread_self();
  log->row = *(int *)args;
  log->y = log->row * 4;
  log->x = LOG_WIDTH * -1;
  log->active = 1;
  return NULL;
}

void delete_log(struct log *log, struct node *head) {
  pthread_mutex_lock(&list_lock);
  delete(log, head);
  pthread_mutex_unlock(&list_lock);
}

void draw_log(struct log *log) {
  pthread_mutex_lock(&screen_lock);
  consoleClearImage(log->y, log->x, LOG_WIDTH, LOG_HEIGHT);
  consoleDrawImage(log->y, log->x, LOG_GRAPHIC, LOG_HEIGHT);
  pthread_mutex_unlock(&screen_lock);
}
