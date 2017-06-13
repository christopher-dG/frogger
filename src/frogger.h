#ifndef _FROGGER_H
#define _FROGGER_H_

#include <pthread.h>

#include "globals.h"
#include "console.h"
#include "player.h"
#include "log.h"

extern pthread_mutex_t screen_lock;
extern pthread_mutex_t list_lock;
extern pthread_mutex_t frog_lock;

void *refresh(void *args);
void *check_lives(void *args);

#endif
