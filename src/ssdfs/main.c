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
    mark_as_used(0);
    mark_as_used(1);
    mark_as_used(2);
    mark_as_used(3);
    // for (int i = 5; i<100;i++){
    //     mark_as_unused(i);
    // }



    osFile* os_file = os_open("~/drums.mp4", 'w');
    if (os_file != NULL){
        char buffer[10] = "AAABBBCCCD";
        
        os_write(os_file, buffer, 10);
        os_lifemap(6*PAGES_PER_BLOCK, 7*PAGES_PER_BLOCK);
        os_tree();
    }
    
    
    os_rm("~/drums.mp4");
    os_tree();



    char testdir[] = "~/carpeta/ggramo";
    os_mkdir(testdir);
    os_tree();

    return 0;
}
