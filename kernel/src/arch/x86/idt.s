[bits 32]

global idt
global idtptr

section .data
align 16
;; Interrupt Descriptor Table
idt:
idt_start:
	times 255 dq 0		; 255 empty entries
idt_end:

section .rodata
align 16
;; IDT descriptor
idtptr:
	dw idt_end - idt_start - 1
	dd idt
