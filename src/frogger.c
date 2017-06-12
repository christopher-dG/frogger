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

/* pthread_mutex_t lives_lock; */
pthread_mutex_t screen_lock = PTHREAD_MUTEX_INITIALIZER;;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;

int main() {
  consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);

  pthread_t player;
  pthread_t screen;
  pthread_t log_producer[LOG_ROWS];
  /* pthread_t lives; */

  pthread_create(&screen, NULL, &refresh, NULL);
  pthread_create(&player, NULL, &init_player, NULL);
  /* pthread_create(&lives, NULL, &check_lives, NULL); */
  int i, j;
  for (i=0; i<LOG_ROWS; i++) {
    j = i+1;
    pthread_create(&log_producer[i], NULL, &init_producer, &j);
  }

  pthread_join(screen, NULL);
  pthread_join(player, NULL);
  /* pthread_join(lives, NULL); */

  consoleFinish();
}

void *refresh(void *args) {
  while (1) {
    consoleRefresh();
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
