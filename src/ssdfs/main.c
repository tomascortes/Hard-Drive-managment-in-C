#include <stdio.h>
#define BUFSZ 16

// declaración de macros y variables globales
char global_diskname[1023];
int open_file; // Para no abrir más de 1  archivo a la vez

void os_mount(char* diskname, unsigned file) {
    strcpy(global_diskname, diskname);
    open_file = 0;
}

void os_bitmap(unsigned num){
  
}


int main(int argc, char const *argv[])
{
  printf("Hello P1!\n");
  FILE* f = fopen("simdiskfilled.bin", "rb");
  int first = fgetc(f);

    if (first != EOF)
        printf("first byte = %x\n", (unsigned)first);

    /* TODO else read failed, empty file?? */

    fclose(f);
    return 0;
  
}
