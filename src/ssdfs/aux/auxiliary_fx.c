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
                aux[1] = '\0';
                aux[0] = buffer[j];
                strcat(path2, aux); // Concatenar char
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
                aux[1] = '\0';
                aux[0] = buffer[j];
                strcat(path2, aux); // Concatenar char
            }
            strcat(path, "/"); // Concatenar nuevo directorio
            int puntero = buffer[1]; // Pesco los bytes 1-4
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
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");
    char dir[100];
    strcpy(dir, dirname);

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
                aux[1] = '\0';
                aux[0] = buffer[j];
                strcat(path, aux); // Concatenar char
            }
            strcat(path, "/");
            int puntero = buffer[1]; // Pesco los bytes 1-4
            if (strcmp(path, dirname) == 0) { // compara con filename
                fclose(f); // Evitamos leaks
                return 1;
            }
            if (find_dir(puntero, dir, path)){// Función recursiva para leer
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
                aux[1] = '\0';
                aux[0] = buffer[j];
                strcat(path2, aux); // Concatenar char
            }
            strcat(path2, "/"); // Concatenar nuevo directorio
            int puntero = buffer[1]; // Pesco los bytes 1-4
            if (strcmp(path2, dirname) == 0) { // compara con filename
                fclose(f2); // Evitamos leaks
                return 1;
            }
            if (find_dir(puntero, dirname, path2)){// Función recursiva para leer
                fclose(f2); // Evitamos leaks
                return 1;
            };
        }
    }

    fclose(f2); // Evitamos leaks
    return 0;
}
