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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Representación de archivos abiertos mediate struct
typedef struct osFile {
    char* name;  // Nombre del archivo
    // Puse 2 caracteres para que sea un poco más a prueba de errores
    char mode[2]; // r -> ReadOnly || w-> WriteOnly || {rw,wr,r+} -> ReadWrite || N -> Null

    char* disk; // Puntero al disco para hacer las lecturas.

    int current_plane; // Número de plano en el que se encuentra el archivo.
    // plane --> {0..1}
    int current_block; // Número de bloque en el que se encuentra el archivo.
    // block --> {0..1023}
    // cada pag tiene 2048 celdas
    // cada celda son 16b

    int length; // Largo en bytes del archivo

    int current_page; // Página actual
    // page --> {0..256}
    int current_pos; // Posición actual dentro de la página actual
    // pos --> {0..4096}

    void* loaded_page; //página cargada en memoria
    bool page_loaded; // Si una página está cargada o no en heap

} osFile;

osFile* osFile_new(char* name, char* disk_pointer);

osFile* osFile_set_mode(osFile* self, char mode[2]);
osFile* osFile_set_location(osFile* self,
                            int plane,
                            int block,
                            // Bloque de índice dice el tamaño del archivo
                            int length_bytes);
osFile* osFile_offset_pointer(osFile* self, int offset);
int osFile_get_blk_start_pos(osFile* self);
void osFile_destroy(osFile* self);

