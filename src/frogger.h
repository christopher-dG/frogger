#ifndef _FROGGER_H
#define _FROGGER_H_

#include <pthread.h>

#include "globals.h"
#include "wrappers.h"
#include "console.h"
#include "player.h"
#include "log.h"

extern int running;
extern int lives;
extern pthread_cond_t cond;
extern pthread_mutex_t screen_lock;
extern pthread_mutex_t list_lock;
extern pthread_mutex_t frog_lock;
extern pthread_mutex_t lives_lock;

// Exit the game.
void quit_game(char *msg);

// Pause for some amount of time. If ticks is negative, wait
// until the game is manually resumed or quit.
void pause_game(int ticks);

// Continuously update the screen.
void *refresh(void *args);

// Keep track of lives, goals.
void *monitor_game(void *args);


#endif
