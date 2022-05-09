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

#include "./os_API.h"

// ===== API de ssdfs =====


// ----- Funciones generales -----

/* Monta el disco virtual
 * Establece como variable global el archivo .bin correspondiente al disco. Define como
 * lı́mite de ciclos P/E al valor de life. La función debe poder ser llamada múltiples
 * veces si se desea abrir diferentes discos a lo largo de la ejecución de main.c.*/
void os_mount(char* diskname, unsigned life) {
    /* Crea una variable global con el nombre del archivo y otra con el
     * valor de life */
    strcpy(global_diskname, diskname);
    global_P_E = life;
    unactualized_change = 0;
}

/* Imprime el valor del bitmap para el bloque num.
 * Si num=0 se debe imprimir t */
void os_bitmap(unsigned num) {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // El disco tiene 2048 bloques, por lo que para el bitmap necesitamos
    // 2048 bits = 256 bytes
    unsigned char buffer[256]; // Buffer para guardar los bytes
    fread(buffer, sizeof(buffer), 1, f);

    if (num == 0) {
        printf("\nBitmap del Disco\n");

        int fill = 0;
        int free = 0;

        for (int i = 0; i < 256; i++) {
            for (int j = 7; j >= 0; j--) {
                int bit = (buffer[i] & (1 << j)) >> j; // Shift left para sacar el bit
                printf("%d", bit );
                bit ? fill++ : free++; // Se ve más cool así
            }
        }

        printf("\nBloques Ocupados: %d\nBloques Libres: %d\n", fill, free);

    } else if (num > 0 && num < 2048) {
        printf("\nBitmap Bloque N°%d\n", num);
        // num / 8 es el byte donde se encuentra el bit deseado
        // num % 8 es el offset del bit dentro de ese byte
        printf("%d\n", (buffer[num / 8] & 1 << (7 - num % 8)) >> (7 - num % 8));

        // En el momento 15:35 de la cápsula P1 dice que esto hay que entregarlo
        // aunque el argumento no sea 0
        int fill = 0;
        int free = 0;

        for (int i = 0; i < 256; i++) {
            for (int j = 7; j >= 0; j--) {
                int bit = (buffer[i] & (1 << j)) >> j; // Shift left para sacar el bit
                bit ? fill++ : free++; // Se ve más cool así
            }
        }

        printf("Bloques Ocupados: %d\nBloques Libres: %d\n", fill, free);

    } else {
        printf("\nBitmap Bloque N°%d\n", num);
        printf("%s\n", "SEGFAULT uwu");
    }

    fclose(f); // Evitamos leaks
}

/* Imprime el estado P/E de las páginas desde lower y upper-1. Si ambos valores son -1,
 * se debe imprimir el lifemap completo. Además se debe imprimir en una segunda lı́nea la
 * cantidad de bloques rotten y la cantidad de bloques saludables. */
void os_lifemap(int lower, int upper) {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 1 MiB, para llegar al bloque N°1, de directorio.
    fseek(f, 1 * BLOCK_SIZE, SEEK_SET);

    if (upper > PAGES_PER_DISK || lower < -1 || lower > PAGES_PER_DISK || upper < -2 ) {
        printf("Error de input para os_lifemap\n");
        return;
    }

    if (lower  == -1 && upper == -1) {
        upper = PAGES_PER_DISK;
        lower = 0;
    }

    int rotten_blocks = 0;
    int total_blocks = 0;
    int rotten_found = 0;
    int block_visited = 0;
    // Son 524288 paginas entre los 2 planos, por lo que recorremos 524288 numeros
    // Son 4096 bloques en el disco
    for (int i = 0; i < PAGES_PER_DISK; i++) {
        int buffer; // see leen ints de 4 bytes
        fread(&buffer, sizeof(int), 1, f); // Leo una entrada de un int

        if ( lower < i && i < upper) {
            printf(" %d", buffer);
            block_visited = 1;
        }

        if (i % 256 == 0 && block_visited == 1){
          // Se suman las condiciones de bloque visitado
            rotten_blocks += rotten_found;
            total_blocks++;
            rotten_found = 0;
            block_visited = 0;
        }
        if (buffer == -1) {
            rotten_found = 1;
        }
    }
    printf("\nCantidad de bloques rotten: %d", rotten_blocks);
    printf("\nCantidad de bloques sanos: %d\n", total_blocks - rotten_blocks);
    fclose(f); // Evitamos leaks
}

