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
#include <stdio.h>
#include <string.h>

#include "./auxiliary_fx.h"

/// Representación de archivos abiertos mediate struct
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

    unsigned char* loaded_page; //página cargada en memoria
    bool has_page_loaded; // Si una página está cargada o no en heap

    // NOTE: esto no es propio de un archivo como tal, pero no se me ocurría cómo más pasarlo
    //  si no era por medio de este atributo.
    unsigned char* loaded_data; //datos de una página cargados en memoria
    bool has_data_loaded; // Si tiene datos cargados

} osFile;

// ----- Setup -----
/// Crea una nueva instancia de la representación de un archivo y retorna su ubicación en memoria
osFile* osFile_new(char* name, char* disk_pointer);

/// Settea el modo de operación (read/write)
void osFile_set_mode(osFile* self, char mode[2]);

/// Settea la ubicación del puntero y largo del archivo
void osFile_set_location(osFile* self,
                            int plane,
                            int block,
                            // Bloque de índice dice el tamaño del archivo para ponerlo aquí.
                            int length_bytes);

// ----- File pointer -----
/// Desplazo el puntero n espacios
void osFile_offset_pointer(osFile* self, int offset);

// ----- Page -----
/// Cargo la página "n_page" del bloque en la dirección de memoria self->loaded_page
void osFile_load_page(osFile* self, int n_page);

/// Carga los datos del disco en memoria dado un offset
void osFile_copy_page_data(osFile* self, long int offset);

/// Si hay una página cargada, la libera
void osFile_release_page_if_loaded(osFile* self);

/// Libero la memoria de la página
void osFile_release_page(osFile* self);

// ----- Data -----
/// Carga datos desde la página cargada en memoria a un array.
void osFile_load_data(osFile* self, int start, int end);

/// Si hay una página cargada, la libera
void osFile_release_data_if_loaded(osFile* self);

/// Libero la memoria de la página
void osFile_release_data(osFile* self);

// ----- Clean -----
/// Libera la memoria de todo lo asociado al struct. Luego libera la memoria del struct mismo.
void osFile_destroy(osFile* self);

