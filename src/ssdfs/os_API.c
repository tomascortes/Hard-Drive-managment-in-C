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
  // Abro el archivo
  FILE *f = fopen(global_diskname, "rb");

  // El disco tiene 2048 bloques, por lo que para el bitmap necesitamos
  // 2048 bits = 256 bytes
  unsigned char buffer[256]; // Buffer para guardar los bytes
  fread(buffer, sizeof(buffer), 1, f);

  if(num == 0){
    printf("Bitmap del Disco\n");
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
  } else if(num > 0 && num < 2048){
    printf("Bitmap Bloque N°%d\n", num);
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
    printf("Bitmap Bloque N°%d\n", num);
    printf("%s\n\n", "SEGFAULT uwu");
  }
  
  fclose(f); // Evitamos leaks
}

void os_tree(){
  // Abro el archivo
  FILE *f = fopen(global_diskname, "rb");

  // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
  fseek(f, 3145728, SEEK_SET);

  // La entrada 1672 tiene un archivo
  // Son 32768 entradas en el bloque de directorio
  for(int i = 0; i < 32768; i++){
    unsigned char buffer[32]; // Buffer para guardar los bytes de una entrada
    fread(buffer, sizeof(buffer), 1, f); // Leo una entrada
    if(buffer[0] == 1){ // directorio:
      printf("Primer byte entrada %i: %i\n", i, buffer[0]);
      // Printear nombre del archivo
      for(int j = 5; j < 32; j++){
        printf("%c", buffer[j]);
      }
      printf("\n");
      int puntero = buffer[1]; // Pesco los bytes 1-4
      printf("Puntero: %i\n", puntero);
      // Pendiente
      //fseek(f, puntero*4096, SEEK_SET); // Cada pág tiene 4096 bytes


    } else if(buffer[0] == 3){ // archivo:
      printf("Primer byte entrada %i: %i\n", i, buffer[0]);
      // Printear nombre del archivo
      for(int j = 5; j < 32; j++){
        printf("%c", buffer[j]);
      }
      printf("\n");
    }
  }


  fclose(f); // Evitamos leaks
}

// Temporal ----- Esta función es para testear
// Tira los nombres de todo lo que hay en el disco
void print_names(){
  // Abro el archivo
  FILE *f = fopen(global_diskname, "rb");

  // Me muevo 3 MiB, para llegar al bloque N°3, del directorio base.
  fseek(f, 3145728, SEEK_SET);

  // root está en el bloque 3 por convención, por lo que si 
  // hubiese que moverlo para que no se pudra, se perdería para siempre

  // (La entrada 1672 tiene un archivo)
  // Son 32768 entradas en el bloque de directorio
  for(int i = 0; i < 32768; i++){
    unsigned char buffer[32]; // Buffer para guardar los bytes de una entrada
    fread(buffer, sizeof(buffer), 1, f); // Leo una entrada
    if(buffer[0]){ // Si hay archivo o directorio:
      printf("Primer byte entrada %i: %i\n", i, buffer[0]);
      // Printear nombre del archivo
      for(int j = 5; j < 32; j++){
        printf("%c", buffer[j]);
      }
      printf("\n"); 
    }
  }
  fclose(f); // Evitamos leaks
}