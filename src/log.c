#include "log.h"

struct node *head = NULL;  // A linked list of logs.
static char *LOG_GRAPHIC[N_LOG_GRAPHICS][LOG_HEIGHT + 1] = {
  {"/======================\\",
   "|   +                  |",
   "|     +                |",
   "\\======================/"},
  {"/======================\\",
   "|               +      |",
   "|                 +    |",
   "\\======================/"},
};

// Continuously spawn new log threads.
void *init_producer(void *args) {
  pthread_t log;
  int i;
  while (running) {
    for (i=LOG_ROWS; running && i>0; i--) {
      lock_mutex(&list_lock);
      create_thread(&log, &init_log, &i);
      unlock_mutex(&list_lock);
      sleepTicks(TICK_BASE + rand() % TICK_VARIATION);
    }
  }

  return NULL;
}

// Create a new log and move it across the screen.
void *init_log(void *args) {
  struct log *log = malloc(sizeof(struct log));
  log->tid = pthread_self();
  log->row = *(int *)args;
  log->direction = log->row % 2 ? 1 : -1;
  log->y = log->row *  LOG_HEIGHT;
  log->x = log->direction > 0 ? LOG_WIDTH * -1 : GAME_COLS;
  log->active = 1;

  // THIS IS FUCKING THINGS UP
  /* lock_mutex(&list_lock); */
  if (!add(log, head)) head = create(log);
  /* unlock_mutex(&list_lock); */

  while (running && log->active) {
    move_log(log, log->direction);
    sleepTicks(log->row * 2);
  }

  return NULL;
}

void *manage_logs(void *args) {
  struct node *cur;

  while (running) {
    lock_mutex(&list_lock);
    for (cur = head; cur != NULL; cur = cur->next) {
      if (!cur->log->active) delete(cur->log, head);
    }
    unlock_mutex(&list_lock);
    sleepTicks(100);
  }

  return NULL;
}

void move_log(struct log *log, int direction) {
  lock_mutex(&screen_lock);
  if (direction > 0)
    consoleClearImage(log->y, log->x, 1, LOG_HEIGHT);
  else
    consoleClearImage(log->y, log->x + LOG_WIDTH - 1, 1, LOG_HEIGHT);
  unlock_mutex(&screen_lock);

  log->x += direction;
  draw_log(log);
  if (on_log(log)) move_frog(0, direction);
  log->active = !(log->x < LOG_WIDTH * -1 || log->x >= GAME_COLS);
}

void draw_log(struct log *log) {
  int graphic = abs(log->x) % 20 < 10 ? 0 : 1;  // Alternate the graphic every 10 frames.
  lock_mutex(&screen_lock);
  consoleClearImage(log->y, log->x, LOG_WIDTH, LOG_HEIGHT);
  consoleDrawImage(log->y, log->x, LOG_GRAPHIC[graphic], LOG_HEIGHT);
  unlock_mutex(&screen_lock);
}
