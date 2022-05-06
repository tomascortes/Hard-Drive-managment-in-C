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

// Compilar sólo una vez
#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

// Constante
#define DEBUG_MODE 0
#define SIMULATE_WAIT 0
#define WAITING_TIME 250  // ms
#define WAITING_TIME2 1000  // ms
#define WAITING_TIME3 2000  // ms

// Funciones
void wait_debug(int time_ms);
void just_wait_debug();
void wait_longer_debug();
void wait_a_bit_debug();

void dprint_line();
void dprint_txt();
void dprint_txt2();

void dprint_char_xx(char **input);
void dprint_char_x(char *input);
void dprint_txt_char_xx(char **input);
void dprint_txt_char_x(char *input);
void dprint_char_xx2(char **input);
void dprint_char_x2(char *input);
void dprint_txt_char_xx2(char **input);
void dprint_txt_char_x2(char *input);

void dprint_pid(pid_t process_id);
void dprint_txt_pid(pid_t process_id);

void dprint_int(int input);
void dprint_txt_int(int input);

void dprint_float(float input);
void dprint_txt_float(float input);

void print_debug(char *input);
