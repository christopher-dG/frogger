#ifndef _LLIST_H_
#define _LLIST_H_

#include <stdlib.h>

struct node {
  struct log *log;
  struct node *prev;
  struct node *next;
};

#include "log.h"
#include "frogger.h"

struct node *create(struct log *log);
int add(struct log *log, struct node *head);
int delete(struct log *log, struct node *head);
char *length(struct node *head);

#endif
