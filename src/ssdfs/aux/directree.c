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

#include "directree.h"

// Defino la verión recursiva de la función acá adentro
// para cumplir con las reglas de no ofrecer más funciones en la API
void aux_directree(int directory_block, int depth, char* global_diskname) {
    //NOTE: Consejo: Poner un maxdepth para que no haga stack overflow si se sale de control
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
            aux_directree(puntero, depth, global_diskname); // Llamada recursiva
            depth--; // Vuelvo a la profundidad anterior
        }
    }

    fclose(f2); // Evitamos leaks
}

// Defino la verión recursiva de la función acá adentro
// para cumplir con las reglas de no ofrecer más funciones en la API
int aux_directreen(int directory_block, char* filename,
                   char* path, char* global_diskname) {
    //NOTE: Consejo: Poner un maxdepth para que no haga stack overflow si se sale de control
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
                // WARN: Se está tirando un "unsign char" a "char"
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
                // WARN: Se está tirando un "unsign char" a "char"
                aux[0] = buffer[j];
                strcat(path2, aux); // Concatenar char
            }

            strcat(path, "/"); // Concatenar nuevo directorio
            int puntero = buffer[1]; // Pesco los bytes 1-4

            if (aux_directreen(puntero, filename, path2, global_diskname)){// Función recursiva para leer
                fclose(f2); // Evitamos leaks
                return 1;
            }
        }
    }

    fclose(f2); // Evitamos leaks

    return 0;
}

