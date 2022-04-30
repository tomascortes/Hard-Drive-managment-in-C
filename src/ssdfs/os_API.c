#include <stdio.h>
#include "./os_API.h"

void os_bitmap(unsigned num, FILE* f){
  unsigned char buffer[256];

  fread(buffer, sizeof(buffer),1,f);
  if(num == 0){
    int oc=0;
    int lb=0;
    for(int i = 0; i<256; i++){
      for (int j = 7; j >= 0; j--){
        int bit = (buffer[i] & (1 << j)) >> j;
        printf("%d", bit );
        if(bit){
          oc++;
        } else {
          lb++;
        }
      }
    }
    printf("\n");
    printf("Ocupado: %d\nLibres: %d\n", oc, lb);
  } else if(num>=0 && num<=2048) {
    printf("%d\n", num/8);
    printf("%d\n", num%8);
    printf("%d\n", (buffer[num/8] & (1 << (7-num%7)) >> num%8 ));
  } else{
    printf("%s\n", "SEGFAULT");
  }
}
