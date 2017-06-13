#include "log.h"

struct node *head = NULL;
static char *LOG_GRAPHIC[LOG_HEIGHT] = {
  "/======================\\",
  "|                      |",
  "|                      |",
  "\\======================/"
};

void *init_producer(void *args) {
  pthread_t log;
  int i;
  while (1) {
    for (i=1; i<=LOG_ROWS-1; i++) {
      pthread_create(&log, NULL, &init_log, &i);
      sleepTicks(100);  // TODO: Make this random.
    }
  }
  return NULL;
}

void *init_log(void *args) {
  struct log *log = malloc(sizeof(struct log));
  log->tid = pthread_self();
  log->row = *(int *)args;
  log->direction = log->row % 2 ? 1 : -1;
  log->y = log->row * LOG_HEIGHT;
  log->x = log->direction > 0 ? LOG_WIDTH * -1 : GAME_COLS;
  log->active = 1;

  pthread_mutex_lock(&list_lock);
  if (!add(log, head)) head = create(log);
  pthread_mutex_unlock(&list_lock);

  log_move(log);

  return NULL;
}

void log_move(struct log *log) {
  while (log->active) {

    pthread_mutex_lock(&screen_lock);
    if (log->direction > 0)
      consoleClearImage(log->y, log->x - 1, LOG_WIDTH, LOG_HEIGHT);
    else
      consoleClearImage(log->y, log->x + LOG_WIDTH - 1, LOG_WIDTH, LOG_HEIGHT);
    pthread_mutex_unlock(&screen_lock);

    log->x += log->direction;
    draw_log(log);
    sleepTicks(log->row * 2);
    log->active = !(log->x < LOG_WIDTH * -1 || log->x >= GAME_COLS);
  }
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
