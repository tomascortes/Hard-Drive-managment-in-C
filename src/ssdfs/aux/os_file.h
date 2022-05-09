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
    unsigned char index_pointer[4056]; // Puntero al inicio del bloque indice
    int block_index_number; // Numero del bloque que corresponde con indice
    char mode[2]; // r -> ReadOnly || w-> WriteOnly
    long int length; // Largo en bytes del archivo

    int current_index; // numero de bloque index
    // entre 1 y 1012
    int current_block; // Número de bloque en el que se encuentra el archivo.
    // entre 1 y 1024
    int current_page; // Página actual
    // page entre 0 y 255
    int current_cell; // Posición actual dentro de la página actual
    // pos entre 0 y 4095
    int current_byte; // Posición actual dentro de la celda
    // pos entre 0 y 1

    int bytes_loaded_count; // Cantidad de bytes leídos
    int remaining_bytes; // Bytes restantes que quedan por leer

    int amount_of_blocks;

} osFile;

// ----- Setup -----
/// Crea una nueva instancia de la representación de un archivo y retorna su
osFile* osFile_new(char* filename, char mode);

/// Settea el modo de operación (read/write)
void osFile_set_mode(osFile* self, char mode);

/// Settea la ubicación del puntero y largo del archivo
void setup_from_disk(osFile* self, char* filename);

// ----- Clean -----
/// Libera la memoria de todo lo asociado al struct. Luego libera la memoria del struct mismo.
void osFile_destroy(osFile* self);

void add_block_to_index(osFile* self, int new_block);
void print_index_block(osFile* self);

/// ------------------------------------------
/// Cosas que fui necesitando en el camino
/// ------------------------------------------

bool fxExtra_revisar_modo(osFile* file);
void fxExtra_hacer_el_setup(osFile* file);
int fxExtra_calc_max_bytes_lectura(osFile* file, int nbytes);
char* fxExtra_reservar_mem_char(int cantidad);
void* fxExtra_reservar_mem_void(int cantidad);
void fxExtra_liberar_mem_char(char* en_donde);
void fxExtra_liberar_mem_void(void* en_donde);
int fxExtra_nro_pagina_que_tengo_que_leer(osFile* file);
void fxExtra_cargar_pagina_en_mem(osFile* file, char* dir_pagina, int nro_pag);
bool quedan_bytes_por_leer(osFile* file);
void copiar_byte(osFile* file, char* desde, void* hacia, int en_donde_voy);
void avanzar_contador_archivo_y_actualizar_pos(osFile* file);
void reducir_bytes_restantes(osFile* file);
