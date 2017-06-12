#include "llist.h"

// Create a new linked list.
struct node *create(struct log *log) {
  struct node *head = malloc(sizeof(struct node));
  head->log = log;
  head->prev = NULL;
  head->next = NULL;
  return head;
}

// Add a new log to the end of the list.
int add(struct log *log, struct node *head) {
  if (head == NULL) return 0;
  struct node *cur;
  for (cur = head; cur->next != NULL; cur = cur->next)
    if (cur->log->tid == log->tid) return 0;
  if (cur->log->tid == log->tid) return 0;

  struct node *new = malloc(sizeof(struct node));
  new->log = log;
  new->prev = cur;
  new->next = NULL;
  cur->next = new;
  return 1;
}

// Delete a log from the list.
int delete(struct log *log, struct node *head) {
  struct node *cur;
  for (cur = head; cur != NULL; cur = cur->next) {
    if (cur->log->tid == log->tid) {
      if (cur->prev != NULL) cur->prev->next = cur->next;
      if (cur -> next != NULL) cur->next->prev = cur->prev;
      free(log);
      return 1;
    }
  }
  return 0;
}

// Retrieve a log from the list.
struct log *search(pthread_t tid, struct node *head) {
  struct node *cur;
  for (cur = head; cur != NULL; cur = cur->next)
    if (cur->log->tid == tid) return cur->log;
  return NULL;
}
