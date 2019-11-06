.p2align 5
.globl bincopy
bincopy:

MOVDQA	0(%rsi),%xmm1			#store first 16 bytes from second argument to xmm1
MOVDQA	16(%rsi),%xmm2			#store second 16 bytes from second argument to xmm2
MOVDQA	32(%rsi),%xmm3			#store third 16 bytes from second argument to xmm3
MOVDQA	48(%rsi),%xmm4			#store forth 16 bytes from second argument to xmm4

MOVDQA	%xmm1, 0(%rdi)			#store xmm1 to first 16 bytes from first argument
MOVDQA	%xmm2, 16(%rdi)			#store xmm2 to second 16 bytes from first argument
MOVDQA	%xmm3, 32(%rdi)			#store xmm3 to third 16 bytes from first argument
MOVDQA	%xmm4, 48(%rdi)			#store xmm4 to forth 16 bytes from first argument

ret

