// Compilar sólo una vez
#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

// Constante
#define DEBUG_MODE 0
#define SIMULATE_WAIT 1
#define WAITING_TIME3 10  // ms
#define WAITING_TIME 50  // ms
#define WAITING_TIME2 350  // ms

// Funciones
void wait_just_a_bit();
void just_wait();
void just_wait_longer();

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
