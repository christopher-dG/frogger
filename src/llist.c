#include "llist.h"

void add(struct log *log, struct node **head) {
  struct node *new = malloc(sizeof(struct node));
  new->log = log;
  new->prev = NULL;
  new->next = *head;
  if (*head != NULL) (*head)->prev = new;
  *head = new;
}

/* void delete(struct node *node, struct node **head) { */
/*   if (*head == node) { */
/*     *head = node->next; */
/*     (*head)->prev = NULL; */
/*   } else { */
/*     if (node->prev != NULL) node->prev->next = node->next; */
/*     if (node->next != NULL) node->next->prev = node->prev; */
/*   } */
/*   free(node->log); */
/*   free(node); */
/* } */

void delete(struct log *log, struct node **head) {
  struct node *cur;
  struct node *del = NULL;;
  for (cur = *head; del == NULL && cur != NULL; cur = cur->next)
    if (cur->log == log) del = cur;
  if (del == *head) {
    *head = del->next;
    (*head)->prev = NULL;
  } else {
    if (del->prev != NULL) del->prev->next = del->next;
    if (del->next != NULL) del->next->prev = del->prev;
  }
  free(del->log);
  free(del);
}
