[bits 32]
section .text
global check_sse
global check_sse2
global check_sse3
global check_ssse3
global check_sse41
global check_sse42
global check_sse4A

;; Returns 1 if SSE is present AND activated, else 0
check_sse:
mov   eax, 0x1
cpuid
test  edx, 1 << 25
jz    noSSE
mov   eax, cr0
and   ax, 0xFFFB	;; clear coprocessor emulation CR0.EM
or    ax, 0x2			;; set coprocessor monitoring  CR0.MP
mov   cr0, eax
mov   eax, cr4
or    ax, 3 << 9		;; set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
mov   cr4, eax
mov   eax, 1
ret

;; Check is SSE2 is supported
check_sse2:
mov   eax, 0x1
cpuid
test  edx, 1 << 26
jz    noSSE
mov   eax, 1
ret

;; Check is SSE3 is supported
check_sse3:
mov   eax, 0x1
cpuid
test  ecx, 1 << 0
jz    noSSE
mov   eax, 1
ret

;; Check is SSSE3 is supported
check_ssse3:
mov   eax, 0x1
cpuid
test  ecx, 1 << 9
jz    noSSE
mov   eax, 1
ret

;; Check is SSE4.1 is supported
check_sse41:
mov   eax, 0x1
cpuid
test  ecx, 1 << 19
jz    noSSE
mov   eax, 1
ret

;; Check is SSE4.2 is supported
check_sse42:
mov   eax, 0x1
cpuid
test  ecx, 1 << 20
jz    noSSE
mov   eax, 1
ret

;; Check is SSE4A is supported
check_sse4A:
mov   eax, 0x1
cpuid
test  ecx, 1 << 6
jz    noSSE
mov   eax, 1
ret

noSSE:
mov   eax, 0
ret
