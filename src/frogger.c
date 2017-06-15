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

int lives = N_STARTING_LIVES;
int goals[N_GOALS];

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int running = 1;

pthread_mutex_t screen_lock = PTHREAD_MUTEX_INITIALIZER;;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t frog_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lives_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_t player;
pthread_t screen;
pthread_t log_producer;
pthread_t log_manager;
pthread_t game_monitor;

int main() {
  consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);

  create_thread(&screen, &refresh, NULL);
  create_thread(&player, &init_player, NULL);
  create_thread(&log_producer, &init_producer, NULL);
  create_thread(&log_manager, &manage_logs, NULL);
  create_thread(&game_monitor, &monitor_game, NULL);

  // Wait for the game to end.
  while (running) cond_wait(&cond, &screen_lock);

  printf("Done!\n");
  exit(EXIT_SUCCESS);
}

// Clean up and exit the game.
void quit(char *msg) {
  lock_mutex(&screen_lock);
  putBanner(msg);
  disableConsole(1);
  unlock_mutex(&screen_lock);

  finalKeypress();
  consoleFinish();

  running = false;
  cond_signal(&cond);

  join_thread(log_producer);
  join_thread(screen);
  join_thread(log_manager);
  join_thread(player);
  join_thread(game_monitor);
}

// Pause for some amount of time. If ticks is negative, wait
// until the game is manually resumed or quit.
void pause_game(int ticks) {
  lock_mutex(&lives_lock);
  lock_mutex(&frog_lock);
  lock_mutex(&screen_lock);
  lock_mutex(&list_lock);

  if (ticks >= 0) {
    disableConsole(1);
    sleepTicks(ticks);
  } else {
    putBanner(PAUSE_MSG);
    disableConsole(1);
    int paused = 1;
    char c;

    while (running && paused) {
      c = getchar();
      if (c == ' ') {
        disableConsole(0);
        putBanner("      ");
        paused = 0;
      } else if (c == 'q') {
        unlock_mutex(&screen_lock);
        unlock_mutex(&list_lock);
        unlock_mutex(&frog_lock);
        unlock_mutex(&lives_lock);
        disableConsole(0);
        quit(QUIT_MSG);
      } else sleep(0);
    }
  }

  disableConsole(0);
  unlock_mutex(&lives_lock);
  unlock_mutex(&screen_lock);
  unlock_mutex(&frog_lock);
  unlock_mutex(&list_lock);
}

  void *refresh(void *args) {
    while (running) {
      lock_mutex(&screen_lock);
      consoleRefresh();
      unlock_mutex(&screen_lock);

      sleepTicks(3);
    }
    return NULL;
  }

  void *monitor_game(void *args) {
    int i, complete;
    char *output[1];
    output[0] = malloc(2);

    while (running) {
      lock_mutex(&lives_lock);
      sprintf(output[0], "%d", lives);
      unlock_mutex(&lives_lock);

      lock_mutex(&screen_lock);
      consoleDrawImage(0, LIVES_X, output, 1);
      unlock_mutex(&screen_lock);

      if (lives <= 0) quit(LOSE_MSG);

      complete = 1;
      for (i=0; complete && i<N_GOALS; i++)
        if (!goals[i]) complete = 0;
      if (complete) quit("Congratulations.");

      sleepTicks(30);

    }
    free(output[0]);

    return NULL;
  }
