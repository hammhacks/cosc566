#include <stdio.h>
#include <stdlib.h>

void function(int *val_ptr) {
  printf("Value is %d\n", *val_ptr);
}

int main(int argc, char **argv) {
  int values[100];
  values[25] = 1234;

  function(values + 2525);
  return 0;
}
