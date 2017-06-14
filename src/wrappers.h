#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "console.h"

void create_thread(pthread_t *thread, void *func, void *args);
void join_thread(pthread_t thread);
void lock_mutex(pthread_mutex_t *mutex);
void unlock_mutex(pthread_mutex_t *mutex);
void cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex);
void cond_signal(pthread_cond_t *cv);

#endif
