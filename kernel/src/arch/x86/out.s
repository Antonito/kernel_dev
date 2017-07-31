	[BITS 32]
	section .text
	global	outb, outw, outl

outb:
	mov		al, [esp + 8]
	mov		dx, [esp + 4]
	out		dx, al
	ret

outw:
	mov		ax, [esp + 8]
	mov		dx, [esp + 4]
	out		dx, ax
	ret

outl:
	mov		eax, [esp + 8]
	mov		dx, [esp + 4]
	out		dx, eax
	ret
