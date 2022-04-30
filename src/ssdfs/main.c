#include <stdio.h>
#include "os_API.h"




int main(int argc, char** const argv[])
{
  printf("BITMAP P1!\n");
  os_mount(argv[1], 5000);
  unsigned char buffer[256];
  FILE *f = fopen(global_diskname, "rb");

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
