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

/* Para funciónes auxiliares */
#include "auxiliary_fx.h"

// Calcula offsets en base a plano, bloque, página, celta y byte
// NOTE: El plano 0 es el primer plano
//  El bloque 0 es el primer bloque
//  ...
long int calc_offset(int plane, int block, int page, int cell, int bytes) {
    long int offset = 0;

    offset += bytes;
    offset += cell * CELL_SIZE;
    offset += page * PAGE_SIZE;
    offset += block * BLOCK_SIZE;
    offset += plane * PLANE_SIZE;

    return offset;
}

int is_page_rotten(int page, char* diskname) {
    // Abro el archivo
    FILE* file = fopen(diskname, "rb");
    int buffer;

    fseek(file,  BLOCK_SIZE + (page * sizeof(int)), SEEK_SET);

    // Leo una entrada de un int
    fread(&buffer, sizeof(int), 1, file);

    if (buffer == -1) {
        return 0;

    } else {
        return 1;
    }
}

/* Esta función recibe un path que ya sé que existe y me tira el bloque
   en el que está ubicado el directorio */
// 1° llamada -> bloque_final = 3
// 1° llamada -> path_parcial = "~/" de largo 100
int pathfinder(char* path, int bloque_final, char* path_parcial, char* diskname) {
    printf("llamada a la función\t");
    // Abro el archivo
    FILE* disk_file = fopen(diskname, "rb");
    
    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(disk_file, BLOCK_SIZE * bloque_final, SEEK_SET);

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE];
        char path_parcial_r[100];

        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, disk_file); // Leo una entrada
        strcpy(path_parcial_r, path_parcial);

        if (buffer[0] == 1){ // archivo o directorio:
            char aux[2]; // variable para concatenar char

            for (int j = 5; j < DIR_ENTRY_SIZE; j++){
                aux[1] = '\0';
                // WARN: Narrowing conversion from 'unsigned char' to signed type 'char' is implementation-defined
                aux[0] = buffer[j];
                strcat(path_parcial_r, aux); // Concatenar char
            } // Debuggear acá
            
            printf("Bloque final antes: %i\t", bloque_final);
            bloque_final = *(int*) (buffer + 1);
            printf("Bloque final después: %i\t", bloque_final);
            printf("Path parcial: %s\n", path_parcial_r);

            if (strcmp(path_parcial_r, path) == 0) {
                fclose(disk_file); // Evitamos leaks
                return bloque_final;
            }

            strcat(path_parcial_r, "/");
            printf("Path parcial hasta acá: %s\n", path_parcial);
            // Recursión
            fclose(disk_file); // Què pasarà si saco esto?
            printf("%s, %i, %s\n", path, bloque_final, path_parcial_r);
            
            if(strstr(path, path_parcial_r)) { // Solo si es substr
                bloque_final = pathfinder(path, bloque_final,
                                          path_parcial_r, diskname);

                if (bloque_final) {
                    fclose(disk_file);
                    return bloque_final;
                }
            }
        }
    }

    fclose(disk_file); // Evitamos leaks
    return 0; // 0 es que no se encontró
}

    

// Defino la función recursiva de la función acá adentro
// para cumplir con las reglas de no ofrecer más funciones en la API
int find_file(int directory_block, char* filename, char* path, char* diskname) {
    FILE* disk_file = fopen(diskname, "rb");
    fseek(disk_file, directory_block * BLOCK_SIZE, SEEK_SET);
    
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        // Buffer para guardar los bytes de una entrada
        unsigned char buffer[DIR_ENTRY_SIZE];

        // Leo una entrada
        fread(buffer, sizeof(buffer), 1, disk_file);
        
        if(buffer[0] == 3) { // archivo:
            char current_path[100]; // path actual
            char aux[2]; // variable para concatenar char
            strcpy(current_path, path); // Copiar strings
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del archivo
                
                aux[1] = '\0';
                // WARN: Narrowing conversion from 'unsigned char' to signed type 'char' is implementation-
                aux[0] = buffer[j];

                // Concatenar char
                strcat(current_path, aux);
            }

            // compara con filename
            if (strcmp(current_path, filename) == 0) {
                fclose(disk_file);
                return 1;
            }

        // Directorio
        } else if(buffer[0] == 1) {
            char path2[100]; // path actual
            char aux[2]; // variable para concatenar char
            int puntero = *(int*) buffer + 1;
            
            strcpy(path2, path); // Copiar strings
            
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) {
                // Printear nombre del directorio
                aux[1] = '\0';
                // WARN: Narrowing conversion from 'unsigned char' to signed type 'char' is implementation-defined
                aux[0] = buffer[j];
                strcat(path2, aux); // Concatenar char
            }
            
            strcat(path, "/"); // Concatenar nuevo directorio

            // Función recursiva para leer
            if (find_file(puntero, filename, path2)) {
                fclose(disk_file);
                return 1;
            };
        }
    }
    fclose(disk_file);
    return 0;
}

