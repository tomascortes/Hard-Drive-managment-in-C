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

#pragma once

#define BUFFER_SIZE 4096

// Representación de archivos abiertos mediate struct
typedef struct osFile {
    // REVIEW: Revisar que el nombre de tamaño indefinido no interfiera con
    //  malloc(sizeof(osFile))
    char* name;  // Nombre del archivo
    // Puse 2 caracteres para que sea un poco más a prueba de errores
    char mode[2]; // r -> ReadOnly || w-> WriteOnly || {rw,wr,r+} -> ReadWrite || N -> Null

    int starting_pos; // Posición donde inicia el bloque en el que parte el archivo.

    int start_block;  // Bloque donde comienza el archivo
    int length_bytes;  // Largo del archivo en bytes
    int end_block; // Último bloque que alcanza a ocupar

    int current_block; // Bloque actual
    int current_page; // Página actual del bloque actual
    int current_pos; // Posición actual de la página actual

    void* file;  // Apunta al archivo. TODO
    unsigned int block;
    unsigned int page;
} osFile;

osFile* osFile_new(char* name);
osFile* osFile_set_mode(osFile* self, char mode[2]);
osFile* osFile_set_location(osFile* self,
                            int start_block,
                            int end_block,
                            int length_bytes);
osFile* osFile_offset_pointer(osFile* self, int offset);
void osFile_destroy(osFile* self);

