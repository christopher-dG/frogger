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

void quit(char *msg);
void *refresh(void *args);
void *monitor_game(void *args);
void pause_game(int ticks);

#endif
