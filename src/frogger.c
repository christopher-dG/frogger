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

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int running = 1;

pthread_mutex_t screen_lock = PTHREAD_MUTEX_INITIALIZER;;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t frog_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_t player;
pthread_t screen;
pthread_t log_producer;
pthread_t log_manager;

int main() {
  consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);
  pthread_create(&screen, NULL, &refresh, NULL);
  pthread_create(&player, NULL, &init_player, NULL);
  pthread_create(&log_producer, NULL, &init_producer, NULL);
  pthread_create(&log_manager, NULL, &manage_logs, NULL);
  while (running) pthread_cond_wait(&cond, &screen_lock);
  printf("Done!\n");
}

void quit() {
    pthread_mutex_lock(&screen_lock);
    putBanner("You didn't lose but... you didn't win.");
    disableConsole(1);
    pthread_mutex_unlock(&screen_lock);

    finalKeypress();
    consoleFinish();

    running = false;
    pthread_cond_signal(&cond);

    pthread_join(log_producer, NULL);
    pthread_join(screen, NULL);
    pthread_join(log_manager, NULL);
    pthread_join(player, NULL);
}

void *refresh(void *args) {
  while (running) {
    pthread_mutex_lock(&screen_lock);
    consoleRefresh();
    pthread_mutex_unlock(&screen_lock);

    sleepTicks(3);
  }
  return NULL;
}
