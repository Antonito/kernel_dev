[BITS 32]
section .text

global	idt_load
extern	idtr

idt_load:
lidt	[idtr]
ret
