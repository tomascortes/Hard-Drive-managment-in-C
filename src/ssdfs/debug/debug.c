//Importar archivo
#include "./debug.h"

/// https://qnaplus.com/c-program-to-sleep-in-milliseconds/
void wait_debug(int time_ms) {
    if (DEBUG_MODE && SIMULATE_WAIT) {
        usleep(time_ms * 1000);
    }
}

void just_wait_debug() {
    if (DEBUG_MODE && SIMULATE_WAIT) {
        usleep(WAITING_TIME * 1000);
    }
}

void wait_longer_debug() {
    if (DEBUG_MODE && SIMULATE_WAIT) {
        usleep(WAITING_TIME2 * 1000);
    }
}

void wait_a_bit_debug() {
    if (DEBUG_MODE && SIMULATE_WAIT) {
        usleep(WAITING_TIME3 * 1000);
    }
}

// Printea "\n"
void dprint_line() {
    if (DEBUG_MODE) {
        printf("\n");
    }
}

// Printea "DEBUG: "
void dprint_txt() {
    if (DEBUG_MODE) {
        char *start_color = "\033[33;3;1m";
        char *end_color = "\033[0m";
        printf("%sDEBUG: %s", start_color, end_color);
    }
}

// Printea "DEBUG: "
void dprint_txt2() {
    if (DEBUG_MODE) {
        char *start_color = "\033[31;3;1m";
        char *end_color = "\033[0m";
        printf("%sDEBUG: %s", start_color, end_color);
    }
}

// Printea "**input"
void dprint_char_xx(char **input) {
    if (DEBUG_MODE) {
        char *start_color = "\033[32;3;1m";
        char *end_color = "\033[0m";
        printf("%s%s%s", start_color, *input, end_color);
    }
}

// Printea "**input"
void dprint_char_xx2(char **input) {
    if (DEBUG_MODE) {
        char *start_color = "\033[34;3;1m";
        char *end_color = "\033[0m";
        printf("%s%s%s", start_color, *input, end_color);
    }
}

// Printea "*input"
void dprint_char_x(char *input) {
    if (DEBUG_MODE) {
        char *start_color = "\033[36;3;1m";
        char *end_color = "\033[0m";
        printf("%s%s%s", start_color, input, end_color);
    }
}

// Printea "*input"
void dprint_char_x2(char *input) {
    if (DEBUG_MODE) {
        char *start_color = "\033[34;3;1m";
        char *end_color = "\033[0m";
        printf("%s%s%s", start_color, input, end_color);
    }
}

// Printea "DEBUG: **input\n"
void dprint_txt_char_xx(char **input) {
    if (DEBUG_MODE) {
        dprint_txt();
        dprint_char_xx(input);
        dprint_line();
    }
}

// Printea "DEBUG: **input\n"
void dprint_txt_char_xx2(char **input) {
    if (DEBUG_MODE) {
        dprint_txt2();
        dprint_char_xx2(input);
        dprint_line();
    }
}

// Printea "DEBUG: *input\n"
void dprint_txt_char_x(char *input) {
    if (DEBUG_MODE) {
        dprint_txt();
        dprint_char_x(input);
        dprint_line();
    }
}

// Printea "DEBUG: *input\n"
void dprint_txt_char_x2(char *input) {
    if (DEBUG_MODE) {
        dprint_txt2();
        dprint_char_x2(input);
        dprint_line();
    }
}


// Printea "pid"
void dprint_pid(pid_t process_id) {
    if (DEBUG_MODE) {
        char *start_color = "\033[33;3;1m";
        char *end_color = "\033[0m";
        printf("%s%d%s", start_color, process_id, end_color);
    }
}

// Printea "DEBUG: pid\n"
void dprint_txt_pid(pid_t process_id) {
    if (DEBUG_MODE) {
        dprint_txt();
        dprint_pid(process_id);
        dprint_line();
    }
}

// Printea "input"
void dprint_int(int input) {
    if (DEBUG_MODE) {
        char *start_color = "\033[35;3;1m";
        char *end_color = "\033[0m";
        printf("%s%i%s", start_color, input, end_color);
    }
}

// Printea "DEBUG: input\n"
void dprint_txt_int(int input) {
    if (DEBUG_MODE) {
        dprint_txt();
        dprint_int(input);
        dprint_line();
    }
}

// Printea "input"
void dprint_float(float input) {
    if (DEBUG_MODE) {
        char *start_color = "\033[31;3;1m";
        char *end_color = "\033[0m";
        printf("%s%f%s", start_color, input, end_color);
    }
}

// Printea "DEBUG: input\n"
void dprint_txt_float(float input) {
    if (DEBUG_MODE) {
        dprint_txt();
        dprint_float(input);
        dprint_line();
    }
}


void print_debug(char *input) {
    if (DEBUG_MODE) {
        dprint_txt_char_x(input);
    }
}
