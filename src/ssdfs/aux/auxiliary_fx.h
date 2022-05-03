/* Para funciónes auxiliares */

#pragma once

#define PLANES_PER_DISK 2
#define BLOCKS_PER_PLANE 1024
#define PAGES_PER_BLOCK 256
#define CELLS_PER_PAGE 2048
#define BYTES_PER_CELL 2

// Dependen de las de antes
#define CELL_SIZE BYTES_PER_CELL
#define PAGE_SIZE (CELL_SIZE * CELLS_PER_PAGE)
#define BLOCK_SIZE (PAGE_SIZE * PAGES_PER_BLOCK)
#define PLANE_SIZE (BLOCK_SIZE * BLOCKS_PER_PLANE)
#define DISK_SIZE (PLANE_SIZE * PLANES_PER_DISK)

long int calc_offset(int plane, // Número de planos
                     int block, // Número de bloques
                     int page, // Número de páginas
                     int cell, // Número de celdas
                     int bytes); // Número de bytes
