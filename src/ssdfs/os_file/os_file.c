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
osFile* osFile_new(char* name, char* disk_pointer) {
    // Reservo memoria
    osFile* instance_pointer = malloc(sizeof(osFile));

    // Guardo el "disco" (como puntero)
    instance_pointer->disk = disk_pointer;

    // Pongo nombre
    instance_pointer->name = name;

    // Inicializo con valores por defecto. Inválidos para propósitos del FS
    instance_pointer = osFile_set_mode(instance_pointer, "N");

    // Retorno el puntero a la representación del archivo
    return instance_pointer;
}

// Settea el modo de operación (read/write)
osFile* osFile_set_mode(osFile* self, char* mode) {
    // Revisar validez
    // https://stackoverflow.com/questions/19365901/how-do-i-modify-the-character-array-in-this-struct-c
    strncpy(self->mode,  // Atributo a modificar
            mode,  // Nuevo contenido
            sizeof(self->mode));  // Máximo espacio (Para evitar stack overflow)

    return self;
}

osFile* osFile_set_location(osFile* self,
                            unsigned int plane,
                            unsigned int block,
                            int length_bytes) {
    self->current_plane = plane;
    self->current_block = block;

    self->length = length_bytes;

    self->current_page = 0;
    self->current_pos = 0;

    return self;
}

osFile* osFile_offset_pointer(osFile* self, int offset) {
    // TODO: revisar límites
    self->current_pos = self->current_pos + offset;
}

char* osFile_get_block(osFile* self) {
    return;
}

char* osFile_get_page(osFile* self, char* block, int page) {
    return;
}

// Reserva espacio para un array de bytes
void osFile_load_page(osFile* self, char* block, int page) {

    return;
}

// Libero la memoria de la página
void osFile_release_page(osFile* self, char* block, int page) {
    return;
}

void osFile_destroy(osFile* self) {
    // Libero memoria puntero nombre
    free(self->name);
    free(self->disk); // REVIEW: Hay que dejarlo???
    free(self);
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