/* Esta función debe recorrer el disco completo. Para cada bloque que contenga páginas
 * cuyo valor P/E se encuentra a limit ciclos de pasar a estado rotten, reubicarla a un
 * bloque que no contenga páginas en esta condición. Esta operación no debe corromper
 * archivos ni directorios, por lo que mover un bloque implica actualizar todos los
 * punteros que sea necesario para no perder su referencia. En caso de que no hayan
 * suficientes bloques disponibles para realizar cualquiera de estas operaciones, se debe
 * indicar la cantidad de estos, y además indicar que archivos o directorios se podrı́an
 * ver afectados por pérdida de información en limit ciclos. Esta función retorna el
 * número bloques que fueron reubicados exitosamente. */
int os_trim(unsigned limit) {  // TODO: Pendiente
    return 0;
}

/* Función para imprimir el árbol de directorios y archivos del sistema, a partir del
 * directorio base. */
void os_tree(){
    //// NOTE: Moví directree a ./aux/directree.*:aux_directree    - Luis
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(f, 3 * BLOCK_SIZE, SEEK_SET);

    printf("~\n"); // root
    int depth = 1; // Para cachar que tan profundo estoy

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE];
        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada

        if (buffer[0] == 1) { // directorio:
            for (int k = 0; k < depth; k++) { // Desplazar depth a la derecha
                printf("| ");
            }
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del directorio
                if (buffer[j] == 0){
                    break;
                } else {
                    printf("%c", buffer[j]);
                }
            }
            printf("\n");
            int *puntero;
            puntero = &buffer[1];
            depth++; // Subo la profundidad en 1
            directree(*puntero, depth, global_diskname); // Llamada recursiva
            depth--; // Vuelvo a la profundidad anterior
        } 
        
        else if (buffer[0] == 3) { // archivo:
            for (int k = 0; k < depth; k++) {
                printf("| ");
            }
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del archivo
                if (buffer[j] == 0){
                    break;
                } else {
                    printf("%c", buffer[j]);
                }
            }
            printf("\n");
        }
    }

    fclose(f); // Evitamos leaks
}

// ----- Funciones de manejo de archivos -----
/* Permite revisar si un archivo existe o no. Retorna 1 en caso de que exista, 0 de caso
 * contrario. */
int os_exists(char* filename) {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, de directorio.
    fseek(f, BLOCK_SIZE * 3, SEEK_SET);

    // Son 32768 entradas en un bloque de directorio
    for (int i = 0; i < DIR_ENTRIES_PER_BLOCK; i++) {
        unsigned char buffer[DIR_ENTRY_SIZE];
        
        // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada

        if(buffer[0] == 3){ // archivo:
            char path[100] = "/"; // path inicial
            char aux[2]; // variable para concatenar char

            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del archivo
                if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path, aux); // Concatenar char   
                }
            }
            if (strcmp(path, filename) == 0) { // compara con filename
                fclose(f); // Evitamos leaks
                return 1;
            }
        }

        else if (buffer[0] == 1) { // directorio:
            char path[100] = "/"; // path inicial
            char aux[2]; // variable para concatenar char
            for (int j = 5; j < DIR_ENTRY_SIZE; j++) { // Printear nombre del directorio
                if (buffer[j] == 0){
                    aux[1] = '\0';
                    aux[0] = '\0';
                    strcat(path, aux); // Concatenar char
                    break;
                } else {
                    aux[1] = '\0';
                    aux[0] = buffer[j];
                    strcat(path, aux); // Concatenar char   
                }
            }
            strcat(path, "/");
            int *puntero;
            puntero = &buffer[1];
            if (find_file(*puntero, filename, path)){// Función recursiva para leer
                fclose(f); // Evitamos leaks
                return 1;
            }
        }
    }

    fclose(f); // Evitamos leaks
    return 0;
}

