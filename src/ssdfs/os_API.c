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

// ===== API de ssdfs =====


// ----- Funciones generales -----

/* Monta el disco virtual
 * Establece como variable global el archivo .bin correspondiente al disco
 * Define como lı́mite de ciclos P/E al valor de life.
 * La función debe poder ser llamada múltiples veces si se desea abrir
 * diferentes discos a lo largo de la ejecución de main.c.*/
void os_mount(char* diskname, unsigned life) {
    /* Crea una variable global con el nombre del archivo y otra con el
     * valor de life */
    strcpy(global_diskname, diskname);
    // FIXME: "Narrowing conversion from 'unsigned int' to signed type 'int' is implementation-defined"
    //  Tal vez algún check o casteo lo arregla?
    global_P_E = life;
}

/* - imprime el valor del bitmap para el bloque num.
 * Si num=0 se debe imprimir t */
void os_bitmap(unsigned num){
  // Abro el archivo
  FILE *f = fopen(global_diskname, "rb");

  // El disco tiene 2048 bloques, por lo que para el bitmap necesitamos
  // 2048 bits = 256 bytes
  unsigned char buffer[256]; // Buffer para guardar los bytes
  fread(buffer, sizeof(buffer), 1, f);

  if(num == 0){
    printf("\nBitmap del Disco\n");
    int fill=0;
    int free=0;
    for(int i = 0; i < 256; i++){
      for (int j = 7; j >= 0; j--){
        int bit = (buffer[i] & (1 << j)) >> j; // Shift left para sacar el bit
        printf("%d", bit );
        bit ? fill++ : free++; // Se ve más cool así
      }
    }
    printf("\nBloques Ocupados: %d\nBloques Libres: %d\n", fill, free);
  } else if(num > 0 && num < 2048){
    printf("\nBitmap Bloque N°%d\n", num);
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
    printf("Bloques Ocupados: %d\nBloques Libres: %d\n", fill, free);
  } else {
    printf("\nBitmap Bloque N°%d\n", num);
    printf("%s\n", "SEGFAULT uwu");
  }
  
  fclose(f); // Evitamos leaks
}

/* Imprime el estado P/E de las páginas desde lower y upper-1.
 * Si ambos valores son -1, se debe imprimir el lifemap completo.
 * Además se debe imprimir en una segunda lı́nea la cantidad de bloques rotten y la cantidad de bloques saludables. */
