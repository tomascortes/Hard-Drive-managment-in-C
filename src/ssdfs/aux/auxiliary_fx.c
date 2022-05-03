/* Para funciónes auxiliares */

#include "auxiliary_fx.h"

int calc_offset(int plane, int block, int page, int cell) {
    int offset;

    int cell_offset = BYTES_PER_CELL;
    int page_offset = CELLS_PER_PAGE * cell_offset;
    int block_offset = PAGES_PER_BLOCK * page_offset;
    int plane_offset = BLOCKS_PER_PLANE * block_offset;


}

