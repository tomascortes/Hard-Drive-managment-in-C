/* +===================================+=================================+
 * |     P01 IIC2333      |    DCCegmentation Fault   |    2022-05-04    |
 * +==================+================+===================+=============+
 * |      Nombre      |  Usr. GitHub   |      Correo       | Nro. Alumno |
 * +==================+================+===================+=============+
 * | Matías López     | MatLop1        | milopez8@uc.cl    | 17210674    |
 * +------------------+----------------+-------------------+-------------+
 * | Tomás Cortés     | ticortes       | ticortez@uc.cl    | 17640849    |
 * +------------------+----------------+-------------------+-------------+
 * | Felipe Villagrán | BibarelUsedFly | fivillagran@uc.cl | 16638689    |
 * +------------------+----------------+-------------------+-------------+
 * | Marcelo Bernal   | msbernal       | msbernal@uc.cl    | 1763671J    |
 * +------------------+----------------+-------------------+-------------+
 * | Luis González    | ljgonzalez1    | ljgonzalez@uc.cl  | 16625439    |
 * +------------------+----------------+-------------------+-------------+ */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./os_API.h"
#include "./debug/debug.h"

// ===== API de ssdfs =====


// ----- Funciones generales -----

/* Monta el disco virtual
 * Establece como variable global el archivo .bin correspondiente al disco. Define como
 * lı́mite de ciclos P/E al valor de life. La función debe poder ser llamada múltiples
 * veces si se desea abrir diferentes discos a lo largo de la ejecución de main.c.*/
void os_mount(char* diskname, unsigned life) {
    /* Crea una variable global con el nombre del archivo y otra con el
     * valor de life */
    strcpy(global_diskname, diskname);
    //// FIXME: "Narrowing conversion from 'unsigned int' to signed type 'int'
    ////  is implementation-defined"
    ////  --------------------------------------------------------
    ////  Tal vez algún check o casteo lo arregla?
    global_P_E = life;
    unactualized_change = 0;
}

/* Imprime el valor del bitmap para el bloque num.
 * Si num=0 se debe imprimir t */
void os_bitmap(unsigned num) {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // El disco tiene 2048 bloques, por lo que para el bitmap necesitamos
    // 2048 bits = 256 bytes
    unsigned char buffer[256]; // Buffer para guardar los bytes
    fread(buffer, sizeof(buffer), 1, f);

    if (num == 0) {
        printf("\nBitmap del Disco\n");

        int fill = 0;
        int free = 0;

        for (int i = 0; i < 256; i++) {
            for (int j = 7; j >= 0; j--) {
                int bit = (buffer[i] & (1 << j)) >> j; // Shift left para sacar el bit
                printf("%d", bit );
                bit ? fill++ : free++; // Se ve más cool así
            }
        }

        printf("\nBloques Ocupados: %d\nBloques Libres: %d\n", fill, free);

    } else if (num > 0 && num < 2048) {
        printf("\nBitmap Bloque N°%d\n", num);
        // num / 8 es el byte donde se encuentra el bit deseado
        // num % 8 es el offset del bit dentro de ese byte
        printf("%d\n", (buffer[num / 8] & 1 << (7 - num % 8)) >> (7 - num % 8));

        // En el momento 15:35 de la cápsula P1 dice que esto hay que entregarlo
        // aunque el argumento no sea 0
        int fill = 0;
        int free = 0;

        for (int i = 0; i < 256; i++) {
            for (int j = 7; j >= 0; j--) {
                int bit = (buffer[i] & (1 << j)) >> j; // Shift left para sacar el bit
                bit ? fill++ : free++; // Se ve más cool así
            }
        }

        printf("Bloques Ocupados: %d\nBloques Libres: %d\n", fill, free);

    } else {
        printf("\nBitmap Bloque N°%d\n", num);
        printf("%s\n", "SEGFAULT uwu");
    }

    fclose(f); // Evitamos leaks
}

/* Imprime el estado P/E de las páginas desde lower y upper-1. Si ambos valores son -1,
 * se debe imprimir el lifemap completo. Además se debe imprimir en una segunda lı́nea la
 * cantidad de bloques rotten y la cantidad de bloques saludables. */
