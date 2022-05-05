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

#include "./os_file.h"

// ========================== Struct ==========================

// =======================--- Setup ---========================
/// Crea una nueva instancia de la representación de un archivo
/// y retorna su ubicación en memoria
osFile* osFile_new(char* name, char* disk_pointer) {
    // Reservo memoria
    osFile* instance_pointer = malloc(sizeof(osFile));

    // Guardo el "disco" (como puntero)
    instance_pointer->disk = disk_pointer;

    // Pongo nombre
    instance_pointer->name = name;

    // Inicializo con valores por defecto. Inválidos para propósitos del FS
    osFile_set_mode(instance_pointer, "N");
    instance_pointer->has_page_loaded = false;

    // Retorno el puntero a la representación del archivo
    return instance_pointer;
}

/// Settea el modo de operación (read/write)
void osFile_set_mode(osFile* self, char* mode) {
    // TODO: Revisar validez
    // https://stackoverflow.com/questions/19365901/how-do-i-modify-the-character-array-in-this-struct-c
    strncpy(self->mode,  // Atributo a modificar
            mode,  // Nuevo contenido
            sizeof(self->mode));  // Máximo espacio (Para evitar stack overflow)
}

/// Settea la ubicación del puntero y largo del archivo
void osFile_set_location(osFile* self,
                         int plane,
                         int block,
                         int length_bytes) {
    self->current_plane = plane;
    self->current_block = block;

    self->length = length_bytes;

    self->current_page = 0;
    self->current_pos = 0;
}

void mark_rotten_pages(osFile* self, int*) {

}


// ====================--- File pointer ---====================

/// Desplazo el puntero n espacios
void osFile_offset_pointer(osFile* self, int offset) {
    if (self->current_pos + offset <= self->length)

    self->current_pos += offset;
}

// =======================--- Offset ---=======================
/// Calcula el offset de una página del archivo en relación al inicio del disco
long int osFile_calc_page_offset(osFile* self, int n_page) {
    long int page_offset;

    // ---- OFFSET ----
    // Pido el offset del bloque archivo y la página del input
    page_offset = calc_offset(self->current_plane,
                              self->current_block,
                              n_page,
                              0, 0);

    return page_offset;
}

// =======================--- Page-R ---=======================
/// Carga la página en la que se encuentra el puntero de lectura a memoria
void osFile_load_pointer_page(osFile* self) {

}

/// Cargo la página "n_page" del bloque en la dirección de memoria self->loaded_page
void osFile_load_page(osFile* self, int n_page) {
    // TODO: Revisar que página que mando no esté rotten.
    //  idealmente ANTES de pedir la lectura en esta función.
    long int page_offset;

    // ---- Checks ----
    // ------ MEM -----
    osFile_reserve_page_mem(self);

    // ---- OFFSET ----
    // Pido el offset del bloque archivo y la página del input
    page_offset = osFile_calc_page_offset(self, n_page);

    // ------ I/O -----
    osFile_copy_page_data(self, page_offset);
}

/// Carga los datos del disco en memoria dado un offset
void osFile_copy_page_data(osFile* self, long int offset) {
    FILE* file;  // Puntero a archivo

    //  https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
    //  Mode: "r" --> Opens a file for reading. The file must exist.
    //  https://stackoverflow.com/questions/2174889/whats-the-differences-between-r-and-rb-in-fopen
    //  'You should use "rb" if you're opening non-text files, because in this case,
    //  the translations are not appropriate.'
    // Abro un stream para el disco
    file = fopen(self->disk, "rb");

    // Desplazo el puntero al inicio de la pág.
    fseek(file, offset, SEEK_SET);

    //  https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm
    //  size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
    //      ptr − This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
    //      size − This is the size in bytes of each element to be read.
    //      nmemb − This is the number of elements, each one with a size of size bytes.
    //      stream − This is the pointer to a FILE object that specifies an input stream.
    // Cargo el contenido de la página en el heap
    fread(self->loaded_page, PAGE_SIZE, 1, file);

    // Cierro el archivo
    fclose(file);
}

