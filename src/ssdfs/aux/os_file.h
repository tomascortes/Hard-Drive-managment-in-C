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
    char* filename;  // Nombre del archivo

    char* disk; // Puntero al disco para hacer las lecturas.

    char mode[2]; // r -> ReadOnly || w-> WriteOnly || {rw,wr,r+} -> ReadWrite || N -> Null

    long int length; // Largo en bytes del archivo

    int index_block; // Puntero al inicio del bloque indice

} osFile;

// ----- Setup -----
/// Crea una nueva instancia de la representación de un archivo y retorna su
osFile* osFile_new(char* filename, char mode);

/// Settea el modo de operación (read/write)
void osFile_set_mode(osFile* self, char mode);

/// Settea la ubicación del puntero y largo del archivo
void osFile_set_location(osFile* self, char* filename);

// ----- File pointer -----
/// Desplazo el puntero n espacios
void osFile_offset_pointer(osFile* self, int offset);

// ----- Offset -----
/// Calcula el offset de una página del archivo en relación al inicio del disco
long int osFile_calc_page_offset(osFile* self, int n_page);

// ---- Page-R ----
/// Calcula la página del bloque en la que se encuentra el puntero
int osFile_get_current_page(osFile* self, const int* rotten_array_pointer);

/// Resetea cuenta de bytes cargados
void osFile_reset_bytes_count(osFile* self);

/// Carga la página en la que se encuentra el puntero de lectura a memoria
void osFile_load_pointer_page(osFile* self, const int* rotten_array_pointer);

/// Cargo la página "n_page" del bloque en la dirección de memoria self->loaded_page
void osFile_load_page(osFile* self, int n_page);

/// Carga los datos del disco en memoria dado un offset
void osFile_copy_page_data(osFile* self, long int offset);

// ----- Page -----
/// Si hay una página cargada, la libera
void osFile_release_page_if_loaded(osFile* self);

/// Libero la memoria de la página
void osFile_release_page(osFile* self);

// ---- Page-W ----
/// Recibe el contenido de una página y lo guarda en memoria.
/// Tiene que tener largo de una página
void osFile_transfer_page(osFile* self, unsigned char content[PAGE_SIZE]);

/// Escribe el contenido que tiene guardado en memoria en la página n_page del disco
void osFile_write_page(osFile* self, int n_page);

// ------ Mem -----
/// Reservo memoria para una página y la asigno a self->loaded_page
void osFile_reserve_page_mem(osFile* self);

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
