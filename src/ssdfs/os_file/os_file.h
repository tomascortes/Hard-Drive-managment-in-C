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

#define PLANES_PER_DISK 2
#define BLOCKS_PER_PLANE 1024
#define PAGES_PER_BLOCK 256
#define CELLS_PER_PAGE 2048
#define BYTES_PER_CELL 2

// Representación de archivos abiertos mediate struct
typedef struct osFile {
    char *name;  // Nombre del archivo
    // Puse 2 caracteres para que sea un poco más a prueba de errores
    char mode[2]; // r -> ReadOnly || w-> WriteOnly || {rw,wr,r+} -> ReadWrite || N -> Null

    char* disk; // Puntero al disco para hacer las lecturas.

    unsigned int current_plane; // Número de plano en el que se encuentra el archivo.
    // plane --> {0..1}
    unsigned int current_block; // Número de bloque en el que se encuentra el archivo.
    // block --> {0..1023}
    // cada pag tiene 2048 celdas
    // cada celda son 16b

    int length; // Largo en bytes del archivo

    unsigned int current_page; // Página actual
    // page --> {0..256}
    unsigned int current_pos; // Posición actual dentro de la página actual
    // pos --> {0..4096}

} osFile;

osFile* osFile_new(char* name, char* disk_pointer);

osFile* osFile_set_mode(osFile* self, char mode[2]);
osFile* osFile_set_location(osFile* self,
                            unsigned int plane,
                            unsigned int block,
                            // Bloque de índice dice el tamaño del archivo
                            int length_bytes);
osFile* osFile_offset_pointer(osFile* self, int offset);
void osFile_destroy(osFile* self);

