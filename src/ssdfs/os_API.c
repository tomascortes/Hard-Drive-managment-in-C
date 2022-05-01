#include <stdio.h>
#include <string.h>
#include "./os_API.h"

// ===== Funciones generales =====

/* - Monta el disco virtual
 * - Establece como variable global el archivo .bin correspondiente al disco
 * - Define como lı́mite de ciclos P/E al valor de life.
 * La función debe poder ser llamada múltiples veces si se desea abrir
 * diferentes discos a lo largo de la ejecución de main.c.*/
void os_mount(char* diskname, unsigned life) {
    strcpy(global_diskname, diskname);
    global_P_E = life;
}

/* - imprime el valor del bitmap para el bloque num.
 * Si num=0 se debe imprimir t */
void os_bitmap(unsigned num) {
    printf("BITMAP P1!\n");
    FILE *f = fopen(global_diskname, "rb");

    unsigned char buffer[256];
    fread(buffer, sizeof(buffer),1,f);

    if (num == 0) {
        // Printear 't'
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
        // Printea bitmap para el bloque num.
        printf("%d\n", num/8);
        printf("%d\n", num%8);
        printf("%d\n", (buffer[num/8] & (1 << (7-num%7)) >> num%8 ));

    } else {
        // num no pertenece al array (0-2047)
        printf("%s\n", "SEGFAULT");
    }
    
    fclose(f);
}

/* Imprime el estado P/E de las páginas desde lower y upper-1.
 * Si ambos valores son -1, se debe imprimir el lifemap completo.
 * Además se debe imprimir en una segunda lı́nea la cantidad de bloques rotten y la cantidad de bloques saludables. */
void os_lifemap(int lower, int upper) {  // TODO: Pendiente
    return;
}

/* Esta función debe recorrer el disco completo.
 * Para cada bloque que contenga páginas cuyo valor P/E se encuentra a limit ciclos de pasar a estado rotten,
 * reubicarla a un bloque que no contenga páginas en esta condición.
 * Esta operación no debe corromper archivos ni directorios, por lo que mover un bloque implica
 * actualizar todos los punteros que sea necesario para no perder su referencia. En caso de que no hayan
 * suficientes bloques disponibles para realizar cualquiera de estas operaciones, se debe indicar la cantidad
 * de estos, y además indicar que archivos o directorios se podrı́an ver afectados por pérdida de información
 * en limit ciclos. Esta función retorna el número bloques que fueron reubicados exitosamente. */
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


