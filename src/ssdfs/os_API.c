#include <stdio.h>
#include <string.h>
#include "./os_API.h"

/* - Monta el disco virtual
 * - Establece como variable global el archivo .bin correspondiente al disco
 * - Define como lı́mite de ciclos P/E al valor de life.
 * La función debe poder ser llamada múltiples veces si se desea abrir
 * diferentes discos a lo largo de la ejecución de main.c.*/
void os_mount(char* diskname, unsigned life) {
    strcpy(global_diskname, diskname);
    global_P_E = life;
}

void os_bitmap(unsigned num) {
    printf("BITMAP P1!\n");
    FILE *f = fopen(global_diskname, "rb");

    unsigned char buffer[256];
    fread(buffer, sizeof(buffer),1,f);

    if (num == 0) {
        int fill = 0;
        int free = 0;

        for (int i = 0; i<256; i++) {
            for (int j = 7; j >= 0; j--) {
                int bit = (buffer[i] & (1 << j)) >> j;
                printf("%d", bit );

                if (bit) {
                    fill++;

                } else {
                    free++;
                }
            }
        }

        printf("\n");
        printf("Bloques Ocupados: %d\nBloques Libres: %d\n", fill, free);

    } else if (num>=0 && num<=2048) {
        printf("%d\n", num/8);
        printf("%d\n", num%8);
        printf("%d\n", (buffer[num/8] & (1 << (7-num%7)) >> num%8 ));

    } else {
        printf("%s\n", "SEGFAULT");
    }
    
    fclose(f);
}

void os_lifemap(int lower, int upper) {  // TODO: Pendiente
    return;
}

int os_trim(unsigned limit) {  // TODO: Pendiente
    return 0;
}

void os_tree() {  // TODO: Pendiente
    return;
}


// Funciones de manejo de archivos
int os_exists(char* filename) {  // TODO: Pendiente
    return 0;
}

osFile* os_open(char* filename, char mode) {  // TODO: Pendiente
    return 0;
}

int os_read(osFile* file_desc, void* buffer, int nbytes) {  // TODO: Pendiente
    return 0;
}

int os_write(osFile* file_desc, void* buffer, int nbytes) {  // TODO: Pendiente
    return 0;
}

int os_close(osFile* file_desc) {  // TODO: Pendiente
    return 0;
}

int os_rm(char* filename) {  // TODO: Pendiente
    return 0;
}

int os_mkdir(char* path) {  // TODO: Pendiente
    return 0;
}

int os_rmdir(char* path) {  // TODO: Pendiente
    return 0;
}

int os_rmrfdir(char* path) {  // TODO: Pendiente
    return 0;
}

int os_unload(char* orig, char* dest) {  // TODO: Pendiente
    return 0;
}

int os_load(char* orig) {  // TODO: Pendiente
    return 0;
}


