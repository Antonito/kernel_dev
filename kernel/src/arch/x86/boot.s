[bits 32]

	global			start
	extern			kmain, gdt_flush

	;; Boot informations
	STACK_SIZE		equ 0x1000 * 16
	KERNEL_OFFSET		equ 0x00000000
	KERNEL_PAGE_NUMBER	equ (KERNEL_OFFSET >> 22)

section .header
align 8
mb_header_start:
		dd 0xe85250d6						;; magic number (multiboot 2)
		dd 0										;; architecture 0 (protected mode of i386)
		dd mb_header_end - mb_header_start	;; header length

		;; checksum
		dd 0x100000000 - (0xe85250d6 + (mb_header_end - mb_header_start))

		;; required end tag
		dw 0					;; type
		dw 0					;; flags
		dd 8					;; size
mb_header_end:

section	.text
start:
	cli

_kernel_start:
	;; Setup stack
	mov	esp, boot_stack_top
	mov	ebp, boot_stack_top

	;; Push magic number
	push	eax

	;; Push boot informations
	add	ebx, KERNEL_OFFSET
	push	ebx

	;; Init GDT
	call gdt_flush

	;; Start C Kernel
	mov		ecx, kmain
	call	ecx

;; Declare a stack
section	.bss
		align 0x1000
boot_stack:
		resb	STACK_SIZE
boot_stack_top:
