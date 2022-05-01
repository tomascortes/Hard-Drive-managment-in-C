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
// REVIEW: Alguien por favor revise si esto se puede combinar dentro de os_file
//  con 'typedef struct osFile {...} osFile;' a pesar de referenciarlo dentro
//  de sí mismo para reservar memoria
struct os_file;
typedef struct os_file osFile;

struct os_file {
    char* name;  // Nombre del archivo
    char mode[2]; // r -> ReadOnly || {w,rw,wr,r+} -> ReadWrite
    unsigned int start_pos;  // Donde comienza el archivo
    unsigned int length;  // Largo del archivo
    unsigned int end_pos; // Donde termina
};

// REVIEW: Alguien por favor revise si esto se puede combinar dentro de os_file
//  con 'typedef struct osFile {...} osFile;' a pesar de referenciarlo dentro
//  de sí mismo para reservar memoria
typedef struct os_file osFile;
