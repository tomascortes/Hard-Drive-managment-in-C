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

#include <string.h>
#include "./os_file.h"

// ========================== Struct ==========================

// =======================--- Setup ---========================
/// Crea una nueva instancia de la representación de un archivo
/// y retorna su ubicación en memoria
osFile* osFile_new(char* filename, char mode) {
    
    osFile* instance_pointer = malloc(sizeof(osFile)); // Reservo memoria

    instance_pointer->filename = filename;
    osFile_set_mode(instance_pointer, mode); // Inicializo con valores por defecto.

    if (strcmp(instance_pointer->mode, "w") == 0) {
        // put_on_disk(instance_pointer, filename); 
        //TODO: Falta agregar el osFile NUEVO al direcotrio del disco
         // Encontrar primer bloque desocupado para usarlo de indice
        int index_block = get_usable_block();
        mark_as_used(index_block); // marcamos como usado el nuevo indice
        instance_pointer ->block_index_number = index_block;
        printf("Nuevo bloque de directorio %d\n", instance_pointer ->block_index_number);
        instance_pointer-> filename = filename; // BUG: esto está mal uwu pero lo veré despues
        instance_pointer->amount_of_blocks = 0; // atributo utilizado solo en write
        instance_pointer->length=-1; // Archivo no escrito

        return instance_pointer;
    }

    else if (strcmp(instance_pointer->mode, "r") == 0) {
        setup_from_disk(instance_pointer, filename);
        return instance_pointer;

    } else {
        return NULL;
    }
}

/// Settea el modo de operación (read/write)
void osFile_set_mode(osFile* self, char mode) {
    char aux[2]; // variable para concatenar char
    aux[1] = '\0';
    aux[0] = mode;
    strcpy(self->mode, aux);  // Máximo espacio (Para evitar stack overflow)
}

// Settea la ubicación del puntero, largo del archivo y otras variables necesarias para la lectura.
void setup_from_disk(osFile* self, char* filename) {
    // asignar atributos
    self->block_index_number = get_index_pointer(filename); 
    FILE* opened_file = fopen(global_diskname, "rb");
    fseek(opened_file, self->block_index_number * BLOCK_SIZE, SEEK_SET);

    // leer todo el bloque indice
    fread(self->index_pointer, sizeof(self->index_pointer), 1, opened_file); // Leo una entrada

    //encontrar length
    // Si quiero acceder a los primeros 8 Byte es self->pointer
    // Si quiero acceder a los 8 Byte siguientes al primer byte es (self->pointer + 1)
    // Si suma en C corresponde al avance en punteros de los byte correspondientes a la variable
    // +1 en char es 1 byte
    // +1 en int son 4 byte
    self->length = *(long int*)self->index_pointer;

    // puntero al pimer bloque de datos
    self->current_index = 2; // Indice Actual
    self->current_block = *(int*) (self->index_pointer + 4*self->current_index); // Número de bloque en el que se encuentra el archivo
    self->current_page = 0; // Página actual
    self->current_cell = 0; // celda actual
    self->current_byte = 0; // byte actual

    self->bytes_loaded_count = 0; // Cantidad de bytes leídos. Debe resetearse cada vez que se lee
    self->remaining_bytes = self->length; // Bytes restantes que quedan por leer

    printf("\n\nLargo de Archivo: %ld\n", self->length);
    printf("Cantidad de Bloques del Archivo: %f\n", (float)self->length/BLOCK_SIZE);
    printf("Indice Actual: %d\n", self->current_index);
    printf("Bloque Actual: %d\n", self->current_block);
    printf("Pagina Actual: %d\n", self->current_page);
    printf("Celda Actual: %d\n", self->current_cell);
    printf("Byte Actual: %d\n", self->current_byte);
    printf("\nByte Cargados: %d\n", self->bytes_loaded_count);
    printf("Byte faltantes de Leer: %d\n\n", self->remaining_bytes);

    fclose(opened_file);
}

void add_block_to_index(osFile* self, int new_block) {
    FILE *file = fopen(global_diskname, "rb");
    fseek(file , BLOCK_SIZE*(self->block_index_number) + 4*self->amount_of_blocks, SEEK_SET);
    fwrite(new_block, 1,1,file);
    fclose(file);
}