void os_lifemap(int lower, int upper) {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 1 MiB, para llegar al bloque N°1, de directorio.
    fseek(f, 1 * BLOCK_SIZE, SEEK_SET);

    if (upper > PAGES_PER_DISK || lower < -1 || lower > PAGES_PER_DISK || upper < -2 ) {
        printf("Error de input para os_lifemap\n");
        return;
    }

    if (lower  == -1 && upper == -1) {
        upper = PAGES_PER_DISK;
        lower = 0;
    }

    int rotten_blocks = 0;
    int total_blocks = 0;
    int rotten_found = 0;
    int block_visited = 0;
    // Son 524288 paginas entre los 2 planos, por lo que recorremos 524288 numeros
    // Son 4096 bloques en el disco
    for (int i = 0; i < PAGES_PER_DISK; i++) {
        int buffer; // see leen ints de 4 bytes
        fread(&buffer, sizeof(int), 1, f); // Leo una entrada de un int

        if ( lower < i && i < upper) {
            printf(" %d", buffer);
            block_visited = 1;
        }

        if (i % 256 == 0 && block_visited == 1){
          // Se suman las condiciones de bloque visitado
            rotten_blocks += rotten_found;
            total_blocks++;
            rotten_found = 0;
            block_visited = 0;
        }
        if (buffer == -1) {
            rotten_found = 1;
        }
    }
    printf("\nCantidad de bloques rotten: %d", rotten_blocks);
    printf("\nCantidad de bloques sanos: %d\n", total_blocks - rotten_blocks);
    fclose(f); // Evitamos leaks
}

/* Esta función debe recorrer el disco completo. Para cada bloque que contenga páginas
 * cuyo valor P/E se encuentra a limit ciclos de pasar a estado rotten, reubicarla a un
 * bloque que no contenga páginas en esta condición. Esta operación no debe corromper
 * archivos ni directorios, por lo que mover un bloque implica actualizar todos los
 * punteros que sea necesario para no perder su referencia. En caso de que no hayan
 * suficientes bloques disponibles para realizar cualquiera de estas operaciones, se debe
 * indicar la cantidad de estos, y además indicar que archivos o directorios se podrı́an
 * ver afectados por pérdida de información en limit ciclos. Esta función retorna el
 * número bloques que fueron reubicados exitosamente. */
int os_trim(unsigned limit) {  // TODO: Pendiente
    return 0;
}

/* Función para imprimir el árbol de directorios y archivos del sistema, a partir del
 * directorio base. */
void os_tree(){
    // Defino la verión recursiva de la función acá adentro
    // para cumplir con las reglas de no ofrecer más funciones en la API
    //// FIXME: Me tira error.
    ////  "Function definition is not allowed here"
    ////  No se debería definir una función dentro de otra.
    ////  --------------------------------------------------------
    ////  Tal vez sirva definirla en otro lado. Está el paquete, librería o como se llame
    ////  en C, ./aux/auxiliary_fx. Tal vez poner esto ahí sea conveniente.
    void directree(int directory_block, int depth) {
        FILE* f2 = fopen(global_diskname, "rb");
        fseek(f2, directory_block * BLOCK_SIZE, SEEK_SET);
        // Cada bloque tiene 1048576 bytes
        
        // Son 32768 entradas en un bloque de directorio
        for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
            unsigned char buffer[DIR_ENTRY_SIZE]; // Buffer para guardar los bytes de una entrada
            fread(buffer, sizeof(buffer), 1, f2); // Leo una entrada

            if(buffer[0] == 3) { // archivo:
                for (int k = 0; k < depth; k++){
                    printf("| ");
                }

                for (int j = 5; j < DIR_ENTRY_SIZE; j++) {
                    printf("%c", buffer[j]);
                }

                printf("\n");
            }

            else if(buffer[0] == 1) { // Directorio
                for (int k = 0; k < depth; k++){
                    printf("| ");
                }

                for (int j = 5; j < DIR_ENTRY_SIZE; j++) {
                    printf("%c", buffer[j]);
                }

                printf("\n");
                depth++; // Subo la profundidad en 1
                int puntero = buffer[1];
                //// FIXME: Me tira error.
                ////  Hace referencia a una función que marca como indefinida.
                ////  --------------------------------------------------------
                ////  Supongo que no definir una función dentro de otra solucionaría el
                ////  problema
                directree(puntero, depth); // Llamada recursiva
                depth--; // Vuelvo a la profundidad anterior
            }
        }

        fclose(f2); // Evitamos leaks
    }

    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(f, 3 * BLOCK_SIZE, SEEK_SET);

    printf("~\n"); // root
    int depth = 1; // Para cachar que tan profundo estoy

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE];
        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada

        if (buffer[0] == 1) { // directorio:
            for (int k = 0; k < depth; k++) { // Desplazar depth a la derecha
                printf("| ");
            }
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del directorio
                printf("%c", buffer[j]);
            }

            printf("\n");
            int puntero = buffer[1]; // Pesco los bytes 1-4
            depth++; // Subo la profundidad en 1
            //// FIXME: Me tira error.
            ////  Hace referencia a una función que marca como indefinida.
            ////  --------------------------------------------------------
            ////  Supongo que no definir una función dentro de otra solucionaría el
            ////  problema
            directree(puntero, depth); // Función recursiva para leer
                                          // dentro del directorio
            depth--; // Vuelvo a la profundidad anterior
        } 
        
        else if (buffer[0] == 3) { // archivo:
            for (int k = 0; k < depth; k++) {
                printf("| ");
            }
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del archivo
                printf("%c", buffer[j]);
            }

            printf("\n");
        }
    }

    fclose(f); // Evitamos leaks
}

