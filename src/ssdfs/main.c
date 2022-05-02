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

#include <stdio.h>
#include "os_API.h"


int main (int argc, char* const argv[]) {
  // Montar el disco pasado por consola con life = 5000
  os_mount(argv[1], 5000);

    os_bitmap(0); // Bitmap completo
    os_bitmap(8); // Bitmap bloque N°8 (Se espera 1)
    os_bitmap(2047); // Bitmap bloque N°395 (Se espera 0)
    os_bitmap(2048); // Bitmap bloque inexistente (Se espera SEGFAULT)
    printf("\n");

  os_tree();

  return 0;
}
