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
  "" };

pthread_mutex_t screen_lock = PTHREAD_MUTEX_INITIALIZER;

int main() {
  consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);
  pthread_t player;
  pthread_create(&player, NULL, &init_player, NULL);
  pthread_join(player, NULL);
  consoleFinish();
}
