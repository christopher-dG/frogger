#include <stdio.h>

void ok(void *ptr) {
  printf("%d\n", *(int *)ptr);
}

int main() {
  int x = 5;
  ok(&x);
}
