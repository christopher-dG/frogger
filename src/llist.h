#ifndef _LLIST_H_
#define _LLIST_H_

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

struct node {
  struct log *log;
  struct node *prev;
  struct node *next;
};

struct node *create(struct log *log);
int add(struct log *log, struct node *head);
int delete(struct log *log, struct node *head);
struct log *search(pthread_t tid, struct node *head);
#endif