// ----- Funciones de manejo de archivos -----
/* Permite revisar si un archivo existe o no. Retorna 1 en caso de que exista, 0 de caso
 * contrario. */
int os_exists(char* filename) {  // TODO: Pendiente
    // Defino la verión recursiva de la función acá adentro
    // para cumplir con las reglas de no ofrecer más funciones en la API
    //// FIXME: Me tira error.
    ////  "Function definition is not allowed here"
    ////  No se debería definir una función dentro de otra.
    ////  --------------------------------------------------------
    ////  Tal vez sirva definirla en otro lado. Está el paquete, librería o como se llame
    ////  en C, ./aux/auxiliary_fx. Tal vez poner esto ahí sea conveniente.
    ////  --------------------------------------------------------
    ////  Además es casi lo mismo que lo que está arriba.
    ////  Estoy seguro que se puede hacer de tal forma que resulte los siguiente
    ////  os_tree   -> directree_general -> directree_solo_diferencias
    ////  os_exists -> directree_general -> directreen_solon_diferenciasn
    ////  Además califica al tiro como "code smell" por el código repetido.
    int directreen(int directory_block, char* filename, char* path) {
        FILE* f2 = fopen(global_diskname, "rb");
        fseek(f2, directory_block * BLOCK_SIZE, SEEK_SET);
        // Cada bloque tiene 1048576 bytes

        // Son 32768 entradas en un bloque de directorio
        for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
            unsigned char buffer[DIR_ENTRY_SIZE]; // Buffer para guardar los bytes de una entrada
            fread(buffer, sizeof(buffer), 1, f2); // Leo una entrada

            if(buffer[0] == 3) { // archivo:
                char path2[100]; // path actual
                char aux[2]; // variable para concatenar char
                strcpy(path2, path); // Copiar strings
                for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del archivo
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path2, aux); // Concatenar char
                }
                printf("Path: %s\n", path2);
                if (strcmp(path2, filename) == 0) { // compara con filename
                    fclose(f2); // Evitamos leaks
                    return 1;
                }
            }
            else if(buffer[0] == 1) { // Directorio
                char path2[100]; // path actual
                char aux[2]; // variable para concatenar char
                strcpy(path2, path); // Copiar strings
                for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del directorio
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path2, aux); // Concatenar char
                }
                strcat(path, "/"); // Concatenar nuevo directorio
                int puntero = buffer[1]; // Pesco los bytes 1-4
                //// FIXME: Me tira error.
                ////  Hace referencia a una función que marca como indefinida.
                ////  --------------------------------------------------------
                ////  Supongo que no definir una función dentro de otra solucionaría el
                ////  problema
                if (directreen(puntero, filename, path2)){// Función recursiva para leer
                    fclose(f2); // Evitamos leaks
                    return 1;
                };
            }
        }

        fclose(f2); // Evitamos leaks
        return 0;
    }

    printf("Filename: %s\n", filename);

    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(f, BLOCK_SIZE * 3, SEEK_SET);

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE];
        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada
        if(buffer[0] == 3){ // archivo:
            char path[100] = "/"; // path inicial
            char aux[2]; // variable para concatenar char
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del archivo
                aux[1] = '\0';
                aux[0] = buffer[j];
                strcat(path, aux); // Concatenar char
            }
            printf("Path: %s\n", path);
            if (strcmp(path, filename) == 0) { // compara con filename
                fclose(f); // Evitamos leaks
                printf("¡Esta!\n");
                return 1;
            }
        }
        else if (buffer[0] == 1) { // directorio:
            char path[100] = "/"; // path inicial
            char aux[2]; // variable para concatenar char
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del directorio
                aux[1] = '\0';
                aux[0] = buffer[j];
                strcat(path, aux); // Concatenar char
            }
            strcat(path, "/");
            int puntero = buffer[1]; // Pesco los bytes 1-4
            //// FIXME: Me tira error.
            ////  Hace referencia a una función que marca como indefinida.
            ////  --------------------------------------------------------
            ////  Supongo que no definir una función dentro de otra solucionaría el
            ////  problema
            if (directreen(puntero, filename, path)){// Función recursiva para leer
                fclose(f); // Evitamos leaks
                printf("¡Esta!\n");
                return 1;
            };
        }
    }

    fclose(f); // Evitamos leaks
    printf("¡No Esta!\n");
    return 0;
}

