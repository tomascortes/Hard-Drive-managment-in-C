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
        // num no pertenece al array (0..2047)
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

/* Función para imprimir el árbol de directorios y archivos del sistema, a partir del directorio base. */
void os_tree() {  // TODO: Pendiente
    return;
}


// ===== Funciones de manejo de archivos =====
/* Permite revisar si un archivo existe o no. Retorna 1 en caso de que exista, 0 de caso contrario. */
int os_exists(char* filename) {  // TODO: Pendiente
    return 0;
}

/* Esta función abre un archivo. Si mode='r', se busca el archivo filename
 * y se retorna el osFile* que lo representa. Si mode='w', se verifica que el
 * archivo no exista, y se retirna un nuevo osFile* que lo representa. */
osFile* os_open(char* filename, char mode) {  // TODO: Pendiente
    return 0;
}

/* Imprime el estado P/E de las páginas desde lower y upper-1.
 * Si ambos valores son -1, se debe imprimir el lifemap completo.
 * Además se debe imprimir en una segunda lı́nea la cantidad de bloques rotten y la
 * cantidad de bloques saludables. */
int os_read(osFile* file_desc, void* buffer, int nbytes) {  // TODO: Pendiente
    return 0;
}

/* Esta función sirve para leer archivos.
 * Lee los siguientes nbytes desde el archivo descrito por file_desc y los guarda en la dirección
 * apuntada por buffer. Debe retornar la cantidad de Bytes efectivamente leı́dos desde el archivo.
 * Esto es importante si nbytes es mayor a la cantidad de Bytes restantes en el archivo o en el
 * caso que el archivo contenga páginas rotten. La lectura de read se efectúa desde la posición
 * del archivo inmediatamente posterior a la última posición leı́da por un llamado a read */
int os_write(osFile* file_desc, void* buffer, int nbytes) {  // TODO: Pendiente
    return 0;
}

/* Esta función permite cerrar un archivo. Cierra el archivo indicado por file desc. Debe garantizar
 * que cuando esta función retorna, el archivo se encuentra actualizado en disco. */
int os_close(osFile* file_desc) {  // TODO: Pendiente
    return 0;
}

/* Esta función elimina el archivo indicado por filename. El bloque de ı́ndice del archivo debe ser
 * borrado (todos sus bits puestos en 0), lo que aumenta en 1 el contador P/E asociado a dichas páginas.
 * También se deber actualizar la página del bloque de directorio que contenı́a el puntero a dicho
 * ı́ndice, lo que también incrementa su contador P/E en 1. */
int os_rm(char* filename) {  // TODO: Pendiente
    return 0;
}

/* Esta función crea un directorio con el nombre indicado. Esto incrementa en 1 el contador P/E de
 * las páginas que sea necesario actualizar para crear las referencias a este directorio. */
int os_mkdir(char* path) {  // TODO: Pendiente
    return 0;
}

/* Esta función elimina un directorio vacı́o con el nombre indicado. Esto incrementa en 1 el contador
 * P/E de las páginas que sea necesario actualizar para borrar las referencias a este directorio. */
int os_rmdir(char* path) {  // TODO: Pendiente
    return 0;
}

/* Esta función elimina un directorio con el nombre indicado, todos sus archivos y subdirectorios
 * correspondientes. Esto incrementa en 1 el contador P/E de las páginas que sea necesario actualizar
 * para borrar las referencias a este directorio. */
int os_rmrfdir(char* path) {  // TODO: Pendiente
    return 0;
}

/* Esta función que se encarga de copiar un archivo o carpeta referenciado por orig hacia un nuevo archivo
 * o directorio de ruta dest en su computador. */
int os_unload(char* orig, char* dest) {  // TODO: Pendiente
    return 0;
}

/* Esta función que se encarga de copiar un archivo o los contenidos de una carpeta, referenciado
 * por orig al disco. En caso de que un archivo sea demasiado pesado para el disco, se debe escribir
 * tanto como sea posible hasta acabar el espacio disponible. En caso de que el sea una carpeta,
 * se deben copiar los archivos que estén dentro de esta carpeta, ignorando cualquier carpeta
 * adicional que tenga. Esta función debe actualizar el lifemap según corresponda. */
int os_load(char* orig) {  // TODO: Pendiente
    return 0;
}


