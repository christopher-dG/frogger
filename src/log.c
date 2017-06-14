#include "log.h"

struct node *head = NULL;
static char *LOG_GRAPHIC[N_LOG_GRAPHICS][LOG_HEIGHT + 1] = {
  {"/======================\\",
   "| +                    |",
   "|  +                   |",
   "\\======================/"},
  {"/======================\\",
   "|               +      |",
   "|                 +    |",
   "\\======================/"},
};

void *init_producer(void *args) {
  pthread_t log;
  int i;
  while (running) {
    for (i=LOG_ROWS; running && i>0; i--) {
      pthread_create(&log, NULL, &init_log, &i);
      sleepTicks(TICK_BASE + rand() % TICK_VARIATION);
    }
  }
  return NULL;
}

void *init_log(void *args) {
  struct log *log = malloc(sizeof(struct log));
  log->tid = pthread_self();
  log->row = *(int *)args;
  log->direction = log->row % 2 ? 1 : -1;
  log->y = log->row *  LOG_HEIGHT;
  log->x = log->direction > 0 ? LOG_WIDTH * -1 : GAME_COLS;
  log->active = 1;

  pthread_mutex_lock(&list_lock);
  if (!add(log, head)) head = create(log);
  pthread_mutex_unlock(&list_lock);

  while (running && log->active) {
    move_log(log, log->direction);
    sleepTicks(log->row * 2);
  }

  pthread_mutex_lock(&list_lock);
  assert(delete(log, head));
  pthread_mutex_unlock(&list_lock);

  return NULL;
}

void *manage_logs(void *args) {
  struct node *cur;

  pthread_mutex_lock(&list_lock);
  for (cur = head; cur != NULL; cur = cur->next)
    if (!cur->log->active) delete(cur->log, head);
  pthread_mutex_unlock(&list_lock);

  return NULL;
}

void move_log(struct log *log, int direction) {
  pthread_mutex_lock(&screen_lock);
  if (direction > 0)
    consoleClearImage(log->y, log->x, 1, LOG_HEIGHT);
  else
    consoleClearImage(log->y, log->x + LOG_WIDTH - 1, 1, LOG_HEIGHT);
  pthread_mutex_unlock(&screen_lock);

  log->x += direction;
  draw_log(log);
  if (has_frog(log)) move_frog(0, direction);
  log->active = !(log->x < LOG_WIDTH * -1 || log->x >= GAME_COLS);
}

void delete_log(struct log *log, struct node *head) {
  pthread_mutex_lock(&list_lock);
  delete(log, head);
  pthread_mutex_unlock(&list_lock);
}

int has_frog(struct log *log) {
  return frog->x >= log->x && frog->x <= log->x + LOG_WIDTH &&
    frog->y >= log->y && frog->y <= log->y + LOG_HEIGHT;
}

void draw_log(struct log *log) {
  int graphic = log->x % 15 >= 8 ? 0 : 1;
  pthread_mutex_lock(&screen_lock);
  consoleClearImage(log->y, log->x, LOG_WIDTH, LOG_HEIGHT);
  consoleDrawImage(log->y, log->x, LOG_GRAPHIC[graphic], LOG_HEIGHT);
  pthread_mutex_unlock(&screen_lock);
}
