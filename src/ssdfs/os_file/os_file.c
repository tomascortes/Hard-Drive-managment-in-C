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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "os_file.h"

// ----- Structs -----
/* Representa un archivo abierto con todos sus atributos */
osFile* osFile_new(char* name) {
    // Reservo memoria
    osFile* instance_pointer = malloc(sizeof(osFile));

    // Pongo nombre
    instance_pointer->name = name;

    // Inicializo con valores por defecto. Inválidos para propósitos del FS
    // TODO: Sacar línea innecesaria
    // instance_pointer = set_location(instance_pointer, -1, -1, -1);
    instance_pointer = set_mode(instance_pointer, "N");

    // Retorno el puntero a la representación del archivo
    return instance_pointer;
}

// Settea el modo de operación (read/write)
osFile* set_mode(osFile* self, char* mode) {
    // Revisar validez
    // https://stackoverflow.com/questions/19365901/how-do-i-modify-the-character-array-in-this-struct-c
    strncpy(self->mode,  // Atributo a modificar
            mode,  // Nuevo contenido
            sizeof(self->mode));  // Máximo espacio (Para evitar stack overflow)

    return self;
}

osFile* set_location(osFile* self, int start_pos, int length, int end_pos) {
    self->start_pos = start_pos;
    self->length = length;
    self->end_pos = end_pos;

    return self;
}


void osFile_destroy(osFile* self) {
    // REVIEW: no debería tener que asegurarme de nada, porque no reservé más memoria
    //  dentro de sí, pero si alguien la quiere revisar, se agradece
    free(self)
}

// TODO: Sacar si no la uso.
//  La deje de usar por ahora, pero no la quiero borrar por si la vuelvo a necesitar
int interpret_mode(char* mode) {
    if (strcmp(mode, "r") != 0 ||
            strcmp(mode, "R") != 0) {
        return 0;  // ReadOnly

    } else if (strcmp(mode, "w") != 0 ||
            strcmp(mode, "W") != 0) {
        return 1;  // WriteOnly

    } else if (strcmp(mode, "wr") != 0 ||
            strcmp(mode, "WR") != 0 ||
            strcmp(mode, "rw") != 0 ||
            strcmp(mode, "RW") != 0 ||
            strcmp(mode, "r+") != 0 ||
            strcmp(mode, "R+") != 0) {
        return 2; //Read-Write

    } else {
        return -1;  // Null
    }
}
