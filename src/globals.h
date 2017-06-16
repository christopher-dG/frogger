#ifndef _GLOBALS_H
#define _GLOBALS_H_

// Number of rows in the console.
#define GAME_ROWS 24
// Number of columns in the console.
#define GAME_COLS 80
//Number of player graphics to cycle through.
#define N_PLAYER_GRAPHICS 2
// Number of log graphics to cycle through.
#define N_LOG_GRAPHICS 2
// Height of the player graphics.
#define PLAYER_HEIGHT 2
// Width of the player graphics.
#define PLAYER_WIDTH 2
// Player starting x position.
#define PLAYER_START_X GAME_COLS / 2
// Player starting y position.
#define PLAYER_START_Y GAME_ROWS - PLAYER_HEIGHT - 1
// Time in ms / 10 between blinks.
#define BLINK_DELAY 100
// Number of lives to start the game with.
#define N_STARTING_LIVES 4
// Number of rows of logs.
#define N_LOG_ROWS 4
// x position of the current lives display.
#define LIVES_X 42
// Number of rows to move the player up/down.
#define UP_DOWN_STEP 4
// Number of rows to move player up/down into a goal.
#define FINAL_UP_DOWN_STEP 3
// y position of the frog when it has crosse all logs.
#define HOME_Y 5
// y position of the frog when it is on the last row of logs.
#define LAST_LOG_ROW_Y 5
// Width of each home slot.
#define HOME_WIDTH 6 - (PLAYER_WIDTH - 1)
// y position of the home slots.
#define HOME_ROW 2
// x position of the first home slot.
#define HOME_COL_1 1
// x position of the second home slot.
#define HOME_COL_2 19
// x position of the third home slot.
#define HOME_COL_3 37
// x position of the fourth home slot.
#define HOME_COL_4 55
// x position of the fifth home slot.
#define HOME_COL_5 73

// Number of nanoseconds to wait for user input.
#define TIMEOUT_USEC 166667
// Number of goals to fill.
#define N_GOALS 5
// y position of the first river tile.
#define RIVER_START 4
// y position of the last river tile.
#define RIVER_END 20
//Width of a log.
#define LOG_WIDTH 24
// Height of a log.
#define LOG_HEIGHT 4
// Minimum number of ms / 10 between log spawns.
#define TICK_BASE 70
// Number of ms / 10 to vary log spawns.
#define TICK_VARIATION 90
// Screen refresh rate.
#define REFRESH_RATE 30
// Message to display while paused.
#define PAUSE_MSG "PAUSED"
// Blank space to overwrite the pause message.
#define UNPAUSE_MSG "      "
// Message to display upon quitting.
#define QUIT_MSG "You didn't lose, but... you didn't win."
// Message to display upon winning.
#define WIN_MSG "Congratulations!"
// Message to display upon losing.
#define LOSE_MSG "Maybe next time."
// Key to move player upwards.
#define KEY_UP 'w'
// Key to move player downwards.
#define KEY_DOWN 's'
// Key to move player to the left.
#define KEY_LEFT 'a'
// Key to move player to the right.
#define KEY_RIGHT 'd'
// Key to pause the game.
#define KEY_PAUSE ' '
// Key to quit the game.
#define KEY_QUIT 'q'

#endif
