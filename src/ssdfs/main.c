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
#include "./debug/debug.h"


#include <string.h> // Para testear



int main (int argc, char* const argv[]) {
   
    os_mount(argv[1], 5000);
    // os_bitmap(0);
    // os_tree();
    printf("\Inicio de apertura del archivo\n");
    osFile* os_file = os_open("/redes/dino.jpg", 'r');
    printf("\nTermino de apertura del archivo\n");
    printf("\Inicio de lectura  del archivo\n");
    void* buffer;
    // int a = 10;
    // void buffer2[sideof(int)] = a;
    os_read(os_file, &buffer, 4);
    os_read(os_file, &buffer, 10);
    os_read(os_file, &buffer, 3);
    os_read(os_file, &buffer, 2);
    printf("\nTermino de lectura  del archivo\n");
    os_close(os_file);  

    return 0;
}
