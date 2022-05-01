#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./os_API.h"

void os_mount(char* diskname, unsigned life){
    /* Crea una variable global con el nombre del archivo y otra con el
       valor de life */
    strcpy(global_diskname, diskname);
    global_P_E = life;
}

void os_bitmap(unsigned num){
  printf("BITMAP P1!\n");

  // Abro el archivo
  FILE *f = fopen(global_diskname, "rb");

  // El disco tiene 2048 bloques, por lo que para el bitmap necesitamos
  // 2048 bits = 256 bytes
  unsigned char buffer[256]; // Buffer para guardar los bytes
  fread(buffer, sizeof(buffer), 1, f);

  if(num == 0){
    int fill=0;
    int free=0;
    for(int i = 0; i < 256; i++){
      for (int j = 7; j >= 0; j--){
        int bit = (buffer[i] & (1 << j)) >> j; // Shift left para sacar el bit
        printf("%d", bit );
        bit ? fill++ : free++; // Se ve más cool así
      }
    }
    printf("\n");
    printf("Bloques Ocupados: %d\nBloques Libres: %d\n\n", fill, free);
  } else if(num > 0 && num <= 2048){
    // num/8 es el byte donde se encuentra el bit deseado
    // num%8 es el offset del bit dentro de ese byte
    printf("%d\n", (buffer[num/8] & 1 << (7-num%8)) >> (7-num%8));

    // En el momento 15:35 de la cápsula P1 dice que esto hay que entregarlo
    // aunque el argumento no sea 0
    int fill=0;
    int free=0;
    for(int i = 0; i < 256; i++){
      for (int j = 7; j >= 0; j--){
        int bit = (buffer[i] & (1 << j)) >> j; // Shift left para sacar el bit
        bit ? fill++ : free++; // Se ve más cool así
      }
    }
    printf("Bloques Ocupados: %d\nBloques Libres: %d\n\n", fill, free);
  } else {
    printf("%s\n\n", "SEGFAULT uwu");
  }
  
  fclose(f);
}
