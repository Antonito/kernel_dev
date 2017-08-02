[bits 32]
global gdtptr_phys
global gdtptr
global gdt_flush

section .text
;; Loads the GDT table
gdt_flush:
	lgdt		[gdtptr_phys]
	mov		ax, 0x10
	mov		ds, ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax
	mov		ss, ax
	jmp		0x08:flush2
flush2:
	ret

section .rodata
align 16
gdtptr_phys:
	dw gdt_end - gdt_start - 1
	dd gdt

section .rodata
align 16
gdtptr:
	dw gdt_end - gdt_start - 1
	dd gdt

section .data
align 16
gdt:
gdt_start:
	;; NULL entry
	dd 0
	dd 0

	;; Code selector
	dw 0xFFFF	     ;; limit 15:0
	dw 0x0000	     ;; base 15:0
	db 0x00		     ;; base 23:16
	db 0b10011010	 ;; P(1) DPL(00) (1) C(1) D(0) R(1) A(0)
	db 0b11001111	 ;; G(1) S(1) (0) (0) limit 19:16
	db 0x00		     ;; base 31:24

	;; Kernel data selector
	dw 0xFFFF	     ;; limit 15:0
	dw 0x0000	     ;; base 15:0
	db 0x00		     ;; base 23:16
	db 0b10010010	 ;; P(1) DPL(00) (1) C(0) E(0) W(1) A(0)
	db 0b11001111	 ;; G(1) S(1) (0) (0) limit 19:16
	db 0x00		     ;; base 31:24

	;; User data selector
	dw 0xFFFF	     ;; limit 15:0
	dw 0x0000	     ;; base 15:0
	db 0x00		     ;; base 23:16
	db 0b11000010	 ;; P(1) DPL(11) (1) C(0) E(0) W(1) A(0)
	db 0b11001111	 ;; G(1) S(1) (0) (0) limit 19:16
	db 0x00		     ;; base 31:24

gdt_end:
