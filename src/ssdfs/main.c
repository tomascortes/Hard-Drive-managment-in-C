#include <stdio.h>
#include "os_API.h"

// FIXME: Second parameter of 'main' (argument array) must be type 'char **'
int main(int argc, char** const argv[]) {
  os_mount(argv[1], 5000);

  os_bitmap(0);

  return 0;
}
