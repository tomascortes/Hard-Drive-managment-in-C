/* Para funciónes auxiliares */

#include "auxiliary_fx.h"

// Calcula offsets en base a plano, bloque, página, celta y byte
// NOTE: El plano 0 es el primer plano
// NOTE: El bloque 0 es el primer plano
// ...
long int calc_offset(int plane, int block, int page, int cell, int bytes) {
    long int offset = 0;

    long int cell_offset = BYTES_PER_CELL;
    long int page_offset = CELLS_PER_PAGE * cell_offset;
    long int block_offset = PAGES_PER_BLOCK * page_offset;
    long int plane_offset = BLOCKS_PER_PLANE * block_offset;

    offset += bytes;
    offset += cell * cell_offset;
    offset += page * page_offset;
    offset += block * block_offset;
    offset += plane * plane_offset;

    return offset;
}
