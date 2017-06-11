#include "player.h"

void *init_player(void *args) {
  struct player *frog = malloc(sizeof(struct player));
  frog->tid = pthread_self();
  frog->x = GAME_COLS / 2;
  frog->y = GAME_ROWS - PLAYER_HEIGHT - 1;

  pthread_create(&frog->blinker, NULL, &blink, (void *)frog);
  pthread_create(&frog->keyboard, NULL, &input, (void *)frog);

  draw(frog);

  pthread_join(frog->blinker, NULL);
  pthread_join(frog->keyboard, NULL);
  free(frog);
  return NULL;
}

// Open or close the frog's eyes.
void *blink(void *args) {
  struct player *frog = (struct player *)args;
  while (1) {
    frog->blinking = !frog->blinking;
    draw(frog);
    sleep(1);
  }
  return NULL;
}

void *input(void *args) {
  struct player *frog = (struct player *)args;
  struct timeval *timeout = malloc(sizeof(struct timeval));
  timeout->tv_usec = 166667;  // 1/60 seconds.
  char *c = malloc(1);

  while (1) {  // TODO: Probably needs to use the condition variable.
    if ((select(1, STDIN_FILENO, NULL, NULL, timeout)) > 1);
    read(STDIN_FILENO, c, 1);
    switch(c[0]) {
    case 'w':
      move(frog, UP, 0);
      break;
    case 'a':
      move(frog, 0, LEFT);
      break;
    case 's':
      move(frog, DOWN, 0);
      break;
    case 'd':
      move(frog, 0, RIGHT);
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
void draw(struct player *frog) {
  char **tile = PLAYER_GRAPHIC[frog->blinking];

  pthread_mutex_lock(&screen_lock);
  consoleClearImage(frog->y, frog->x, PLAYER_HEIGHT, PLAYER_WIDTH);
  consoleDrawImage(frog->y, frog->x, tile, PLAYER_HEIGHT);
  consoleRefresh();
  pthread_mutex_unlock(&screen_lock);
}

// Move the frog y and x units.
// Note that y comes first as per curses convention.
void move(struct player *frog, int y, int x) {
  if ((valid_move(frog->y + y, frog->x + x, PLAYER_HEIGHT, PLAYER_WIDTH))) {

    pthread_mutex_lock(&screen_lock);
    consoleClearImage(frog->y, frog->x, PLAYER_HEIGHT, PLAYER_WIDTH);
    pthread_mutex_unlock(&screen_lock);

    frog->x += x;
    frog->y += y;
    draw(frog);
  }
}

int valid_move(int y, int x, int height, int width) {
  return (y >= 0 && x >= 0 && y + height - 1 < GAME_ROWS && x + width - 1 < GAME_COLS);
}
