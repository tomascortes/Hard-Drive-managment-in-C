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

    // os_mount(argv[1], 5000);  // 0.2
    os_mount("simdiskfilled.bin", 10);
    mark_as_used(0);
    mark_as_used(1);
    mark_as_used(2);
    mark_as_used(3);

    os_tree();
    os_rmdir("~/uwu");
    os_rmdir("~/carpeta");
    os_tree();
    // os_lifemap(0, 10000);
    // os_bitmap(0);
    // os_tree();
    
    // os_mount("simdiskempty.bin", 10);
    // os_tree();
    // Crear archivo nuevo
    // printf("Comienza open\n");
    // char  nombre_generico_de_archivo[] = "~/uwu.dll" ;

    // osFile* os_file = os_open(nombre_generico_de_archivo, 'w');
    // if (os_file != NULL){
    //     char buffer[10] = "AAABBBCCCD";
    //     os_write(os_file, buffer, 10);
    // }
    // os_close(os_file);
    
    // os_lifemap(6*PAGES_PER_BLOCK, 7*PAGES_PER_BLOCK);
    // os_tree();
    // os_rm(nombre_generico_de_archivo);
    // printf("Despues de borrar el archivo");
    // os_tree();

    // printf("\n\n\n\n\nCreamos un directorio\n");
    // char path[] = "~/dir1";
    // os_mkdir(path);
    // os_tree();

    // printf("\n Termino exitoso\n");
    return 0;
}
