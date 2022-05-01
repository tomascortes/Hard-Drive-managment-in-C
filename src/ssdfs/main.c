#include <stdio.h>
#include "os_API.h"




int main(int argc, char** const argv[])
{
  printf("BITMAP P1!\n");
  os_mount(argv[1], 5000);
  

  os_bitmap(1);
  printf("\n");
  os_bitmap(2);
  printf("\n");
  os_bitmap(7);
  printf("\n");
  os_bitmap(8);


  return 0;
}
