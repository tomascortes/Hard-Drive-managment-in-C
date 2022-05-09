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

// Esta es para llamar a la otra con los parámetros iniciales
int pathfinder(char* path){
    if (strcmp(path, "~")){
        return 3;
    } else {
        return pathfinder_internal(path, 3, "~/");
    }
}

/* Esta función recibe un path y me tira el bloque
   en el que está ubicado el directorio o archivo 
   (0 si no existe)*/
// 1° llamada -> bloque_final = 3
// 1° llamada -> path_parcial = "~/" de largo 100
int pathfinder_internal(char* path, int bloque_final, char* path_parcial){
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(f, BLOCK_SIZE * bloque_final, SEEK_SET);

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE];
        
        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada
        char path_parcial_r[100]; // Para no modificar el path parcial
        strcpy(path_parcial_r, path_parcial);
        if (buffer[0]){ // archivo o directorio:
            char aux[2]; // variable para concatenar char
            for (int j = 5; j < DIR_ENTRY_SIZE; j++){
                if (buffer[j] == 0){ // No caracteres basura
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path_parcial_r, aux); // Concatenar char
                } // Debuggear acá
            }
            
            int bloque_final = *(int*) (buffer + 1);

            // Condición de término
            if (strcmp(path_parcial_r, path) == 0){
                fclose(f); // Evitamos leaks
                return bloque_final;
            }
            
            // Recursión no testeada
            if(strstr(path, path_parcial_r)){ // Solo si es substr
                fclose(f); // Què pasarà si saco esto?
                strcat(path_parcial_r, "/");
                bloque_final = pathfinder_internal(path, bloque_final, path_parcial_r);
                return bloque_final;
            }
        }
    }
    fclose(f); // Evitamos leaks
    printf("No existe ese path\n");
    return 0; // 0 es que no se encontró
}

    

// Defino la verión recursiva de la función acá adentro
// para cumplir con las reglas de no ofrecer más funciones en la API
int find_file(int directory_block, char* filename, char* path) {
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
                if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path2, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path2, aux); // Concatenar char
                }
            }
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
            if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path2, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path2, aux); // Concatenar char
                }
            }
            strcat(path, "/"); // Concatenar nuevo directorio
            int puntero = *(int*) buffer + 1;
            if (find_file(puntero, filename, path2)){// Función recursiva para leer
                fclose(f2); // Evitamos leaks
                return 1;
            };
        }
    }

    fclose(f2); // Evitamos leaks
    return 0;
}

int dir_exists(char* dirname) {
    FILE *f = fopen(global_diskname, "rb");
    char dir[100];
    strcpy(dir, dirname);
    // Abro el archivo
    if (strcmp(dir, "/") == 0 ){
        return 1;
    };

    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(f, BLOCK_SIZE * 3, SEEK_SET);

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE];
        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada
        if(buffer[0] == 1) { // directorio:
            char path[100] = "/"; // path inicial
            char aux[2]; // variable para concatenar char
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del directorio
                if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path, aux); // Concatenar char
                }
            }
            strcat(path, "/");
            int *puntero;
            puntero = &buffer[1];
            if (strcmp(path, dirname) == 0) { // compara con filename
                fclose(f); // Evitamos leaks
                return 1;
            }
            if (find_dir(*puntero, dir, path)){// Función recursiva para leer
                fclose(f); // Evitamos leaks
                return 1;
            };
        }
    }

    fclose(f); // Evitamos leaks
    return 0;
}

int find_dir(int directory_block, char* dirname, char* path) {
    FILE* f2 = fopen(global_diskname, "rb");
    fseek(f2, directory_block * BLOCK_SIZE, SEEK_SET);
    // Cada bloque tiene 1048576 bytes

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE]; // Buffer para guardar los bytes de una entrada
        
        fread(buffer, sizeof(buffer), 1, f2); // Leo una entrada

        if(buffer[0] == 1) { // Directorio
            char path2[100]; // path actual
            char aux[2]; // variable para concatenar char
            strcpy(path2, path); // Copiar strings
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del directorio
            if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path2, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path2, aux); // Concatenar char
                }
            }
            strcat(path2, "/"); // Concatenar nuevo directorio
            int *puntero;
            puntero = &buffer[1];
            if (strcmp(path2, dirname) == 0) { // compara con filename
                fclose(f2); // Evitamos leaks
                return 1;
            }
            if (find_dir(*puntero, dirname, path2)){// Función recursiva para leer
                fclose(f2); // Evitamos leaks
                return 1;
            };
        }
    }

    fclose(f2); // Evitamos leaks
    return 0;
}