/* Esta función abre un archivo. Si mode='r', se busca el archivo filename y se retorna el
 * osFile* que lo representa. Si mode='w', se verifica que el archivo no exista, y se
 * retorna un nuevo osFile* que lo representa. */
osFile* os_open(char* filename, char mode) {  // NOTE: En proceso
    if (mode =='r') {
        if (os_exists(filename)) {
            printf("(Lectura) Encuentra archivo. return osFile.\n");
            osFile* os_file = osFile_new(filename, mode);
            return os_file;
        } else {
            printf("(Lectura) No encuentra archivo. return NULL.\n");
            return NULL;
        }
    }
    if (mode == 'w') {
        if (os_exists(filename)) {
            printf("(Escritura) Encuentra archivo. return NULL.\n");
            return NULL;
        } else {
            /// PATH DIR
            char** splitpath = calloc(2, sizeof(char*));
            int index = 0;

            int pathleng = strlen(filename);
            char path[pathleng+1];
            strcpy(path, filename);

            char* token = strtok(path, "/");
            while(token != NULL)
            {
                splitpath[index] = calloc(4096, sizeof(char));
                strcpy(splitpath[index++], token);
                token = strtok(NULL, "/");
            }
            
            char* filename2 = splitpath[index-1];
            int leng = strlen(filename2);
            path[pathleng-leng] = '\0';

            for(int i=0;i<index;i++){
                free(splitpath[i]);
            }
            free(splitpath);
            /// PATH DIR
            
            if(dir_exists(path)){
                printf("(Escritura) No encuentra archivo y existe directorio. return osFile.\n");
                osFile* os_file = osFile_new(filename, mode);
                return os_file;
            }else{
                printf("(Escritura) No encuentra archivo y no existe directorio. return NULL.\n");
                return NULL;
            }
            return NULL;


        }
    }
    return NULL;
}

/* Esta función sirve para leer archivos. Lee los siguientes nbytes desde el archivo
 * descrito por file desc y los guarda en la dirección apuntada por buffer. Debe retornar
 * la cantidad de Bytes efectivamente leı́dos desde el archivo. Esto es importante si
 * nbytes es mayor a la cantidad de Bytes restantes en el archivo o en el caso que el
 * archivo contenga páginas rotten. La lectura de read se efectúa desde la posición del
 * archivo inmediatamente posterior a la última posición leı́da por un llamado a read. */
