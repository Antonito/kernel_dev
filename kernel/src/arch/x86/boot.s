[bits 32]

	global			start
	extern			kmain, gdt_flush

	;; Boot informations
	STACK_SIZE		equ 0x10000
	KERNEL_OFFSET		equ 0x00000000
	KERNEL_PAGE_NUMBER	equ (KERNEL_OFFSET >> 22)

	;; Multiboot macros
	MULTIBOOT_PAGE_ALIGN	equ 1 << 0
	MULTIBOOT_MEMORY_INFO	equ 1 << 1
	MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
	MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
	MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

	section	.bss
	align	0x8
	[global	boot_stack]
boot_stack:
	resb	STACK_SIZE
boot_stack_top:

	section	.text
	align	0x4
MultiBootHeader:
	;; Multiboot header
	dd	MULTIBOOT_HEADER_MAGIC
	dd	MULTIBOOT_HEADER_FLAGS
	dd	MULTIBOOT_CHECKSUM

start:
	cli

_kernel_start:

	;; Setup stack
	mov	esp, boot_stack
	mov	ebp, boot_stack

	;; Push magic number
	push	eax

	;; Push boot informations
	add	ebx, KERNEL_OFFSET
	push	ebx

	;; Init GDT
	call gdt_flush

	;; Start C Kernel
	mov	ecx, kmain
	call	ecx
