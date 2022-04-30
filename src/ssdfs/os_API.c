#include "./os_API.h"
void os_mount(char* diskname, unsigned life){
    strcpy(global_diskname, diskname);
    global_P_E = life;
}