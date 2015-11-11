#include "demo_cfg.h"

    .extern AppStack
	.extern Cstart
	.global	start

    /* Set stack pointer to end of AppStack and jump to Cstart:
	 */
start:
#ifdef APPSTACKSIZE
    ldr r0, =(AppStack + APPSTACKSIZE - 4)
	mov sp, r0
#endif

jump_to_c:
    bl Cstart
	bx lr
