/* Para funciónes auxiliares */

#include "auxiliary_fx.h"

// Calcula offsets en base a plano, bloque, página, celta y byte
// NOTE: El plano 0 es el primer plano
// NOTE: El bloque 0 es el primer plano
// ...
long int calc_offset(int plane, int block, int page, int cell, int bytes) {
    long int offset = 0;

    offset += bytes;
    offset += cell * CELL_SIZE;
    offset += page * PAGE_SIZE;
    offset += block * BLOCK_SIZE;
    offset += plane * PLANE_SIZE;

    return offset;
}
