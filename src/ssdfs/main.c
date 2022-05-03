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

// Busca en el bitmap el bit correspondiente al bloque
// hy lo marca como usado (lo pone en 1)
// Creo que me falta debuggear esto, no usar por ahora uwu
void mark_as_used(int bloque) {
    // El bit que corresponda al bloque va a estar en el byte:
    int byte = bloque / 8;
    int offset = 7 - bloque % 8;

    // Abro el archivo
    FILE* f = fopen(global_diskname, "rb");

    unsigned char buffer[256]; // Buffer con los bytes del bitmap
    fread(buffer, sizeof(buffer), 1, f);

    // FIXME: "Narrowing conversion from 'unsigned char' to signed type 'char'
    //  is implementation-defined"
    char data = buffer[byte]; // Saco el byte que me sirve

    fclose(f);
    printf("Data before: %i", data);

    // FIXME: "Narrowing conversion from 'int' to signed type 'char' is
    //  implementation-defined"
    // Convierto el bit que me interesa en 1
    data = (data | (1 << offset)) >> offset;
    printf("Data after: %i", data);

    // Puntero al byte de datos a escribir
    char* point_data = &data;
    f = fopen(global_diskname, "wb");

    fseek(f, byte, SEEK_SET);
    fwrite(point_data, 1, 1, f);

    fclose(f);
}

int main (int argc, char* const argv[]) {
    print_debug("Iniciando el programa...");

    print_debug("Montando el disco con life = 5000\n");
    // Montar el disco pasado por consola con life = 5000
    os_mount(argv[1], 5000);

    print_debug("Prueba bitmap comentada...");
    // os_bitmap(0); // Bitmap completo
    // os_bitmap(8); // Bitmap bloque N°8 (Se espera 1)
    // os_bitmap(2047); // Bitmap bloque N°395 (Se espera 0)
    // os_bitmap(2048); // Bitmap bloque inexistente (Se espera SEGFAULT)
    print_debug("...Fin prueba bitmap comentada\n");

    print_debug("Printea el os_lifemap de páginas 1270 a la 1300");
    os_lifemap(1270, 1300);
    print_debug("Fin de la función de os_lifemap lifemap\n");

    print_debug("Prueba os_lifemap comentadas...");
    // os_lifemap(524288/4 - 5000, 524288/4-2000);
    print_debug("...Prueba os_lifemap comentadas\n");

    print_debug("Corre os_tree");
    os_tree();
    print_debug("Termina de correr os_tree\n");

    print_debug("Prueba mark_as_used comentadas...");
    //mark_as_used(3);
    //os_bitmap(0);
    print_debug("...Fin prueba mark_as_used comentadas...\n");

    return 0;
}
