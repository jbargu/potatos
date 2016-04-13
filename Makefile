IDIR=./include
CC=i686-elf-gcc
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(IDIR)

ASS=nasm
ASSFLAGS=-felf32
LDFLAGS=-T linker.ld -ffreestanding -O2 -nostdlib -lgcc
ASSEMBLY_SOURCES=boot.asm
ASSEMBLY_OBJECTS=$(ASSEMBLY_SOURCES:.asm=.o)
C_SOURCES=kernel.c vga.c gdt.c idt.c isrs.c irq.c kb.c
C_OBJECTS=$(C_SOURCES:.c=.o)
KERNEL=potatos.bin
ISO=potatos.iso
QEMU=qemu-system-i386


all: $(KERNEL)

$(KERNEL): $(ASSEMBLY_OBJECTS) $(C_OBJECTS)
	$(CC) $(LDFLAGS) $(ASSEMBLY_OBJECTS) $(C_OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(ASS) $(ASSFLAGS) $< -o $@

run: all
	$(QEMU) -kernel $(KERNEL)

cdrom: all
	\cp $(KERNEL) isodir/boot
	grub-mkrescue -o $(ISO) isodir

run-cd: cdrom
	$(QEMU) -cdrom $(ISO)

clean:
	rm -f $(ASSEMBLY_OBJECTS) $(C_OBJECTS) $(KERNEL) $(ISO)

