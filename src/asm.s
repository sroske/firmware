#include "p33exxxx.h"

.global _asm16X16Mult
_asm16X16Mult:

MOV W0, W4
MOV W1, W5 
MPY W4*W5, A 
MOV ACCAH, W1
MOV ACCA, W0

return
.end
	
               
