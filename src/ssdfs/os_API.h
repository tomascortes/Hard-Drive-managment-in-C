#pragma once

void os_bitmap(unsigned num, FILE* f);
char global_diskname[1023];
int global_P_E;
void os_mount(char* diskname, unsigned life);
