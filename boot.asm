; Declare constants used for creating a multiboot header.
; MAGIC NUMBERS WOOHOOO!!!
MBALIGN     equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MBALIGN | MEMINFO      ; this is the Multiboot 'flag' field
MAGIC       equ  0x1BADB002             ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum of above, to prove we are multiboot

; Declare a header as in the Multiboot Standard. We put this into a special
; section so we can force the header to be in the start of the final program.
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; Let's create our own stack, 16384 sweet sweet bytes.
section .bootstrap_stack, nobits
align 4
stack_bottom:
resb 16384
stack_top:

section .text
global _start
_start:
	; add stack to esp
	mov esp, stack_top

	; call kernel_main function from kernel.c
	extern kernel_main
	call kernel_main

	; In case the function returns, we'll want to put the computer into an
	; infinite loop. We disable interrupts as well. Basically CPU's in a
	; limbo.
	cli
.hang:
	hlt
	jmp .hang

; This will set up our new segment registers.
global gdt_flush
extern gp
gdt_flush:
    lgdt [gp]	      ; Load the GDT with gp pointer from gdt.c
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Reload it with
		      ; far jump
flush2:
    ret

; Loads the IDT defined in 'idtp' into the processor.
global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

;  0: Divide By Zero Exception
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_callback_common

;  1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_callback_common

;  2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_callback_common

;  3: Int 3 Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_callback_common

;  4: INTO Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_callback_common

;  5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_callback_common

;  6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_callback_common

;  7: Coprocessor Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_callback_common

;  8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push byte 8
    jmp isr_callback_common

;  9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_callback_common

; 10: Bad TSS Exception (With Error Code!)
isr10:
    cli
    push byte 10
    jmp isr_callback_common

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    cli
    push byte 11
    jmp isr_callback_common

; 12: Stack Fault Exception (With Error Code!)
isr12:
    cli
    push byte 12
    jmp isr_callback_common

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    cli
    push byte 13
    jmp isr_callback_common

; 14: Page Fault Exception (With Error Code!)
isr14:
    cli
    push byte 14
    jmp isr_callback_common

; 15: Reserved Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_callback_common

; 16: Floating Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_callback_common

; 17: Alignment Check Exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_callback_common

; 18: Machine Check Exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_callback_common

; 19: Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_callback_common

; 20: Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_callback_common

; 21: Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_callback_common

; 22: Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_callback_common

; 23: Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_callback_common

; 24: Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_callback_common

; 25: Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_callback_common

; 26: Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_callback_common

; 27: Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_callback_common

; 28: Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_callback_common

; 29: Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_callback_common

; 30: Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_callback_common

; 31: Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_callback_common


; C handler for handling exceptions (currently one for all).
extern fault_handler

; Handles all ISR routines.
isr_callback_common:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10 ; load data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, fault_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; 32: IRQ0
irq0:
    cli
    push byte 0
    push byte 32
    jmp irq_callback_common

; 33: IRQ1
irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_callback_common

; 34: IRQ2
irq2:
    cli
    push byte 0
    push byte 34
    jmp irq_callback_common

; 35: IRQ3
irq3:
    cli
    push byte 0
    push byte 35
    jmp irq_callback_common

; 36: IRQ4
irq4:
    cli
    push byte 0
    push byte 36
    jmp irq_callback_common

; 37: IRQ5
irq5:
    cli
    push byte 0
    push byte 37
    jmp irq_callback_common

; 38: IRQ6
irq6:
    cli
    push byte 0
    push byte 38
    jmp irq_callback_common

; 39: IRQ7
irq7:
    cli
    push byte 0
    push byte 39
    jmp irq_callback_common

; 40: IRQ8
irq8:
    cli
    push byte 0
    push byte 40
    jmp irq_callback_common

; 41: IRQ9
irq9:
    cli
    push byte 0
    push byte 41
    jmp irq_callback_common

; 42: IRQ10
irq10:
    cli
    push byte 0
    push byte 42
    jmp irq_callback_common

; 43: IRQ11
irq11:
    cli
    push byte 0
    push byte 43
    jmp irq_callback_common

; 44: IRQ12
irq12:
    cli
    push byte 0
    push byte 44
    jmp irq_callback_common

; 45: IRQ13
irq13:
    cli
    push byte 0
    push byte 45
    jmp irq_callback_common

; 46: IRQ14
irq14:
    cli
    push byte 0
    push byte 46
    jmp irq_callback_common

; 47: IRQ15
irq15:
    cli
    push byte 0
    push byte 47
    jmp irq_callback_common

extern irq_handler

irq_callback_common:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp

    push eax
    mov eax, irq_handler
    call eax
    pop eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