int dir_exists(char* dirname, char* diskname) {
    // Abro el archivo
    char dir[100];
    FILE* disk_file = fopen(diskname, "rb");

    strcpy(dir, dirname);

    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(disk_file, BLOCK_SIZE * 3, SEEK_SET);

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE];
        int *puntero;
        
        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, disk_file); // Leo una entrada

        if(buffer[0] == 1) { // directorio:
            char path[100] = "/"; // path inicial
            char aux[2]; // variable para concatenar char

            // Printear nombre del directorio
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) {
                aux[1] = '\0';
                // WARN: Narrowing conversion from 'unsigned char' to signed type 'char' is implementation-defined
                aux[0] = buffer[j];
                strcat(path, aux); // Concatenar char
            }

            strcat(path, "/");

            // NOTE: Cambié la línea a la que sigue.
            //  Casteé de unsigned int a int
            // puntero = &buffer[1];
            puntero = (int *) &buffer[1];

            if (strcmp(path, dirname) == 0) { // compara con filename
                fclose(disk_file); // Evitamos leaks
                return 1;
            }

            if (find_dir(*puntero, dir, path)) { // Función recursiva para leer
                fclose(disk_file); // Evitamos leaks
                return 1;
            }
        }
    }

    fclose(disk_file); // Evitamos leaks
    return 0;
}

int find_dir(int directory_block, char* dirname, char* path, char* diskname) {
    FILE* disk_file = fopen(diskname, "rb");
    fseek(disk_file, directory_block * BLOCK_SIZE, SEEK_SET);
    
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        // Buffer para guardar los bytes de una entrada
        unsigned char buffer[DIR_ENTRY_SIZE];
        int *pointer;

        // Leo una entrada
        fread(buffer, sizeof(buffer), 1, disk_file);

        if(buffer[0] == 1) { // Directorio
            char current_path[100]; // path actual
            char aux[2]; // variable para concatenar char

            strcpy(current_path, path); // Copiar strings

            // Printear nombre del directorio
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) {
                aux[1] = '\0';
                aux[0] = buffer[j];
                // Concatenar char
                strcat(current_path, aux);
            }

            // Concatenar nuevo directorio
            strcat(current_path, "/");
            // NOTE: Cambié la línea a la que sigue.
            //  Casteé de unsigned int a int
            // pointer = &buffer[1];
            pointer = (int *) &buffer[1];

            // compara con filename
            if (strcmp(current_path, dirname) == 0) {
                fclose(disk_file);
                return 1;
            }

            // Función recursiva para leer
            if (find_dir(*pointer, dirname, current_path)) {
                fclose(disk_file);
                return 1;
            }
        }
    }

    fclose(disk_file);
    return 0;
}

int get_index_file(int directory_block, char* filename, char* path, char* diskname) {
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
            if (strcmp(path2, filename) == 0) { // compara con filename
                fclose(f2); // Evitamos leaks
                int *puntero_indice;
                puntero_indice = &buffer[1]; // Pesco los bytes 1-4 en adelante
                return *puntero_indice; // Retorno puntero al bloque indice del archivo
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
            int *puntero;
            puntero = &buffer[1];
            if (get_index_file(*puntero, filename, path2)){// Función recursiva para leer
                fclose(f2); // Evitamos leaks
                return 1;
            };
        }
    }

    fclose(f2); // Evitamos leaks
    return 0;
}

