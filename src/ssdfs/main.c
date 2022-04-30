#include <stdio.h>
#include "./os_API.h"

int main(int argc, char** const argv[])
{ 
  FILE *f = fopen("simdiskfilled.bin", "rb");

  os_bitmap(1, f);
  printf("\n");
  os_bitmap(2, f);
  printf("\n");
  os_bitmap(7, f);
  printf("\n");
  os_bitmap(8, f);

  fclose(f);

  return 0;
}
