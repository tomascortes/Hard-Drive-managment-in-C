/* +===================================+=================================+
 * |     P01 IIC2333      |    DCCegmentation Fault   |    2022-05-04    |
 * +==================+================+===================+=============+
 * |      Nombre      |  Usr. GitHub   |      Correo       | Nro. Alumno |
 * +==================+================+===================+=============+
 * | Matías López     | MatLop1        | milopez8@uc.cl    | 17210674    |
 * +------------------+----------------+-------------------+-------------+
 * | Tomás Cortés     | ticortes       | ticortez@uc.cl    | ########    |
 * +------------------+----------------+-------------------+-------------+
 * | Felipe Villagrán | BibarelUsedFly | fivillagran@uc.cl | 16638689    |
 * +------------------+----------------+-------------------+-------------+
 * | Marcelo Bernal   | msbernal       | msbernal@uc.cl    | 1763671J    |
 * +------------------+----------------+-------------------+-------------+
 * | Luis González    | ljgonzalez1    | ljgonzalez@uc.cl  | 16625439    |
 * +------------------+----------------+-------------------+-------------+ */

#include <stdlib.h>
#include <string.h>
#include "os_file.h"

// ----- Structs -----
/* Representa un archivo abierto con todos sus atributos */
osFile* osFile_new(char* name) {
    osFile* instance_pointer = malloc(sizeof(osFile));

    instance_pointer->name = name;

    instance_pointer->start_pos = -1;
    instance_pointer->length = -1;
    instance_pointer->end_pos = -1;

    // https://stackoverflow.com/questions/19365901/how-do-i-modify-the-character-array-in-this-struct-c
    strncpy(instance_pointer->mode,  // Atributo a modificar
            "NN",  // Nuevo contenido
            sizeof(instance_pointer->mode));  // Máximo espacio (Para evitar stack overflow)

    return instance_pointer;
}