int os_read(osFile* file_desc, void* buffer, int nbytes) {  // REVIEW
    // int page_offset;
    // int* rotten_pages;

    // int starting_page_byte;
    // int end_page_byte;
    // int reading_delta;
    // int amount_read = 0;

    // int buffer_counter = 0;

    // if (strcmp(file_desc->mode, "r") != 0) {
    //     printf("Archivo no está abierto en modo lectura");
    //     return 0;
    // }

    // // Caso borde: nbytes = 0 ==> No se lee ningún byte
    // if (nbytes == 0) {
    //     return 0;
    // }

    // // Reviso las páginas rotten.
    // rotten_pages = calloc(PAGES_PER_BLOCK, sizeof(bool));

    // page_offset = file_desc->current_plane * BLOCKS_PER_PLANE * PAGES_PER_BLOCK;
    // page_offset += file_desc->current_block * PAGES_PER_BLOCK;

    // for (int n_page = 0; n_page < PAGES_PER_BLOCK; n_page ++) {
    //     rotten_pages[n_page] = is_page_rotten(n_page + page_offset,
    //                                           global_diskname);
    // }

    // // Reseteo cuenta de bytes leídos para hacer la comparación
    // osFile_reset_bytes_count(file_desc);

    // // Mientras que me queden bytes por leer debo seguir avanzando loopea
    // while (nbytes > 0) {
    //     // (nbytes - 1 // page_size) + 1 = Páginas por leer
    //     // Usa la función piso/división parte entera, por eso el +-1
    //     // Y como solo se pueden leer páginas como número entero...
    //     osFile_load_pointer_page(file_desc, rotten_pages);

    //     // Inicio y fin de lectura de la página
    //     starting_page_byte = file_desc->current_pos % PAGE_SIZE;

    //     if (nbytes >= PAGE_SIZE) {
    //         end_page_byte = PAGE_SIZE;

    //     } else {
    //         end_page_byte = nbytes;
    //     }

    //     // Cant. de bytes leídos
    //     reading_delta = end_page_byte - starting_page_byte + 1;

    //     // Sustraigo bytes efectivamente leídos
    //     nbytes -= reading_delta;

    //     // Check de largo de archivo
    //     if (file_desc->current_pos + reading_delta > file_desc->length) {
    //         reading_delta = file_desc->length - file_desc->current_pos;
    //         end_page_byte = starting_page_byte + reading_delta;
    //     }

    //     // Bytes realmente leidos
    //     amount_read += reading_delta;

    //     // Cargo bytes a heap
    //     osFile_load_data(file_desc, starting_page_byte, end_page_byte);


    //     // Copio heap a buffer byte por byte
    //     for (int byte = 0; byte < reading_delta; byte++) {
    //         // BUG: No sé cómo copiar un byte de heap (unsigened char*) a buffer (void*)
    //         //  "Incomplete type 'void' is not assignable"
    //         //  La siguiente línea representa la idea de lo que quiero hacer.
    //         //  void* buffer?!?!?!!?
    //         // buffer[buffer_counter] = file_desc->loaded_data[byte];
    //         buffer_counter++;
    //     }
    // }

    // // MEM leak := feo
    // // :(
    // free(rotten_pages);

    return 0;
}

/* Esta función permite escribir un archivo. Escribe en el archivo descrito por file desc
 * los nbytes que se encuentren en la dirección indicada por buffer. Retorna la cantidad
 * de Bytes escritos en el archivo. Si se produjo un error porque no pudo seguir
 * escribiendo, ya sea porque el disco se llenó, ya sea porque existen demasiadas páginas
 * rotten o porque el archivo no puede crecer más, este número puede ser menor a nbytes
 * (incluso 0). Esta función aumenta en 1 el contador P/E en el lifemap asociado a cada
 * página que se escriba. */
