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

#include <stdlib.h>
#include <string.h>
#include "os_file.h"

// ----- Structs -----
/* Representa un archivo abierto con todos sus atributos */
// REVIEW: Revisar que el nombre de tamaño indefinido no interfiera con
//  malloc(sizeof(osFile))
osFile* osFile_new(char* name) {
    // Reservo memoria
    osFile* instance_pointer = malloc(sizeof(osFile));

    // Pongo nombre
    instance_pointer->name = name;

    // Inicializo con valores por defecto. Inválidos para propósitos del FS
    instance_pointer->start_pos = -1;
    instance_pointer->length = -1;
    instance_pointer->end_pos = -1;

    instance_pointer = set_mode(instance_pointer, "NN");

    // Retorno el puntero a la representación del archivo
    return instance_pointer;
}

// Settea el modo de operación (read/write)
osFile* set_mode(osFile* self, char* mode) {
    if

    // https://stackoverflow.com/questions/19365901/how-do-i-modify-the-character-array-in-this-struct-c
    strncpy(self->mode,  // Atributo a modificar
            mode,  // Nuevo contenido
            sizeof(self->mode));  // Máximo espacio (Para evitar stack overflow)

    return self;
}

osFile* set_location(osFile* self, int start_pos, int length, int end_pos) {

}













