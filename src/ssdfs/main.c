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
   
    // Montar el disco pasado por consola con life = 5000
    os_mount(argv[1], 5000);
    mark_as_used(3);

    //Prueba bitmap
    os_bitmap(0);
    // os_tree();

    // abrir archivo existente
    // osFile* os_file = os_open("/dir1ta/amogus.mp4", 'r');

    // Crear archivo nuevo
    printf("Segundo archivo\n");
    osFile* os_file2 = os_open("quiero_llorar.a", 'w');
    char buffer[4] = "AAAA" ;
    
    os_write(os_file2, &buffer, 24);
    os_bitmap(0);

    os_close(os_file2);
    read_from_disk(os_file2, "quiero_llorar.a");


    // Cierra los archivos
    // os_close(os_file);

    // TODO: Malo
    //osFile_destroy(os_file);

    // Cierra el archivo
    // os_close(os_file);

    /*char path[] = "~/carpeta/SuenosyEsperanzas";
    print_debug("Prueba os_mkdir");
    os_mkdir(path);
    print_debug("Fin prueba os_mkdir\n");*/


    // char path_to_find[] = "~/rottendir";
    // char path_inicial_test[100];
    // int testint = 3;
    // strcpy(path_inicial_test, "~/");
    // print_debug("Prueba pathfinder");
    // printf("El directorio está en el bloque %i\n", 
    //         pathfinder(path_to_find, testint, path_inicial_test));
    // print_debug("Fin prueba os_mkdir\n");
    

    return 0;
}
