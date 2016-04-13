#include <vga.h>

// Combine foreground and background color.
uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

// Whole package. Append color to char.
uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

// String length.
size_t strlen(const char* str) {
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

// Initialize terminal.
void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

// Set color.
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

// Insert given character on specified position.
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

// Print out character.
void terminal_putchar(char c) {
	if (c == '\n') {
		terminal_column = 0;
		if(++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
	else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
		}
	}
}

// Print out string.
void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}

// Returns current row.
size_t curr_terminal_row() {
	return terminal_row;
}

// Returns current column.
size_t curr_terminal_column() {
	return terminal_column;
}

// Clears given row.
void terminal_clear_row(size_t row) {
	for (int i = 0; i < VGA_WIDTH; i++)
		terminal_putentryat(' ', terminal_color, i, row);
	terminal_column = 0;
}

// Set row.
void terminal_setrow(size_t row) {
	if (row > VGA_HEIGHT)
		row = VGA_HEIGHT - 1;
	terminal_row = row;
}

// Set column.
void terminal_setcolumn(size_t column) {
	if (column > VGA_WIDTH)
		column = VGA_WIDTH - 1;
	terminal_column = column;
}