int get_index_pointer_and_length(char* filename, char* diskname) {
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
            if (strcmp(path, filename) == 0) { // compara con filename
                fclose(f); // Evitamos leaks
                int *puntero_indice;
                puntero_indice = &buffer[1]; // Pesco los bytes 1-4 en adelante
                return *puntero_indice; // Retorno puntero al bloque indice del archivo
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
            int puntero_index =  get_index_file(puntero, filename, path);
            if (puntero_index){// Función recursiva para leer
                fclose(f); // Evitamos leaks
                return puntero_index;
            };
        }
    }

    fclose(f); // Evitamos leaks
    return 0;
}

// Defino la verión recursiva de la función acá adentro
// para cumplir con las reglas de no ofrecer más funciones en la API
void directree(int directory_block, int depth, char* diskname) {
    //NOTE: Consejo: Poner un maxdepth para que no haga stack overflow si se sale de control
    FILE* f2 = fopen(diskname, "rb");
    fseek(f2, directory_block * BLOCK_SIZE, SEEK_SET);
    // Cada bloque tiene 1048576 bytes

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE]; // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f2); // Leo una entrada
        
        if (buffer[0] == 1) { // Directorio
            for (int k = 0; k < depth; k++) { // Desplazar depth a la derecha
                printf("| ");
            }
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del directorio
                printf("%c", buffer[j]);
            }
            printf("\n");
            int puntero = *(int*) buffer + 1;
            depth++; // Subo la profundidad en 1
            directree(puntero, depth, global_diskname); // Llamada recursiva
            depth--; // Vuelvo a la profundidad anterior
        }

        else if (buffer[0] == 3) { // archivo:
            for (int k = 0; k < depth; k++) {
                printf("| ");
            }
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del archivo
                if (buffer[j] == 0){
                    break;
                } else {
                    printf("%c", buffer[j]);
                }
            }
            printf("\n");
        }
    }

    fclose(f2); // Evitamos leaks
}


bool is_block_rotten(int block, char* diskname){
    FILE *f = fopen(global_diskname, "rb");

    fseek(f, 1 * BLOCK_SIZE, SEEK_SET);

    if (block > 2048 || block < 0 ) {
        printf("Error de input para usefull_lifemap\n");
        return true;
    }

    int buffer; // see leen ints de 4 bytes
    for (int i = 256*block; i < 256*(block + 1); i++) {
        fread(&buffer, sizeof(int), 1, f); // Leo una entrada de un int
        if (buffer == -1) {
            return true;
        }
    }
    fclose(f); // Evitamos leaks
    return false;
}

bool is_block_available(unsigned num, char* diskname) {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // El disco tiene 2048 bloques, por lo que para el bitmap necesitamos
    // 2048 bits = 256 bytes
    unsigned char buffer[256]; // Buffer para guardar los bytes
    fread(buffer, sizeof(buffer), 1, f);

    if (num > 0 && num < 2048) {
        // num / 8 es el byte donde se encuentra el bit deseado
        // num % 8 es el offset del bit dentro de ese byte
        if (1 == (buffer[num / 8] & 1 << (7 - num % 8)) >> (7 - num % 8)){
            return false;
        }
        else{
            return true;
        }

    } else {
        printf("\nBitmap Bloque N°%d\n", num);
        printf("%s\n", "SEGFAULT uwu");
    }

    fclose(f); // Evitamos leaks
}

// Función auxiliar que busca el primer bloque vacío
int blocksearch(char* diskname) {
    // Cargo el bitmap
    FILE *f = fopen(diskname, "rb");
    unsigned char buffer[256];
    int bloque = 0;

    fread(buffer, sizeof(buffer), 1, f);

    for(int i = 0; i < 256; i++) {
        for (int j = 7; j >= 0; j--) {
            // Shift left para sacar el bit
            int bit = (buffer[i] & (1 << j)) >> j;

            // Si el bit es 1 sigo buscando, si no, retorno
            if(bit) {
                bloque++;
            } else {
                fclose(f);
                return bloque;
            }
        }
    }

    fclose(f);
    return 0; // Si no hay bloques disponibles
}
