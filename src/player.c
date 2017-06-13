#include "player.h"

static char *PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};

struct player *frog;

void *init_player(void *args) {
  pthread_mutex_lock(&frog_lock);
  frog = malloc(sizeof(struct player));
  frog->tid = pthread_self();
  frog->x = GAME_COLS / 2;
  frog->y = GAME_ROWS - PLAYER_HEIGHT - 1;
  pthread_mutex_unlock(&frog_lock);

  pthread_create(&frog->blinker, NULL, &blink, NULL);
  pthread_create(&frog->keyboard, NULL, &input, NULL);

  draw_frog();

  pthread_join(frog->blinker, NULL);
  pthread_join(frog->keyboard, NULL);
  free(frog);
  return NULL;
}

// Open or close the frog's eyes.
void *blink(void *args) {
  while (1) {
    pthread_mutex_lock(&frog_lock);
    frog->blinking = !frog->blinking;
    pthread_mutex_unlock(&frog_lock);

    draw_frog();
    sleep(1);
    // sleepTicks(100);
  }
  return NULL;
}

void *input(void *args) {
  struct timeval *timeout = malloc(sizeof(struct timeval));
  timeout->tv_usec = 166667;  // 1/60 seconds.
  char *c = malloc(1);

  while (1) {  // TODO: Probably needs to use the condition variable.
    if ((select(1, STDIN_FILENO, NULL, NULL, timeout)) > 1);
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
      // Quit: Set a condition variable.
      break;
    }
  }

  free(c);
  free(timeout);
}

// Draw the frog.
void draw_frog() {
  char **tile = PLAYER_GRAPHIC[frog->blinking];

  pthread_mutex_lock(&screen_lock);
  consoleClearImage(frog->y, frog->x, PLAYER_HEIGHT, PLAYER_WIDTH);
  consoleDrawImage(frog->y, frog->x, tile, PLAYER_HEIGHT);
  pthread_mutex_unlock(&screen_lock);
}

// Move the frog y and x units.
// Note that y comes first as per curses convention.
void move_frog(int y, int x) {
  if ((on_screen(frog->y + y, frog->x + x, PLAYER_HEIGHT, PLAYER_WIDTH))) {

    pthread_mutex_lock(&screen_lock);
    consoleClearImage(frog->y, frog->x, PLAYER_HEIGHT, PLAYER_WIDTH);
    pthread_mutex_unlock(&screen_lock);

    pthread_mutex_lock(&frog_lock);
    frog->x += x;
    frog->y += y;
    pthread_mutex_unlock(&frog_lock);

    draw_frog();

  }
}

int on_screen(int y, int x, int height, int width) {
  return (y >= 0 && x >= 0 && y + height - 1 < GAME_ROWS && x + width - 1 < GAME_COLS);
}
