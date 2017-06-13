#include "frogger.h"

char *GAME_BOARD[] = {
  "                                   Lives:",
  "/------\\          /------\\          /------\\          /------\\          /------\\",
  "|      |          |      |          |      |          |      |          |      |",
  "+      +----------+      +----------+      +----------+      +----------+      +",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"",
  "",
  "",
  ""
};
/* int lives = STARTING_LIVES; */

/* pthread_cond_t cond; */
/* int running = 1; */

/* pthread_mutex_t lives_lock; */
pthread_mutex_t screen_lock = PTHREAD_MUTEX_INITIALIZER;;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t frog_lock = PTHREAD_MUTEX_INITIALIZER;

int main() {
  consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);

  /* pthread_cond_init(&cond); */

  pthread_t player;
  pthread_t screen;
  pthread_t log_producer;
  /* pthread_t lives; */

  pthread_create(&screen, NULL, &refresh, NULL);
  pthread_create(&player, NULL, &init_player, NULL);
  /* pthread_create(&lives, NULL, &check_lives, NULL); */
  pthread_create(&log_producer, NULL, &init_producer, NULL);

  pthread_join(log_producer, NULL);
  pthread_join(screen, NULL);
  pthread_join(player, NULL);
  /* pthread_join(lives, NULL); */

  consoleFinish();
}

void *refresh(void *args) {
  while (1) {
    pthread_mutex_lock(&screen_lock);
    consoleRefresh();
    pthread_mutex_unlock(&screen_lock);
    sleepTicks(3);
  }
}

/* // Post the number of lives the player has remaining. */
/* void *check_lives(void *args) { */
/*   char *num_lives[1]; */
/*   num_lives[0] = malloc(sizeof(char *)); */
/*   sprintf(num_lives[0], "%d", lives); */
/*   while (1) { */
/*     consoleDrawImage(0, 50, num_lives, 1); */
/*     if (!lives) { */
/*       putBanner("You lose."); */
/*       // Quit. */
/*     } */
/*   } */
/*   return NULL; */
/* } */
