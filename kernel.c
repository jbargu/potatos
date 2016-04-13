#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <vga.h>
#include <gdt.h>
#include <idt.h>
#include <isrs.h>
#include <irq.h>
#include <kb.h>

void kernel_main() {
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	keyboard_install();
	terminal_initialize();

	asm("sti");


	/*uint8_t key = 0;*/
	/*asm("sti");*/
	/*asm("push ax");*/
	/*asm("mov ax, 0");*/
	/*asm("int 16");*/
	/*asm("pop ax");*/
	/*asm("mov %%ah, %0" : "=r"(0));*/
	/*getScancode();*/


	terminal_writestring("Welcome to PotatOS, the best OS under then sun!");

	while(1);
	// Test division by zero
	/*terminal_putchar('a' / 0);*/

}
