#pragma once

char global_diskname[1023];
int global_P_E;

void os_mount(char* diskname, unsigned life);
void os_bitmap(unsigned num);
void os_tree();
void print_names();
