	[BITS 32]
	section .text
	global	inb, inw, inl

inb:
	mov		dx, [esp + 4]
	in		al, dx
	ret

inw:
	mov		dx, [esp + 4]
	in		ax, dx
	ret

inl:
	mov		dx, [esp + 4]
	in		eax, dx
	ret
