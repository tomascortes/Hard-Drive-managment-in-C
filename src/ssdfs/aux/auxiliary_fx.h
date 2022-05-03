/* Para funciónes auxiliares */

#pragma once

#define PLANES_PER_DISK 2
#define BLOCKS_PER_PLANE 1024
#define PAGES_PER_BLOCK 256
#define CELLS_PER_PAGE 2048
#define BYTES_PER_CELL 2

int calc_offset(int plane,
                int block,
                int page,
                int cell);
