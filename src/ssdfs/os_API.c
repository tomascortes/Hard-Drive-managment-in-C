#include <stdio.h>
#include <string.h>
#include "./os_API.h"

void os_mount(char* diskname, unsigned life) {
    strcpy(global_diskname, diskname);
    global_P_E = life;
}

void os_bitmap(unsigned num) {
    printf("BITMAP P1!\n");
    FILE *f = fopen(global_diskname, "rb");

    unsigned char buffer[256];
    fread(buffer, sizeof(buffer),1,f);

    if (num == 0) {
        int fill = 0;
        int free = 0;

        for (int i = 0; i<256; i++) {
            for (int j = 7; j >= 0; j--) {
                int bit = (buffer[i] & (1 << j)) >> j;
                printf("%d", bit );

                if (bit) {
                    fill++;

                } else {
                    free++;
                }
            }
        }

        printf("\n");
        printf("Bloques Ocupados: %d\nBloques Libres: %d\n", fill, free);

    } else if (num>=0 && num<=2048) {
        printf("%d\n", num/8);
        printf("%d\n", num%8);
        printf("%d\n", (buffer[num/8] & (1 << (7-num%7)) >> num%8 ));

    } else {
        printf("%s\n", "SEGFAULT");
    }
    
    fclose(f);
}
