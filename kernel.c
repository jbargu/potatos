#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <vga.h>
#include <gdt.h>
#include <idt.h>
#include <isrs.h>
#include <irq.h>
#include <kb.h>
#include <bash.h>

void kernel_main() {
	gdt_initialize();
	idt_initialize();
	isrs_initialize();
	irq_initialize();
	terminal_initialize();

	// Enable interrupts.
	asm("sti");

	init_bash();
	while(1);

	// Test division by zero
	/*terminal_putchar('a' / 0);*/
}
