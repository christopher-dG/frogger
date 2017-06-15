#include "llist.h"

void add(struct log *log, struct node **head) {
  struct node *new = malloc(sizeof(struct node));
  new->log = log;
  new->prev = NULL;
  new->next = *head;
  if (*head != NULL) (*head)->prev = new;
  *head = new;
}

void delete(struct node *node, struct node **head) {
  if (*head == node) {
    *head = node->next;
    (*head)->prev = NULL;
  } else {
    if (node->prev != NULL) node->prev->next = node->next;
    if (node->next != NULL) node->next->prev = node->prev;
  }
  free(node->log);
  free(node);
}
