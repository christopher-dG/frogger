#include "player.h"

static char *PLAYER_GRAPHIC[N_PLAYER_GRAPHICS][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};

struct player *frog;  // The player's frog.

void *init_player(void *args) {
  lock_mutex(&frog_lock);
  frog = malloc(sizeof(struct player));
  frog->blinking = 0;
  unlock_mutex(&frog_lock);
  reset_frog();

  pthread_t blinker;
  create_thread(&blinker, &blink, NULL);

  draw_frog();

  join_thread(blinker);
  free(frog);

  return NULL;
}

void *blink(void *args) {
  while (running) {
    lock_mutex(&frog_lock);
    frog->blinking = abs(frog->blinking - 1);
    unlock_mutex(&frog_lock);

    draw_frog();
    sleepTicks(BLINK_DELAY);
  }

  return NULL;
}

void draw_frog() {
  lock_mutex(&screen_lock);
  lock_mutex(&frog_lock);
  consoleClearImage(frog->y, frog->x, PLAYER_HEIGHT, PLAYER_WIDTH);
  consoleDrawImage(frog->y, frog->x, PLAYER_GRAPHIC[frog->blinking], PLAYER_HEIGHT);
  unlock_mutex(&frog_lock);
  unlock_mutex(&screen_lock);
}

void drown() {
  draw_frog();
  sleepTicks(3);
  reset_frog();
  pause_game(50);

  lock_mutex(&lives_lock);
  lives--;
  unlock_mutex(&lives_lock);
}

void move_frog(int y, int x) {
  if ((on_screen(frog->y + y, frog->x + x, PLAYER_HEIGHT, PLAYER_WIDTH))) {
    lock_mutex(&frog_lock);
    lock_mutex(&screen_lock);
    consoleClearImage(frog->y, frog->x, PLAYER_HEIGHT, PLAYER_WIDTH);
    unlock_mutex(&screen_lock);
    frog->x += x;
    frog->y += y;
    unlock_mutex(&frog_lock);

    if (is_home() && !goals[get_slot()]) {
      goals[get_slot()] = 1;
      draw_frog();
      reset_frog();
    } else if (!is_home()) draw_frog();
    else {
      lock_mutex(&frog_lock);
      frog->x -= x;
      frog->y -= y;
      unlock_mutex(&frog_lock);
    }
  } else if (on_any_log()) drown();

  if (!is_safe() && !on_any_log()) drown();
}

int on_log(struct log *log) {
  lock_mutex(&frog_lock);
  int on = frog->x >= log->x &&
    frog->x <= log->x + LOG_WIDTH - PLAYER_WIDTH &&
    frog->y >= log->y && frog->y <= log->y + LOG_HEIGHT;
  unlock_mutex(&frog_lock);
  return on;
}

int on_any_log(struct log *log) {
  struct node *cur;
  int on = 0;
  lock_mutex(&list_lock);
  for (cur = head; !on && cur != NULL; cur = cur->next)
    on = on_log(cur->log);
  unlock_mutex(&list_lock);
  return on;
}

int is_safe() {
  lock_mutex(&frog_lock);
  int safe = frog->y < RIVER_START || frog->y > RIVER_END;
  unlock_mutex(&frog_lock);
  struct node *cur;

  lock_mutex(&list_lock);
  for (cur = head; !safe && cur != NULL; cur = cur->next)
    safe = on_log(cur->log);
  unlock_mutex(&list_lock);

  return safe;
}

int is_home() {
  lock_mutex(&frog_lock);
  int home = frog->y == HOME_ROW &&
    ((frog->x >= HOME_COL_1 && frog->x < HOME_COL_1 + HOME_WIDTH) ||
     (frog->x >= HOME_COL_2 && frog->x < HOME_COL_2 + HOME_WIDTH) ||
     (frog->x >= HOME_COL_3 && frog->x < HOME_COL_3 + HOME_WIDTH) ||
     (frog->x >= HOME_COL_4 && frog->x < HOME_COL_4 + HOME_WIDTH) ||
     (frog->x >= HOME_COL_5 && frog->x < HOME_COL_5 + HOME_WIDTH));
  unlock_mutex(&frog_lock);
  return home;
}

int get_slot() {
  lock_mutex(&frog_lock);
  int x = frog->x;
  unlock_mutex(&frog_lock);
  if (x >= HOME_COL_1 && x < HOME_COL_1 + HOME_WIDTH) return 0;
  if (x >= HOME_COL_2 && x < HOME_COL_2 + HOME_WIDTH) return 1;
  if (x >= HOME_COL_3 && x < HOME_COL_3 + HOME_WIDTH) return 2;
  if (x >= HOME_COL_4 && x < HOME_COL_4 + HOME_WIDTH) return 3;
  if (x >= HOME_COL_5 && x < HOME_COL_5 + HOME_WIDTH) return 4;
  return -1;
}

int on_screen(int y, int x, int height, int width) {
  return (y >= 0 && x >= 0 && y + height - 1 < GAME_ROWS && x + width - 1 < GAME_COLS);
}

void reset_frog() {
  lock_mutex(&frog_lock);
  frog->x = PLAYER_START_X;
  frog->y = PLAYER_START_Y;
  unlock_mutex(&frog_lock);
  draw_frog();
}
int on_last_row() {
  lock_mutex(&frog_lock);
  int can = frog->y == LAST_LOG_ROW_Y;
  unlock_mutex(&frog_lock);
  return can;
}

int can_reach_goal() {
  int can = on_last_row();
  if (can) {
    lock_mutex(&frog_lock);
    can = (frog->x >= HOME_COL_1 && frog->x < HOME_COL_1 + HOME_WIDTH) ||
      (frog->x >= HOME_COL_2 && frog->x < HOME_COL_2 + HOME_WIDTH) ||
      (frog->x >= HOME_COL_3 && frog->x < HOME_COL_3 + HOME_WIDTH) ||
      (frog->x >= HOME_COL_4 && frog->x < HOME_COL_4 + HOME_WIDTH) ||
      (frog->x >= HOME_COL_5 && frog->x < HOME_COL_5 + HOME_WIDTH);
    unlock_mutex(&frog_lock);
  }
  return can;
}
