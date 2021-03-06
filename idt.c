#include<idt.h>

struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

/* Set an IDT entry with this function. */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Our very homemade memset function */
void *memset(void *adr, int c, int n) {
    char *a = (char *)adr;
    for(int i = 0; i < n; i++)
	a[i] = c;
    return adr;
}

/* Installs the IDT */
void idt_initialize() {
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    // Initalize IDT to zeros
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Points the processor's internal register to the new IDT */
    idt_load();
}
