#include <isrs.h>

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);

unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);

void irq_initialize();
