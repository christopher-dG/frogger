#ifndef _LLIST_H_
#define _LLIST_H_

#include <stdlib.h>

struct node {
  struct log *log;
  struct node *prev;
  struct node *next;
};

#include "log.h"

// Add a new node to the list.
void add(struct log *log, struct node **head);

// Delete a node from the list.
// void delete(struct node *node, struct node **head);
void delete(struct log *log, struct node **head);

#endif
