#include "kbc_IH_asm.h"

.intel_syntax

.global _kbc_IH_asm
.global _scan_asm


.data
_scan_asm:
	.int 0


.text
_kbc_IH_asm:
	pushad

	OUT_NOT_READY:
		xor eax, eax
		in al, STAT_REG
		test al, OUT_BUF_FULL      //testing if out_buf is full
		jz OUT_NOT_READY
		test al, PAR_ERR           //testing parity error
		jnz OUT_NOT_READY
		test al, TO_ERR  		   //testing timeout error
		jnz OUT_NOT_READY
	xor eax, eax
	in al, OUT_BUF
	mov _scan_asm, eax              //placing in the global variable the scancode
	popad
	ret


