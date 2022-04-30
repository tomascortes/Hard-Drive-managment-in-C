#include <stdio.h>
#include "./os_API.h"

int main(int argc, char** const argv[])
{ 
  FILE *f = fopen("simdiskfilled.bin", "rb");

  os_bitmap(0, f);

  fclose(f);

  return 0;
}
