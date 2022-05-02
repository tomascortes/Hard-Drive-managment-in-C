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

#pragma once
#include <stdlib.h>

#include "./os_file/os_file.h"  // NOTE: Trabajando en esto

char global_diskname[1023];
int global_P_E;
int unactualized_change;

// Funciones generales
void os_mount(char* diskname, unsigned life);
void os_bitmap(unsigned num);
void os_lifemap(int lower, int upper);  // TODO: Pendiente
int os_trim(unsigned limit);  // TODO: Pendiente
void os_tree();

// Funciones de manejo de archivos
int os_exists(char* filename);  // TODO: Pendiente
osFile* os_open(char* filename, char mode);  // TODO: Pendiente
int os_read(osFile* file_desc, void* buffer, int nbytes);  // NOTE: Trabajando en esto
int os_write(osFile* file_desc, void* buffer, int nbytes);  // TODO: Pendiente
int os_close(osFile* file_desc);  // TODO: Pendiente
int os_rm(char* filename);  // TODO: Pendiente
int os_mkdir(char* path);  // TODO: Pendiente
int os_rmdir(char* path);  // TODO: Pendiente
int os_rmrfdir(char* path);  // TODO: Pendiente
int os_unload(char* orig, char* dest);  // TODO: Pendiente
int os_load(char* orig);  // TODO: Pendiente

void print_names();
