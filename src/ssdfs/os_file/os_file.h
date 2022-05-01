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

#pragma once

// XXX: Lo siento, no pude restringirme a las funciones que nos dijeron sin hacer un desastre.
//  Puse estas para manejar el struct

// Representación de archivos abiertos mediate struct
typedef struct osFile {
    // REVIEW: Revisar que el nombre de tamaño indefinido no interfiera con
    //  malloc(sizeof(osFile))
    char* name;  // Nombre del archivo
    char mode[2]; // r -> ReadOnly || {w,rw,wr,r+} -> ReadWrite
    int start_pos;  // Donde comienza el archivo
    int length;  // Largo del archivo
    int end_pos; // Donde termina
} osFile;

osFile* osFile_new(char* name);
osFile* set_mode(osFile* self, char mode[2]);
osFile* set_location(osFile* self,
                     int start_pos,
                     int length,
                     int end_pos);
void osFile_destroy(osFile* self);
