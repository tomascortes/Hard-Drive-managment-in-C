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
