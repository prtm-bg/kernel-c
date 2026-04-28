#include <stdbool.h>    // for bool datatype
#include <stddef.h>     // for size_t, NULL
#include <stdint.h>     // for intx_t and uintx_t types

#if defined(__linux__)
#error "WARNING: Not using a cross-compiler!! You will most certainly encounter problems :P"
#endif

#ifndef __i386__
#error "WARNING: This kernel needs to be compiled with a i686-elf compiler!"
#endif 


/* H/W txt mode colors */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15
};

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

/* Global variables */
size_t term_row;
size_t term_col;
uint8_t term_color;
volatile uint16_t* term_buffer = (uint16_t*)VGA_MEMORY;


/* VGA entry functions */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;    /* final color value has 4-bits fg in the lower nibble and 4-bits bg in the upper nibble */
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;   /* final entry value has char in the LSB and color in the MSB */
}


/* String length function */
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}


/* Terminal functions */
void terminal_init(void) {
    term_row = 0;
    term_col = 0;
    term_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            /* as each cell is in a buffer */
            size_t ind = y * VGA_WIDTH + x;
            term_buffer[ind] = vga_entry(' ', term_color);
        }
    }
}

inline void terminal_setcolor(uint8_t color) {
    term_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    size_t ind = y * VGA_WIDTH + x;
    term_buffer[ind] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        term_col = 0;
        if (++term_row == VGA_HEIGHT) {
            term_row = 0;
        }
        return;
    }
    
    terminal_putentryat(c, term_color, term_col, term_row);
    if (++term_col == VGA_WIDTH) {
        term_col = 0;
        if (++term_row == VGA_HEIGHT) {
            term_row = 0;
        }
    }
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
    }
}

inline void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}


/* Kernel main function */
void kernel_main(void) {
    terminal_init();
    terminal_writestring("Bablu Bhai incoming!!\n");    
}