#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

#include "console.h"
#include "frogger.h"

// Create a new thead at func.
void create_thread(pthread_t *thread, void *func, void *args);

// Join a thread.
void join_thread(pthread_t thread);

// Lock a mutex.
void lock_mutex(pthread_mutex_t *mutex);

// Unlock a mutex.
void unlock_mutex(pthread_mutex_t *mutex);

// Wait for a condition variable.
void cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex);

// Signal a condition variable.
void cond_signal(pthread_cond_t *cv);

#endif