void print_index_block(osFile* self) {
    FILE *file = fopen(global_diskname, "rb");
    fseek(file , BLOCK_SIZE*(self->block_index_number), SEEK_SET);
    printf("\nImprimiendo bloque indice: %d\n",self->block_index_number);
    for (int i = 0; i < 256*2; i++) {
        int buffer; // see leen ints de 4 bytes
        fread(&buffer, sizeof(int), 1, file); // Leo una entrada de un int
        printf(" %d", buffer);
    }
    printf("\n");
    fclose(file);
}

/// Libera la memoria de todo lo asociado al struct. Luego libera la memoria del struct mismo.
void osFile_destroy(osFile* self) {
    // Libero memoria puntero nombre
    free(self);
}

/// ------------------------------------------
/// Cosas que fui necesitando en el camino
/// ------------------------------------------

bool fxExtra_revisar_modo(osFile* file) {
    if (strcmp(file->mode, "r") == 0 || strcmp(file->mode, "R") == 0) {
        return true;

    } else {
        return false;
    }
}

void fxExtra_hacer_el_setup(osFile* file) {
    // Creo que al final no va nada aquí
}

int fxExtra_calc_max_bytes_lectura(osFile* file, int nbytes) {
    int bytes_que_me_quedan_por_leer;
    int byes_que_me_dicen_que_lea;

    byes_que_me_dicen_que_lea = nbytes;
    bytes_que_me_quedan_por_leer = file->remaining_bytes;

    if (byes_que_me_dicen_que_lea > bytes_que_me_quedan_por_leer) {
        return bytes_que_me_quedan_por_leer;

    } else {
        return byes_que_me_dicen_que_lea;
    }
}

char* fxExtra_reservar_mem_char(int cantidad) {
    char* memoria = malloc(cantidad);
    return memoria;
}

void* fxExtra_reservar_mem_void(int cantidad) {
    void* memoria = malloc(cantidad);
    return memoria;
}

void fxExtra_liberar_mem_char(char* en_donde) {
    free(en_donde);
}

void fxExtra_liberar_mem_void(void* en_donde) {
    free(en_donde);
}

int fxExtra_nro_pagina_que_tengo_que_leer(osFile* file) {
    // TODO:
    //  [X] Ver dónde estoy parado
    //  [X] Calcular página según longitud
    //  [F] Meterme al índice y armar un array de bloques
    //  [F] Armar un array de página involucradas
    //  [F] Recorrer página por página y agregarla a un nuevo array si no está rotten
    //  [-] Mandar nro de página absoluta, incluyendo rotten
    //      [X] Mándo nro de pagina

    long int tamanio = file->length;
    int donde_voy_leyendo = file->bytes_loaded_count;
    int mi_paginita = donde_voy_leyendo / PAGE_SIZE;

    return mi_paginita;
}

void fxExtra_cargar_pagina_en_mem(osFile* file, char* dir_pagina, int nro_pag) {
    // TODO:
    //  [ ] Ver dónde estoy parado
    //  [ ] Calcular página según longitud
    //  [ ] Calcular nro de bloques que tiene
    //  [ ] Calcular nro de bloque en el que voy
    //  [ ] Meterme al índice y armar un array de bloques
    //  [ ] Armar un array de página involucradas
    //  [ ] Recorrer página por página y agregarla a un nuevo array si no está rotten
    //  [ ] Avanzar pág por pág hasta la que necesito
    //  [ ] Cargar la que necesito en memoria




}

bool quedan_bytes_por_leer(osFile* file) {
    if (file->remaining_bytes <= 0) {
        return false;

    } else {
        return true;
    }
}

void copiar_byte(osFile* file, char* desde, void* hacia, int en_donde_voy) {
    // TODO:
    //  [ ] Mover la posición en_donde_voy desde desde hacia hacia
}

void avanzar_contador_archivo_y_actualizar_pos(osFile* file) {
    // TODO:
    //  [X] Actualizar cuenta de bytes leidos
    //  [ ] Ir al índice y ver bloques
    //  [ ] Cargar array de bloques desde el índice
    //  [ ] Armar array de nro páginas absolutas
    //  [ ] Armar array de igual largo al anterior
    //  [ ] Definir contador = nro páginas en array
    //  [ ] Copiar nros páginas buenas y restar cant de rotten en el contador
    //  [ ] Calcular plano, bloque, página, celda y byte donde voy en relación a bytes_loaded_count
    //  [ ] Actualizar valores en obj.

    file->bytes_loaded_count++;

}

void reducir_bytes_restantes(osFile* file) {
    file->remaining_bytes--;
}
