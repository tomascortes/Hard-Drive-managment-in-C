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

// Busca en el bitmap el bit correspondiente al bloque
// y lo marca como usado (lo pone en 1)
void mark_as_used(int bloque) {
    // El bit que corresponda al bloque va a estar en el byte:
    int byte = bloque / 8;
    int offset = 7 - bloque % 8;

    // Abro el archivo
    FILE* f = fopen(global_diskname, "rb");

    unsigned char buffer[256]; // Buffer con los bytes del bitmap
    fread(buffer, sizeof(buffer), 1, f);

    unsigned char data = buffer[byte]; // Saco el byte que me sirve
    fclose(f);

    // Convierto el bit que me interesa en 1
    data = data | (1 << offset);

    // Puntero al byte de datos a escribir
    unsigned char* point_data = &data;
    f = fopen(global_diskname, "rb+");

    fseek(f, byte, SEEK_SET);
    fwrite(point_data, 1, 1, f);

    fclose(f);
}

int main (int argc, char* const argv[]) {
   
    // Montar el disco pasado por consola con life = 5000
    os_mount(argv[1], 5000);

    //Prueba bitmap
    os_bitmap(0);
    //mark_as_used(7); Funciona!
    //os_bitmap(0);

    // Ver todos los archivos y directorios
    os_tree();

    // Abre el archivo
    // osFile* os_file = os_open("/dir1ta/amogus.mp4", 'r');
    //printf("Segundo archivo\n");
    //osFile* os_file2 = os_open("/dir1ta/quiero_lorrar.a", 'w');
    //char buffer[3] = "AAA";
    //os_write(os_file2, buffer, 24);


    // Cierra los archivos
    // os_close(os_file);
    //os_close(os_file2);

    // TODO: Malo
    //osFile_destroy(os_file);

    // Cierra el archivo
    //os_close(os_file);

    /*char path[] = "~/carpeta/SuenosyEsperanzas";
    print_debug("Prueba os_mkdir");
    os_mkdir(path);
    print_debug("Fin prueba os_mkdir\n");*/


    char path_to_find[] = "~/rottendir";
    char path_inicial_test[100];
    int testint = 3;
    strcpy(path_inicial_test, "~/");
    print_debug("Prueba pathfinder");
    printf("El directorio está en el bloque %i\n", 
            pathfinder(path_to_find, testint, path_inicial_test));
    print_debug("Fin prueba os_mkdir\n");
    

    return 0;
}
