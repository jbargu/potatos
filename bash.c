#include <bash.h>
#include <vga.h>
#include <kb.h>
#include <isrs.h>
#include <string.h>
#include <stdlib.h>
#include <rtc.h>

#define MAX_LENGTH 64

struct command {
    char text[MAX_LENGTH+1];
    int len;
};

struct command *curr_command;

// Cowsay program.
void cowsay() {
    char *message = "I like PotatOS. It's very good!";
    int len = strlen(message);
    clear_screen();
    terminal_putchar(' ');
    for (int i = 0; i < len; i++)
	terminal_putchar('-');
    terminal_putchar('\n');
    terminal_putchar('<');
    terminal_writestring(message);
    terminal_putchar('>');
    terminal_putchar('\n');
    terminal_putchar(' ');
    for (int i = 0; i < len; i++)
	terminal_putchar('-');
    terminal_putchar('\n');
    terminal_writestring("      \\   ^__^\n");
    terminal_writestring("       \\  (oo)\\_______\n");
    terminal_writestring("          (__)\\       )\\/\\\n");
    terminal_writestring("             ||----w |\n");
    terminal_writestring("             ||     ||\n");
    terminal_putchar('\n');

}

// Rainbow program.
void rainbow() {
    clear_screen();
    char *message = "Feel the rainbow!";
    for (int k = 0; k < 16; k++) {
	uint8_t color = make_color(15-k, k);
	terminal_setcolor(color);
	terminal_putchar(message[k]);
    }
    terminal_putchar('\n');
    for (int i = 0; i < 16; i++) {
	for (int k = 0; k < 16; k++) {
	    uint8_t color = make_color(k, k);
	    terminal_setcolor(color);
	    terminal_putchar(' ');
	}
	terminal_putchar('\n');
    }
    terminal_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK));
}

// Execute given command.
void execute_command() {
    curr_command->text[curr_command->len] = '\0';
    /*terminal_writestring("Command executed\n");*/
    if(!strcmp("uname", curr_command->text)) {
	terminal_writestring("PotatOS v0.0.1\n");
    }
    else if(!strcmp("cowsay", curr_command->text)) {
	cowsay();
    }
    else if(!strcmp("clock", curr_command->text)) {
	// Be gentle with clock stuff!
	terminal_writestring(get_time());
	terminal_putchar('\n');
    }
    else if(!strcmp("rainbow", curr_command->text)) {
	rainbow();
    }
    else {
	terminal_writestring("No such command\n");
    }



}

void keyboard_callback(unsigned char key) {
    if (key == '\n') {
	terminal_writestring("\n");
	execute_command();
	curr_command->len = 0;
	print_prompt();
    }
    else {
	if (key == '\b' && curr_command->len > 0)
	    curr_command->len--;
	else if (key != '\b') {
	    if (curr_command->len < MAX_LENGTH) {
		curr_command->text[curr_command->len] = key;
		curr_command->len++;
	    }
	}
	terminal_clear_row(curr_terminal_row());
	print_prompt();
	for(int i = 0; i < curr_command->len; i++)
	    terminal_putchar(curr_command->text[i]);
    }

}

void print_prompt() {
    terminal_writestring("$ ");
}

void init_bash() {
    keyboard_initialize(keyboard_callback);

    curr_command = (struct command *)malloc(sizeof(struct command));
    curr_command->len = 0;

    terminal_writestring("Welcome to PotatOS, the best OS under then sun!\n");
    print_prompt();

    while(1);
}