/* Esta función abre un archivo. Si mode='r', se busca el archivo filename y se retorna el
 * osFile* que lo representa. Si mode='w', se verifica que el archivo no exista, y se
 * retorna un nuevo osFile* que lo representa. */
osFile* os_open(char* filename, char mode) {  // TODO: Pendiente
    // if (os_exist(...) || ! mode == "w") { ...
    osFile* file_desc = osFile_new(filename, global_diskname);
    // TODO: ...
    //osFile_set_mode(file_desc, &mode);
    //osFile_set_location(...);
    // TODO: ...
    // }
    return file_desc;
}

/* Esta función sirve para leer archivos. Lee los siguientes nbytes desde el archivo
 * descrito por file desc y los guarda en la dirección apuntada por buffer. Debe retornar
 * la cantidad de Bytes efectivamente leı́dos desde el archivo. Esto es importante si
 * nbytes es mayor a la cantidad de Bytes restantes en el archivo o en el caso que el
 * archivo contenga páginas rotten. La lectura de read se efectúa desde la posición del
 * archivo inmediatamente posterior a la última posición leı́da por un llamado a read. */
// NOTE: Asumo que los inputs cumplen las siguientes características
// TODO: Hacer que acepte números mayores a el espacio restante.
// TODO: Procesar págs. rotten.
int os_read(osFile* file_desc, void* buffer, int nbytes) {  // NOTE: Trabajando en esto
    int iter;
    int starting_pos;
    int end_pos;
    int bytes_read;

    // file_desc -->  Archivo
    // nbytes    -->  Cantidad de bytes que voy a leer
    // buffer    -->  Lugar donde guardo la info
    starting_pos = file_desc->current_pos;

    for (iter = 0; iter <= nbytes; iter++) {
        osFile_offset_pointer(file_desc, 1);
        // Lectura y escritura usando little endian
        // Lectura de páginas completas

        // NOTE: Still working on it....
    }

    end_pos = file_desc->current_pos;

    // Retorna la cantidad de bytes efectivamente leída del disco
    bytes_read = end_pos - starting_pos;

    return bytes_read;
}

/* Esta función permite escribir un archivo. Escribe en el archivo descrito por file desc
 * los nbytes que se encuentren en la dirección indicada por buffer. Retorna la cantidad
 * de Bytes escritos en el archivo. Si se produjo un error porque no pudo seguir
 * escribiendo, ya sea porque el disco se llenó, ya sea porque existen demasiadas páginas
 * rotten o porque el archivo no puede crecer más, este número puede ser menor a nbytes
 * (incluso 0). Esta función aumenta en 1 el contador P/E en el lifemap asociado a cada
 * página que se escriba. */
