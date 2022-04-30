#include <stdio.h>

int main(int argc, char** const argv[])
{
  printf("BITMAP P1!\n");
  
  unsigned char buffer[256];
  FILE *f = fopen("simdiskfilled.bin", "rb");

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
