/*
  frogger.c

  Responsible for basic game maintenance such as starting/pausing/ending, as well as user input.
*/

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

int lives = N_STARTING_LIVES;                    // Current lives remaining.
int goals[N_GOALS];                              // Array of "home" spots for the player to reach.
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // Condition variable for game end.
int running = 1;                                 // True until the game ends.

pthread_mutex_t screen_lock = PTHREAD_MUTEX_INITIALIZER;  // Lock for accessing the screen.
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;    // Lock for accessing the list of logs.
pthread_mutex_t frog_lock = PTHREAD_MUTEX_INITIALIZER;    // Lock for accessing the player.
pthread_mutex_t lives_lock = PTHREAD_MUTEX_INITIALIZER;   // Lock for accessing the remaining lives.

pthread_t screen;                    // Thread to refresh the screen.
pthread_t player;                    // Thread to control the player.
pthread_t keyboard;                  // Thread to handle user input.
pthread_t log_manager;               // Thread to clean up inactive logs.
pthread_t game_monitor;              // Thread to monitor remaining lives and goals.
pthread_t log_producer[N_LOG_ROWS];  // Thread to continually spawn new logs.

int main() {
  int i;
  consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);

  // Create the threads that run continously.
  create_thread(&screen, &refresh, NULL);
  create_thread(&player, &init_player, NULL);
  create_thread(&keyboard, &input, NULL);
  create_thread(&log_manager, &manage_logs, NULL);
  create_thread(&game_monitor, &monitor_game, NULL);
  for (i=0; i<N_LOG_ROWS; i++) create_thread(&log_producer[i], &init_producer, &i);


  // Wait for the game to end.
  while (running) cond_wait(&cond, &screen_lock);

  printf("Done!\n");
  return EXIT_SUCCESS;
}

void quit_game(char *msg) {
  int i;

  lock_mutex(&screen_lock);
  putBanner(msg);
  disableConsole(1);
  unlock_mutex(&screen_lock);

  finalKeypress();
  consoleFinish();

  running = false;
  cond_signal(&cond);

  for (i=0; i<N_LOG_ROWS; i++) join_thread(log_producer[i]);
  join_thread(game_monitor);
  join_thread(screen);
  join_thread(player);
  join_thread(log_manager);

}

void pause_game(int ticks) {
  lock_mutex(&lives_lock);
  lock_mutex(&frog_lock);
  lock_mutex(&screen_lock);
  lock_mutex(&list_lock);

  if (ticks >= 0) {
    // Pause for some number of ticks.
    disableConsole(1);
    sleepTicks(ticks);
  } else {
    // Pause until manual unpause or quit.
    putBanner(PAUSE_MSG);
    disableConsole(1);
    int paused = 1;

    while (running && paused) {
      switch(getchar()) {
      case KEY_PAUSE:
        disableConsole(0);
        putBanner(UNPAUSE_MSG);
        paused = 0;
        break;
      case KEY_QUIT:
        unlock_mutex(&screen_lock);
        unlock_mutex(&list_lock);
        unlock_mutex(&frog_lock);
        unlock_mutex(&lives_lock);
        disableConsole(0);
        quit_game(QUIT_MSG);
        break;
      }
    }
  }

  disableConsole(0);
  unlock_mutex(&lives_lock);
  unlock_mutex(&screen_lock);
  unlock_mutex(&frog_lock);
  unlock_mutex(&list_lock);
}

void *input(void *args) {
  struct timespec timeout = getTimeout(1);
  char *c = malloc(1);
  int ret;
  fd_set set;

  while (running) {
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    ret = pselect(FD_SETSIZE, &set, NULL, NULL, &timeout, NULL);

    if (ret > 0) read(STDIN_FILENO, c, 1);
    else continue;

    switch(c[0]) {
    case KEY_UP:
      if (can_reach_goal()) {
        move_frog(UP * FINAL_UP_DOWN_STEP, 0);
      } else if (!on_last_row()) {
        move_frog(UP * UP_DOWN_STEP, 0);
      }
      break;
    case KEY_DOWN:
      move_frog(DOWN * UP_DOWN_STEP, 0);
      break;
    case KEY_LEFT:
      move_frog(0, LEFT);
      break;
    case KEY_RIGHT:
      move_frog(0, RIGHT);
      break;
    case KEY_QUIT:
      quit_game(QUIT_MSG);
      break;
    case KEY_PAUSE:
      pause_game(-1);
      break;
    }
  }

  free(c);

  return NULL;
}

void *refresh(void *args) {
  while (running) {
    lock_mutex(&screen_lock);
    consoleRefresh();
    unlock_mutex(&screen_lock);

    sleepTicks(100 / REFRESH_RATE);
  }
  return NULL;
}

void *monitor_game(void *args) {
  int i, won;
  char *output[1];
  output[0] = malloc(2);

  while (running) {
    lock_mutex(&lives_lock);
    sprintf(output[0], "%d", lives);
    unlock_mutex(&lives_lock);

    lock_mutex(&screen_lock);
    consoleDrawImage(0, LIVES_X, output, 1);
    unlock_mutex(&screen_lock);

    if (lives <= 0) quit_game(LOSE_MSG);

    won = 1;
    // If there are any unfilled goals, the game is not yet won.
    for (i=0; won && i<N_GOALS; i++)
      if (!goals[i]) won = 0;
    if (won) quit_game(WIN_MSG);

    sleepTicks(1);
  }

  free(output[0]);

  return NULL;
}