// ========================--- Page ---========================
/// Si hay una página cargada, la libera
void osFile_release_page_if_loaded(osFile* self) {
    if (self->has_page_loaded) {
        osFile_release_page(self);
    }
}

/// Libero la memoria de la página
void osFile_release_page(osFile* self) {
    free(self->loaded_page);
    self->has_page_loaded = false;
}

// =======================--- Page-W ---=======================
/// Recibe el contenido de una página y lo guarda en memoria.
/// Tiene que tener largo de una página
// REVIEW: Favor revisar si puedo cambiar 'unsigned char' por 'const unsigned char'
void osFile_transfer_page(osFile* self, unsigned char content[PAGE_SIZE]) {
    // ---- Checks ----
    // ------ MEM -----
    osFile_reserve_page_mem(self);

    //// OPTIMIZE: Revisar si hay alguna forma más eficiente de mover grupos de datos
    ////  grandes (4kiB)
    // Copia byte por byte del array a memoria.
    for (int bytes_copied = 0;
         bytes_copied < PAGE_SIZE;
         bytes_copied++) {
        // REVIEW: Favor revisar que haga bien la copia
        self->loaded_page[bytes_copied] = content[bytes_copied];
    }
}

/// Escribe el contenido que tiene guardado en memoria en la página n_page del disco
void osFile_write_page(osFile* self, int n_page) {
    // TODO: Revisar que página que mando no esté rotten.
    //  idealmente ANTES de pedir la escritura en esta función.
    long int page_offset;
    FILE* file;  // Puntero a archivo

    // ---- OFFSET ----
    page_offset = osFile_calc_page_offset(self, n_page);

    // ---- I/O ----
    // Abro un stream para el disco
    file = fopen(self->disk, "wb");

    // Desplazo el puntero al inicio de la pág.
    fseek(file, page_offset, SEEK_SET);

    // Cargo el contenido de la página en el heap}
    // XXX: IMPORTANTE: Favor revisar que esté bien escrito
    fwrite(self->loaded_page, PAGE_SIZE, 1, file);

    // Cierro el archivo
    fclose(file);
}

// ========================--- Mem ---=========================
/// Reservo memoria para una página y la asigno a self->loaded_page
void osFile_reserve_page_mem(osFile* self) {
    // ---- Checks ----
    // Saco páginas cargada si es que hay
    osFile_release_page_if_loaded(self);

    // ------ MEM -----
    // Reservo memoria para la página
    self->loaded_page = malloc(PAGE_SIZE);
    self->has_page_loaded = true;
}

// ========================--- Data ---========================
/// Carga datos desde la página cargada en memoria a un array.
void osFile_load_data(osFile* self, int start, int end) {
    int bytes_amount = end - start;

    // Check de memoria
    osFile_release_data_if_loaded(self);

    // Reservo memoria para los bytes que voy a guardar
    self->loaded_data = malloc(bytes_amount);

    // Copio bytes
    // ...Para cada byte del rango, lo copio a la nueva memoria
    for (int bytes_copied = 0;
         bytes_copied < bytes_amount;
         bytes_copied++) {
        // REVIEW: Favor revisar que haga bien la copia
        self->loaded_data[bytes_copied] = self->loaded_page[start + bytes_copied];
    }
}

/// Si hay una página cargada, la libera
void osFile_release_data_if_loaded(osFile* self) {
    if (self->has_data_loaded) {
        osFile_release_data(self);
    }
}

/// Libero la memoria de la página
void osFile_release_data(osFile* self) {
    free(self->loaded_data);
    self->has_data_loaded = false;
}

// =======================--- Clean ---========================
/// Libera la memoria de todo lo asociado al struct. Luego libera la memoria del struct mismo.
void osFile_destroy(osFile* self) {
    // Libero memoria puntero nombre
    free(self->name);
    osFile_release_page_if_loaded(self);
    osFile_release_data_if_loaded(self);
    free(self->disk); // REVIEW: Hay que dejarlo???
    free(self);
}

/* ========================================================================= */

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
