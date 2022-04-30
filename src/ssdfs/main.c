#include <stdio.h>
#include "os_API.h"




int main(int argc, char** const argv[])
{
  printf("BITMAP P1!\n");
  os_mount(argv[1], 5000);
  unsigned char buffer[256];
  FILE *f = fopen(global_diskname, "rb");

  fread(buffer, sizeof(buffer),1,f);

  printf("\n");
  for(int i = 0; i<256; i++){
    for (int j = 7; j >= 0; j--){
      printf("%d", (buffer[i] & (1 << j)) >> j );
    }
  }
  printf("\n");
  /* TODO else read failed, empty file?? */

  fclose(f);
  return 0;
}
