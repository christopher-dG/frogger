#include "wrappers.h"

void create_thread(pthread_t *thread, void *func, void *args) {
  if (pthread_create(thread, NULL, func, args)) {
    consoleFinish();
    printf("Thread creation failed.\n");
    exit(EXIT_FAILURE);
  }
}

void join_thread(pthread_t thread) {
  if (pthread_join(thread, NULL)) {
    consoleFinish();
    printf("Thread joining failed.\n");
    exit(EXIT_FAILURE);
  }
}

void lock_mutex(pthread_mutex_t *mutex) {
  if (pthread_mutex_lock(mutex)) {
    consoleFinish();
    printf("Mutex locking failed.\n");
    exit(EXIT_FAILURE);
  }
}

void unlock_mutex(pthread_mutex_t *mutex) {
  if (pthread_mutex_unlock(mutex)) {
    consoleFinish();
    printf("Mutex unlocking failed.\n");
    exit(EXIT_FAILURE);
  }
}

void cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex) {
  if (pthread_cond_wait(cv, mutex)) {
    consoleFinish();
    printf("Condition waiting failed.\n");
    exit(EXIT_FAILURE);
  }
}

void cond_signal(pthread_cond_t *cv) {
  if (pthread_cond_signal(cv)) {
    consoleFinish();
    printf("Condition signalling failed.\n");
    exit(EXIT_FAILURE);
  }
}
