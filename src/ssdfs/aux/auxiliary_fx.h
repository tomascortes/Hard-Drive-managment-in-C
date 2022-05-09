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

#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PLANES_PER_DISK 2
#define BLOCKS_PER_PLANE 1024
#define PAGES_PER_BLOCK 256
#define CELLS_PER_PAGE 2048
#define BYTES_PER_CELL 2

#define DIR_ENTRY_SIZE 32

// Dependen de las de antes
#define CELL_SIZE BYTES_PER_CELL  // 2 B
#define PAGE_SIZE (CELL_SIZE * CELLS_PER_PAGE)  // 4096 B = 4kiB
#define BLOCK_SIZE (PAGE_SIZE * PAGES_PER_BLOCK)  // 1048576 B = 1MiB
#define PLANE_SIZE (BLOCK_SIZE * BLOCKS_PER_PLANE)  // 1073741824 B = 1GiB
#define DISK_SIZE (PLANE_SIZE * PLANES_PER_DISK)  // 2147483648 B = 2GiB

#define PAGES_PER_DISK (PLANES_PER_DISK * BLOCKS_PER_PLANE * PAGES_PER_BLOCK)  // 524288 pgs
#define DIR_ENTRIES_PER_BLOCK 32768  // No puse la división por miedo a que cambie el tipo de variable

char global_diskname[1023];
int global_P_E;
int unactualized_change;

long int calc_offset(int plane, // Número de planos
                     int block, // Número de bloques
                     int page, // Número de páginas
                     int cell, // Número de celdas
                     int bytes); // Número de bytes

int is_page_rotten(int page, char* diskname);

int dir_exists(char* dirname);

int find_file(int directory_block,
              char* filename,
              char* path);

int pathfinder(char* path, int bloque_final, char* path_parcial, char* diskname);

int find_dir(int directory_block,
             char* filename,
             char* path);
             
int get_index_file(int directory_block, char* filename, char* path);

int get_index_pointer_and_length(char* filenamem);

void directree(int directory_block, int depth, char* global_diskname);
int blocksearch(char* diskname);

bool is_block_rotten(int block);
bool is_block_available(unsigned num);