int os_write(osFile* file_desc, void* buffer, int nbytes) {  // NOTE: En proceso
    if (file_desc->mode ==  'w') {
        printf("Error: El archivo debe estar en modo write.\n");
        return 0;
    } 

    if (nbytes % 2){
        printf("No es par aaaaaaaa");
        nbytes ++; //????    
    }
    int bloques_necesarios = nbytes/BLOCK_SIZE;
    if (nbytes%BLOCK_SIZE != 0){
        bloques_necesarios ++;
    }

    // Abrimos el disco
    FILE *opened_file = fopen(global_diskname, "rb");
    int data_block;
    int puntero_buffer = 0;
    int writed_bytes = 0;
    printf("Comienzo de  for\n");
    for (int bloque = 0; bloque < bloques_necesarios; bloque ++){
        
        data_block = get_usable_block();
        if (data_block == -1){
            break;
            //TODO: Asumiré por mientras que esto no pasa
        }
        
        // todos los cachos de añadir un nuevo bloque al archivo
        mark_as_used(data_block); //bitmap
        int direccion = BLOCK_SIZE*file_desc->block_index_number + 8 + file_desc ->amount_of_blocks*4; // 
        fseek(opened_file , direccion, SEEK_SET); 
        fwrite(data_block, 4, 1, opened_file); // escribimos el numero de bloque en el indice //BUG: Esta linea no hace lo que debería
        printf("Añadido nuevo bloque al indice es el numero: %d  en la posición: %d", data_block);
        file_desc ->amount_of_blocks ++; // añado 1 al contador de bloques
                
        fseek(opened_file , BLOCK_SIZE*data_block, SEEK_SET); 
        // Escribimos los bytes corerspndientes a casa página
        int dif = nbytes-puntero_buffer;
        for (int i=0; i < min(256, dif); i++ ){
            char *puntero;
            puntero = &buffer[puntero_buffer];
            fwrite(*puntero, 4, 1, opened_file); //BUG: Asunmo que esta no esta haciendo lo que deberia
            puntero_buffer ++;
            writed_bytes ++;
            // TODO: Actualizar lifema
        }
        if (puntero_buffer >= nbytes){
            break;
        }
        printf("Quedan bytes\n");
        printf("puntero_buffer: %d\n", puntero_buffer);
        printf("nbytes: %d\n", nbytes);
        data_block = get_usable_block();
        mark_as_used(data_block); 
    }
    // imprimimos para chequear
    // Actualizar el directorio
    // Actualizamos el length
    fseek(opened_file , BLOCK_SIZE*file_desc->block_index_number, SEEK_SET); 
    file_desc->length = writed_bytes;
    // fwrite(25, sizeof(int), 1, opened_file); //BUG: Esta linea no hace lo que debería
    // fseek(opened_file , BLOCK_SIZE*file_desc->block_index_number - 1, SEEK_SET); 
    fwrite(25, sizeof(int), 1, opened_file); //BUG: Esta linea no hace lo que debería
    // fseek(opened_file , BLOCK_SIZE*file_desc->block_index_number + 1, SEEK_SET); 
    // fwrite(25, sizeof(int), 1, opened_file); //BUG: Esta linea no hace lo que debería

    print_index_block(file_desc);


    

    fclose(opened_file);
    return 0;
}

/* Esta función permite cerrar un archivo. Cierra el archivo indicado por file desc. Debe
 * garantizar que cuando esta función retorna, el archivo se encuentra actualizado en
 * disco.*/
int os_close(osFile* file_desc) {
    osFile_destroy(file_desc);
    return 0;
}

/* Esta función elimina el archivo indicado por filename. El bloque de ı́ndice del archivo
 * debe ser borrado (todos sus bits puestos en 0), lo que aumenta en 1 el contador P/E
 * asociado a dichas páginas. También se deber actualizar la página del bloque de
 * directorio que contenı́a el puntero a dicho ı́ndice, lo que también incrementa su
 * contador P/E en 1. */
int os_rm(char* filename) {  // TODO: Pendiente
    return 0;
}

/* Esta función crea un directorio con el nombre indicado. Esto incrementa en 1
   el contador P/E de las páginas que sea necesario actualizar 
   para crear las referencias a este directorio. */