int os_write(osFile* file_desc, void* buffer, int nbytes) {  // TODO: WIP
    if (strcmp(file_desc->mode, "w") != 0) {
        printf("Error: El archivo debe estar en modo write.\n");
        exit(-1);
    }

    // Numero de bytes en un bloque, no se puede escribir entre bloques
    long int max_size = BLOCK_SIZE;
    if (nbytes > max_size) {
        printf("Error: no se puede escribir un archivo tan grande.\n");
        exit(-1);
    }

    return 0;
}

/* Esta función permite cerrar un archivo. Cierra el archivo indicado por file desc. Debe
 * garantizar que cuando esta función retorna, el archivo se encuentra actualizado en
 * disco.*/
int os_close(osFile* file_desc) {  // TODO: Pendiente
    if (unactualized_change == 1) {
        printf("El disco no está actualizado con los respectivos cambios");
    }

    else {
        free(file_desc);  //// XXX: Por qué se libera memoria aquí??
        osFile_destroy(file_desc);
    }

    return 0;
}

/* Esta función elimina el archivo indicado por filename. El bloque de ı́ndice del archivo
 * debe ser borrado (todos sus bits puestos en 0), lo que aumenta en 1 el contador P/E
 * asociado a dichas páginas. También se deber actualizar la página del bloque de
 * directorio que contenı́a el puntero a dicho ı́ndice, lo que también incrementa su
 * contador P/E en 1. */
int os_rm(char* filename) {  // TODO: Pendiente
    return 0;
}

/* Esta función crea un directorio con el nombre indicado. Esto incrementa en 1 el
 * contador P/E de las páginas que sea necesario actualizar para crear las referencias
 * a este directorio. */
int os_mkdir(char* path) {  // TODO: Pendiente
    return 0;
}

/* Esta función elimina un directorio vacı́o con el nombre indicado. Esto incrementa en 1
 * el contador P/E de las páginas que sea necesario actualizar para borrar las referencias
 * a este directorio. */
int os_rmdir(char* path) {  // TODO: Pendiente
    return 0;
}

/* Esta función elimina un directorio con el nombre indicado, todos sus archivos y
 * subdirectorios correspondientes. Esto incrementa en 1 el contador P/E de las páginas
 * que sea necesario actualizar para borrar las referencias a este directorio. */
int os_rmrfdir(char* path) {  // TODO: Pendiente
    return 0;
}

/* Esta función que se encarga de copiar un archivo o carpeta referenciado por orig hacia
 * un nuevo archivo o directorio de ruta dest en su computador. */
int os_unload(char* orig, char* dest) {  // TODO: Pendiente
    return 0;
}

/* Esta función que se encarga de copiar un archivo o los contenidos de una carpeta,
 * referenciado por orig al disco. En caso de que un archivo sea demasiado pesado para el
 * disco, se debe escribir tanto como sea posible hasta acabar el espacio disponible.
 * En caso de que el sea una carpeta, se deben copiar los archivos que estén dentro de
 * esta carpeta, ignorando cualquier carpeta adicional que tenga. Esta función debe
 * actualizar el lifemap según corresponda. */
int os_load(char* orig) {  // TODO: Pendiente
    return 0;
}

// Temporal ----- Esta función es para testear
// Tira los nombres de todo lo que hay en el disco
void print_names() {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, del directorio base.
    int offset = 3 * BLOCK_SIZE; // 3MiB
    fseek(f, offset, SEEK_SET);

    // root está en el bloque 3 por convención, por lo que si
    // hubiese que moverlo para que no se pudra, se perdería para siempre

    // (La entrada 1672 tiene un archivo)
    // Son 32768 entradas en el bloque de directorio
    int max_buffer_size = 32;
    int entries = max_buffer_size * 1024;

    for (int i = 0; i < entries; i++) {
        unsigned char buffer[max_buffer_size]; // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada

        if (buffer[0]) { // Si hay archivo o directorio:
            printf("Primer byte entrada %i: %i\n", i, buffer[0]);

            // Printear nombre del archivo
            for (int j = 5; j < 32; j++) {
                printf("%c", buffer[j]);
            }

            printf("\n");
        }
    }
    fclose(f); // Evitamos leaks
}

// Prints bits of int
/*for (int j = 31; j >= 0; j--) {
    int bit = (puntero & (1 << j)) >> j; // Shift left para sacar el bit
    printf("%d", bit);
}*/
