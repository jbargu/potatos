#ifndef ISRS_H
#define ISRS_H
/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    // pushed in isrX
    unsigned int eip, cs, eflags, useresp, ss;   // pushed automatically
};
void isrs_install();

#endif