int os_mkdir(char* path) {  // TODO: Pendiente
    // Función auxiliar que busca el primer bloque vacío
    int blocksearch(){
        // Cargo el bitmap
        FILE *f = fopen(global_diskname, "rb");
        unsigned char buffer[256];
        fread(buffer, sizeof(buffer), 1, f);
        int bloque = 0;
        for(int i = 0; i < 256; i++){
            for (int j = 7; j >= 0; j--){
                // Shift left para sacar el bit
                int bit = (buffer[i] & (1 << j)) >> j;
                // Si el bit es 1 sigo buscando, si no, retorno
                if(bit){
                    bloque++;
                } else {
                    fclose(f);
                    return bloque;
                } 
            }
        }
        fclose(f);
        return 0; // Si no hay bloques disponibles
    }

    int bloquel = blocksearch();
    printf("El primer bloque disponible es: %i\n", bloquel);

    char** splitpath = calloc(2, sizeof(char*));
    int index = 0;

    int pathleng = strlen(path);
    char pathto[pathleng]; strcpy(pathto, path);

    ///// PATHSEARCH START
    char* token = strtok(path, "/");
    while(token != NULL)
    {
        splitpath[index] = calloc(4096, sizeof(char));
        strcpy(splitpath[index++], token);
        token = strtok(NULL, "/");
    }
    // Remove dangling Windows (\r) and Unix (\n) newlines
    int len = strlen(splitpath[index - 1]);
    if (len > 1 && splitpath[index - 1][len - 2] == '\r')
        splitpath[index - 1][len - 2] = '\0';
    else if (len && splitpath[index - 1][len - 1] == '\n')
        splitpath[index - 1][len - 1] = '\0';
    ///// PATHSEARCH END

    char* filename = splitpath[index-1];
    int leng = strlen(filename);
    pathto[pathleng-leng] = '\0'; 

    printf("El path es: %s\n", pathto);
    printf("El nombre es: %s\n", filename);

    //writeblock = os_find(pathto); // Se asume que esto funciona
    FILE *f = fopen(global_diskname, "rb");
    //fseek(f2, writeblock * 1048576, SEEK_SET);
    //char* texto = "AAA"; // Lo que voy a escribir
    fclose(f);
    
    return 0;
}

/* Esta función elimina un directorio vacı́o con el nombre indicado. Esto incrementa en 1
 * el contador P/E de las páginas que sea necesario actualizar para borrar las referencias
 * a este directorio. */
int os_rmdir(char* path) {  // TODO: Pendiente
    return 0;
}

/* Esta función elimina un directorio con el nombre indicado, todos sus archivos y
 * subdirectorios correspondientes. Esto incrementa en 1 el contador P/E de las páginas
 * que sea necesario actualizar para borrar las referencias a este directorio. */
int os_rmrfdir(char* path) {  // TODO: Pendiente
    return 0;
}

/* Esta función que se encarga de copiar un archivo o carpeta referenciado por orig hacia
 * un nuevo archivo o directorio de ruta dest en su computador. */
int os_unload(char* orig, char* dest) {  // TODO: Pendiente
    return 0;
}

/* Esta función que se encarga de copiar un archivo o los contenidos de una carpeta,
 * referenciado por orig al disco. En caso de que un archivo sea demasiado pesado para el
 * disco, se debe escribir tanto como sea posible hasta acabar el espacio disponible.
 * En caso de que el sea una carpeta, se deben copiar los archivos que estén dentro de
 * esta carpeta, ignorando cualquier carpeta adicional que tenga. Esta función debe
 * actualizar el lifemap según corresponda. */
int os_load(char* orig) {  // TODO: Pendiente
    return 0;
}

// Temporal ----- Esta función es para testear
// Tira los nombres de todo lo que hay en el disco
void print_names() {
    // Abro el archivo
    FILE *f = fopen(global_diskname, "rb");

    // Me muevo 3 MiB, para llegar al bloque N°3, del directorio base.
    int offset = 3 * BLOCK_SIZE; // 3MiB
    fseek(f, offset, SEEK_SET);

    // root está en el bloque 3 por convención, por lo que si
    // hubiese que moverlo para que no se pudra, se perdería para siempre

    // (La entrada 1672 tiene un archivo)
    // Son 32768 entradas en el bloque de directorio
    int max_buffer_size = 32;
    int entries = max_buffer_size * 1024;

    for (int i = 0; i < entries; i++) {
        unsigned char buffer[max_buffer_size]; // Buffer para guardar los bytes de una entrada
        fread(buffer, sizeof(buffer), 1, f); // Leo una entrada

        if (buffer[0]) { // Si hay archivo o directorio:
            printf("Primer byte entrada %i: %i\n", i, buffer[0]);

            // Printear nombre del archivo
            for (int j = 5; j < 32; j++) {
                printf("%c", buffer[j]);
            }

            printf("\n");
        }
    }

    fclose(f); // Evitamos leaks
}
