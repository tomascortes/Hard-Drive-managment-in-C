#include <stdio.h>
#include "os_API.h"

// FIXME: Second parameter of 'main' (argument array) must be type 'char **'
int main(int argc, char** const argv[]) {// Montar el disco pasado por consola con life = 5000
    os_mount(argv[1], 5000);

    os_bitmap(0); // Bitmap completo
    os_bitmap(2); // Bitmap bloque N°2 (Tercer bloque)
    os_bitmap(395); // Bitmap bloque N°395
    os_bitmap(404404); // Bitmap bloque inexistente

    os_tree();

    return 0;
}
