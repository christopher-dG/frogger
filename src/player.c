#include "player.h"

static char *PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};

struct player *frog;

// Create the frog.
void *init_player(void *args) {
  lock_mutex(&frog_lock);
  frog = malloc(sizeof(struct player));
  frog->tid = pthread_self();
  frog->x = GAME_COLS / 2;
  frog->y = GAME_ROWS - PLAYER_HEIGHT - 1;
  unlock_mutex(&frog_lock);

  create_thread(&frog->blinker, &blink, NULL);
  create_thread(&frog->keyboard, &input, NULL);

  draw_frog();

  join_thread(frog->blinker);
  join_thread(frog->keyboard);
  free(frog);

  return NULL;
}

// Open or close the frog's eyes.
void *blink(void *args) {
  while (running) {
    lock_mutex(&frog_lock);
    frog->blinking = !frog->blinking;
    unlock_mutex(&frog_lock);

    draw_frog();
    sleepTicks(BLINK_DELAY);
  }

  return NULL;
}

// Parse keyboard input.
void *input(void *args) {
  struct timeval *timeout = malloc(sizeof(struct timeval));
  timeout->tv_usec = TIMEOUT_USEC;
  char *c = malloc(1);

  while (running) {
    while ((select(1, STDIN_FILENO, NULL, NULL, timeout)) > 1);
    read(STDIN_FILENO, c, 1);

    switch(c[0]) {
    case 'w':
      move_frog(UP * UP_DOWN_STEP, 0);
      break;
    case 'a':
      move_frog(0, LEFT);
      break;
    case 's':
      move_frog(DOWN * UP_DOWN_STEP, 0);
      break;
    case 'd':
      move_frog(0, RIGHT);
      break;
    case 'q':
      quit(QUIT_MSG);
      break;
    case ' ':
      pause_game(-1);
    }
  }

  free(c);
  free(timeout);

  return NULL;
}

// Draw the frog.
void draw_frog() {
  lock_mutex(&screen_lock);
  consoleClearImage(frog->y, frog->x, PLAYER_HEIGHT, PLAYER_WIDTH);
  consoleDrawImage(frog->y, frog->x, PLAYER_GRAPHIC[frog->blinking], PLAYER_HEIGHT);
  unlock_mutex(&screen_lock);
}

// Freeze the screen for a bit, deduct a life, and spawn the frog back at the start.
void drown() {
  draw_frog();
  sleepTicks(3);
  reset_frog();
  pause_game(50);
  lives--;
}


// Move the frog y and x units.
void move_frog(int y, int x) {
  if ((on_screen(frog->y + y, frog->x + x, PLAYER_HEIGHT, PLAYER_WIDTH))) {

    lock_mutex(&screen_lock);
    consoleClearImage(frog->y, frog->x, PLAYER_HEIGHT, PLAYER_WIDTH);
    unlock_mutex(&screen_lock);

    lock_mutex(&frog_lock);
    frog->x += x;
    frog->y += y;
    unlock_mutex(&frog_lock);

    // Is the frog safe? Otherwise, reset.
    // Is the frog in a cubby?
    if (!is_safe()) drown();
    draw_frog();


  }
}

int on_log(struct log *log) {
  return frog->x > log->x && frog->x < log->x + LOG_WIDTH - PLAYER_WIDTH &&
    frog->y >= log->y && frog->y <= log->y + LOG_HEIGHT;
}

int is_safe() {
  int safe = frog->y < RIVER_START || frog->y > RIVER_END;
  struct node *cur;

  lock_mutex(&list_lock);
  for (cur = head; !safe && cur != NULL; cur = cur->next)
    safe = on_log(cur->log);
  unlock_mutex(&list_lock);

  return safe;
}

int on_screen(int y, int x, int height, int width) {
  return (y >= 0 && x >= 0 && y + height - 1 < GAME_ROWS && x + width - 1 < GAME_COLS);
}

void reset_frog() {
  lock_mutex(&frog_lock);
  frog->x = GAME_COLS / 2;
  frog->y = GAME_ROWS - PLAYER_HEIGHT - 1;
  unlock_mutex(&frog_lock);
  draw_frog();
}
