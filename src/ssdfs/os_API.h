#pragma once

char global_diskname[1023];
int global_P_E;

// Representación de archivos abiertos mediate struct
struct os_file {  // NOTE: Trabajando en esto - Luis
    int pid;  // NOTE: No sé si se va a usar. Lo puse por si a caso.
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

// Funciones generales
void os_mount(char* diskname, unsigned life);
void os_bitmap(unsigned num);
void os_lifemap(int lower, int upper);  // TODO: Pendiente
int os_trim(unsigned limit);  // TODO: Pendiente
void os_tree();

// Funciones de manejo de archivos
int os_exists(char* filename);  // TODO: Pendiente
osFile* os_open(char* filename, char mode);  // TODO: Pendiente
int os_read(osFile* file_desc, void* buffer, int nbytes);  // NOTE: Trabajando en esto - Luis
int os_write(osFile* file_desc, void* buffer, int nbytes);  // TODO: Pendiente
int os_close(osFile* file_desc);  // TODO: Pendiente
int os_rm(char* filename);  // TODO: Pendiente
int os_mkdir(char* path);  // TODO: Pendiente
int os_rmdir(char* path);  // TODO: Pendiente
int os_rmrfdir(char* path);  // TODO: Pendiente
int os_unload(char* orig, char* dest);  // TODO: Pendiente
int os_load(char* orig);  // TODO: Pendiente

void print_names();
