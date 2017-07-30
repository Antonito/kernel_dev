[bits 32]

	;; On rend les points d'entrees visibles

	global			start
	extern			kmain, code, bss, data, end

	;; Informations relatives au boot

	STACK_SIZE		equ 0x4000
	KERNEL_OFFSET		equ 0xC0000000
	KERNEL_PAGE_NUMBER	equ (KERNEL_OFFSET >> 22)

	;; Les macros multiboot
	MULTIBOOT_PAGE_ALIGN	equ 1 << 0
	MULTIBOOT_MEMORY_INFO	equ 1 << 1
	MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
	MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
	MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

	section .data
	align 0x1000
	;; Temporary boot_page_directory, used to boot, will be unmap once the kernel is booted
_boot_page_directory:
	dd 0x00000083
	times (KERNEL_PAGE_NUMBER - 1)	dd 0 ;; Pages before kernel
	dd 0x00000083
	times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0 ;; Pages after kernel

	section	.bss
	align	0x8
	[global	boot_stack]
boot_stack_top:
	resb	STACK_SIZE               	; On alloue 16ko
boot_stack:

	section	.text
	extern vmm_paging_enable_pse
	align	4
MultiBootHeader:
	;; Le heade multiboot GRUB
	dd	MULTIBOOT_HEADER_MAGIC
	dd	MULTIBOOT_HEADER_FLAGS
	dd	MULTIBOOT_CHECKSUM
	dd	code
	dd	bss
	dd	end
	dd	start

start:
	cli
	
	;; Load Page Directory
	mov	ecx, (_boot_page_directory - KERNEL_OFFSET)
	mov	cr3, ecx 

	;; Use 4MB pages
	mov	ecx, cr4
	or	ecx, 0x00000010
	mov	cr4, ecx

	;; Activate paging
	mov	ecx, cr0
	or	ecx, 0x80000000
	mov	cr0, ecx

	;; Jump to higher half kernel
	lea ecx, [_kernel_start]
	jmp ecx

	;; Should be in higher half from now on
_kernel_start:

	;; On set la stack
	mov	esp, boot_stack
	mov	ebp, boot_stack

	;; On push le magic number
	push	eax

	;; On push les infos de boot
	add	ebx, KERNEL_OFFSET
	push	ebx

	;; On lance le kernel C
	mov	ecx, kmain
	call	ecx
	hlt