void os_lifemap(int lower, int upper) { 
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");
    // Me muevo 1 MiB, para llegar al bloque N°1, de directorio.
    fseek(f, 1048576, SEEK_SET);

    if (upper > 524288 || lower < -1 || lower > 524288 || upper < -2 ){
      printf("Error de input para os_lifemap\n");
      return;
    }
    if (lower  == -1 && upper == -1){
      upper = 524288;
      lower = 0;
    }

    int rotten_blocks = 0;
    int total_blocks = 0;
    int rotten_found = 0;
    int block_visited = 0;
    // Son 524288  paginas entre los 2 planos, por lo que recorremos 524288 numeros
    // Son 4096 bloques en el disco
    for (int i = 0; i < 524288; i++) {
        int buffer; // see leen ints de 4 bytes
        fread(&buffer, sizeof(int), 1, f); // Leo una entrada de un int

        if ( lower < i && i < upper){
          printf(" %d",buffer);
          block_visited = 1;
        }
        if (i%256 == 0 && block_visited == 1){
          // Se suman las condiciones de bloque visitado
          rotten_blocks += rotten_found;
          total_blocks ++;
          rotten_found = 0;
          block_visited = 0;
        }
        if (buffer == -1){
          rotten_found = 1;
        }
    }
    printf("\nCantidad de bloques rotten: %d", rotten_blocks);
    printf("\nCantidad de bloques sanos: %d\n", total_blocks - rotten_blocks);
    fclose(f); // Evitamos leaks
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

/* Función para imprimir el árbol de directorios y archivos del sistema, 
   a partir del directorio base. */
void os_tree(){

    // Defino la verión recursiva de la función acá adentro
    // para cumplir con las reglas de no ofrecer más funciones en la API
    void directree(int directory_block, int depth){
        FILE* f2 = fopen(global_diskname, "rb");
        fseek(f2, directory_block*1048576, SEEK_SET); 
        // Cada bloque tiene 1048576 bytes
        
        // Son 32768 entradas en un bloque de directorio
        for (int i = 0; i < 32768; i++) {
            unsigned char buffer[32]; // Buffer para guardar los bytes de una entrada
            fread(buffer, sizeof(buffer), 1, f2); // Leo una entrada
            if(buffer[0] == 3){ // archivo:
                for (int k = 0; k < depth; k++){
                    printf("| ");
                }
                for (int j = 5; j < 32; j++) {
                    printf("%c", buffer[j]);
                }
                printf("\n");
            } 
            
            else if(buffer[0] == 1){ // Directorio
                for (int k = 0; k < depth; k++){
                    printf("| ");
                }
                for (int j = 5; j < 32; j++) {
                    printf("%c", buffer[j]);
                }
                printf("\n");
                depth++; // Subo la profundidad en 1
                int puntero = buffer[1];
                directree(puntero, depth); // Llamada recursiva
                depth--; // Vuelvo a la profundidad anterior
            }
        }
        fclose(f2); // Evitamos leaks
    }


    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(f, 3145728, SEEK_SET);

    printf("~\n"); // root
    int depth = 1; // Para cachar que tan profundo estoy

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < 32768; i++) {
        unsigned char buffer[32]; 
        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada

        if(buffer[0] == 1){ // directorio:
            for (int k = 0; k < depth; k++){ // Desplazar depth a la derecha
                printf("| ");
            }
            for (int j = 5; j < 32; j++) { // Printear nombre del directorio
                printf("%c", buffer[j]);
            }
            printf("\n");
            int puntero = buffer[1]; // Pesco los bytes 1-4
            depth++; // Subo la profundidad en 1
            directree(puntero, depth); // Función recursiva para leer
                                          // dentro del directorio
            depth--; // Vuelvo a la profundidad anterior
        } 
        
        else if(buffer[0] == 3){ // archivo:
            for (int k = 0; k < depth; k++){
                printf("| ");
            }
            for (int j = 5; j < 32; j++) { // Printear nombre del archivo
                printf("%c", buffer[j]);
            }
            printf("\n");
        }
    }
    fclose(f); // Evitamos leaks
}




// ----- Funciones de manejo de archivos -----
/* Permite revisar si un archivo existe o no. Retorna 1 en caso de que exista, 0 de caso contrario. */
int os_exists(char* filename) {  // TODO: Pendiente
    return 0;
}

/* Esta función abre un archivo. Si mode='r', se busca el archivo filename
 * y se retorna el osFile* que lo representa. Si mode='w', se verifica que el
 * archivo no exista, y se retirna un nuevo osFile* que lo representa. */
osFile* os_open(char* filename, char mode) {  // TODO: Pendiente
    osFile* file = malloc(sizeof(osFile));
    return file;
}

/* Imprime el estado P/E de las páginas desde lower y upper-1.
 * Si ambos valores son -1, se debe imprimir el lifemap completo.
 * Además se debe imprimir en una segunda lı́nea la cantidad de bloques rotten y la
 * cantidad de bloques saludables. */
int os_read(osFile* file_desc, void* buffer, int nbytes) {  // NOTE: Trabajando en esto
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

/* Esta función crea un directorio con el nombre indicado. Esto incrementa en 1
   el contador P/E de las páginas que sea necesario actualizar 
   para crear las referencias a este directorio. */
int os_mkdir(char* path) {  // TODO: Pendiente
    // Función auxiliar que busca el primer bloque vacío
    int blocksearch(){
        // Pending
        return 0;
    }
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

// Temporal ----- Esta función es para testear
// Tira los nombres de todo lo que hay en el disco
void print_names() {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, del directorio base.
    int offset = 3 * 1024 * 1024; // 3MiB
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
            for(int j = 5; j < 32; j++){
                printf("%c", buffer[j]);
            }

            printf("\n");
        }
    }
    fclose(f); // Evitamos leaks
}

// Prints bits of int
/*for (int j = 31; j >= 0; j--){
    int bit = (puntero & (1 << j)) >> j; // Shift left para sacar el bit
    printf("%d", bit );
}*/
