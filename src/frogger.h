#ifndef _FROGGER_H
#define _FROGGER_H_

#include <pthread.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#include "globals.h"
#include "wrappers.h"
#include "console.h"
#include "player.h"
#include "log.h"

extern int running;
extern int lives;
extern int goals[];
extern pthread_cond_t cond;
extern pthread_mutex_t screen_lock;
extern pthread_mutex_t list_lock;
extern pthread_mutex_t frog_lock;
extern pthread_mutex_t lives_lock;

// Exit the game.
void quit_game(char *msg);

// Pause for some amount of time, or until the game is unpaused or quit.
void pause_game(int ticks);

// Continuously update the screen.
void *refresh(void *args);

// Keep track of lives, goals.
void *monitor_game(void *args);


#endif
