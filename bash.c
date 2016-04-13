#include <bash.h>
#include <vga.h>
#include <kb.h>
#include <isrs.h>

#define MAX_LENGTH 64

struct command {
    char text[MAX_LENGTH];
    int len;
};

struct command curr_command;

void keyboard_callback(unsigned char key) {
    if (key == '\b' && curr_command.len > 0)
	curr_command.len--;
    else if (key != '\b') {
	if (curr_command.len < MAX_LENGTH) {
	    curr_command.text[curr_command.len] = key;
	    curr_command.len++;
	}
    }

    terminal_clear_row(curr_terminal_row());
    print_prompt();
    for(int i = 0; i < curr_command.len; i++)
	terminal_putchar(curr_command.text[i]);
}

void print_prompt() {
    terminal_writestring("$ ");
}

void init_bash() {
    keyboard_initialize(keyboard_callback);

    curr_command.len = 0;

    terminal_writestring("Welcome to PotatOS, the best OS under then sun!\n");

    while(1);
}
