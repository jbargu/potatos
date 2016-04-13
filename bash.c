#include <bash.h>
#include <vga.h>
#include <kb.h>
#include <isrs.h>

void keyboard_callback(unsigned char key) {
    terminal_putchar(key);
}

void init_bash() {
    keyboard_initialize(keyboard_callback);

    terminal_writestring("Welcome to PotatOS, the best OS under then sun!");
    terminal_writestring("$");

    while(1);
}
