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

void directree(int directory_block, int depth, char* global_diskname) {
    // Defino la verión recursiva de la función acá adentro
    // para cumplir con las reglas de no ofrecer más funciones en la API

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

