#include <stdio.h>
#include "os_API.h"


int main(int argc, char* const argv[])
{
  // Montar el disco pasado por consola con life = 5000
  os_mount(argv[1], 5000);

  os_bitmap(0); // Bitmap completo
  os_bitmap(2); // Bitmap bloque N°2 (Se espera 1)
  os_bitmap(7); // Bitmap bloque N°7 (Se espera 0)
  os_bitmap(8); // Bitmap bloque N°8 (Se espera 1)
  os_bitmap(395); // Bitmap bloque N°395 (Se espera 0)
  os_bitmap(404404); // Bitmap bloque inexistente (Se espera SEGFAULT)

  os_tree();

  return 0;
}