int get_index_file(int directory_block, char* filename, char* path) {
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
                if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path2, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path2, aux); // Concatenar char
                }
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
                if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path2, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path2, aux); // Concatenar char
                }
            }
            strcat(path2, "/"); // Concatenar nuevo directorio
            int *puntero;
            puntero = &buffer[1];
            int puntero_index =  get_index_file(*puntero, filename, path);
            if (puntero_index){// Función recursiva para leer
                fclose(f2); // Evitamos leaks
                return puntero_index;
            };
        }
    }

    fclose(f2); // Evitamos leaks
    return 0;
}

int get_index_pointer(char* filename) {
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
                if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path, aux); // Concatenar char
                }
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
                if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path, aux); // Concatenar char
                }
            }
            strcat(path, "/");
            int* puntero = &buffer[1]; // Pesco los bytes 1-4
            int puntero_index =  get_index_file(*puntero, filename, path);
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
void directree(int directory_block, int depth, char* global_diskname) {
    //NOTE: Consejo: Poner un maxdepth para que no haga stack overflow si se sale de control
    FILE* f2 = fopen(global_diskname, "rb");
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


bool is_block_rotten(int block){
    FILE *f = fopen(global_diskname, "rb");

    fseek(f, 1 * BLOCK_SIZE, SEEK_SET);

    if (block > 2048 || block < 0 ) {
        printf("Error de input para is_block_rotten\n");
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

bool is_block_available(unsigned num) {
    // Revisa bitmap
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

int get_usable_block(){
    for (int i = 1; i < 2048; i++) {
        int index_block; // see leen ints de 4 bytes
        if (is_block_available(i)  && is_block_rotten(i) == false){
            return i;
        }
    }
    return -1;
}


// Busca en el bitmap el bit correspondiente al bloque
// y lo marca como usado (lo pone en 1)
void mark_as_used(int bloque) {
    // El bit que corresponda al bloque va a estar en el byte:
    int byte = bloque / 8;
    int offset = 7 - bloque % 8;

    // Abro el archivo
    FILE* f = fopen(global_diskname, "rb");

    unsigned char buffer[256]; // Buffer con los bytes del bitmap
    fread(buffer, sizeof(buffer), 1, f);

    unsigned char data = buffer[byte]; // Saco el byte que me sirve
    fclose(f);

    data = data | (1 << offset);
    // Convierto el bit que me interesa en 1
    // if (bloque == 0){
    //     data = 1;
    // }

    // Puntero al byte de datos a escribir
    unsigned char* point_data = &data;
    f = fopen(global_diskname, "rb+");

    fseek(f, byte, SEEK_SET);
    fwrite(point_data, 1, 1, f);

    fclose(f);
}
// Busca en el bitmap el bit correspondiente al bloque
// y lo marca como usado (lo pone en 1)
void mark_as_unused(int bloque) {
    // El bit que corresponda al bloque va a estar en el byte:
    int byte = bloque / 8;
    int offset = 7 - bloque % 8;

    // Abro el archivo
    FILE* f = fopen(global_diskname, "rb");

    unsigned char buffer[256]; // Buffer con los bytes del bitmap
    fread(buffer, sizeof(buffer), 1, f);

    unsigned char data = buffer[byte]; // Saco el byte que me sirve
    fclose(f);

    // Convierto el bit que me interesa en 1
    data = (data & ~(1 << offset));

    // Puntero al byte de datos a escribir
    unsigned char* point_data = &data;
    f = fopen(global_diskname, "rb+");

    fseek(f, byte, SEEK_SET);
    fwrite(point_data, 1, 1, f);

    fclose(f);
}

int min(int a1, int a2){
    if (a1 < a2){
        return a1;
    }
    return a2;
}

void update_rotten_page(int block, int page_inside_block){
    FILE* open_file = fopen(global_diskname, "rb+");
    int real_page = (block*PAGES_PER_BLOCK + page_inside_block)*sizeof(int);

    fseek(open_file, 1 * BLOCK_SIZE + real_page, SEEK_SET);
    int buffer;
    fread(&buffer, sizeof(int), 1, open_file);
    buffer ++;
    if (buffer >= global_P_E){
        buffer = -1;
    }
    fseek(open_file, 1 * BLOCK_SIZE + real_page, SEEK_SET);
    fwrite(&buffer, sizeof(int), 1, open_file);
    fclose(open_file);
}
