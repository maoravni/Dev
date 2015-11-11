OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
/* Internal Memory Map*/
MEMORY
{
	rom (rx)  : ORIGIN = 0x08000000, LENGTH = 0x00100000
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00020000
	ram1 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x00010000
}

_eram = 0x20000000 + 0x00020000;

/* Section Definitions */ 
SECTIONS 
{ 
    boot_base = .;
    .text : 
    { 
        KEEP(*(.isr_vector .isr_vector.*)) 
        startup_stm32f4xx.o
        *(.text .text.* .gnu.linkonce.t.*) 	      
        *(.glue_7t) *(.glue_7)		                
        *(.rodata .rodata* .gnu.linkonce.r.*)		    	                  
    } > rom
    
    .ARM.extab : 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > rom
    
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    __exidx_end = .;
    
    . = ALIGN(4); 
    _etext = .;
    . = ALIGN(4); 
    _sidata = .; 
    		
    .data : AT (_etext) 
    { 
        _sdata = .; 
        *(.data .data.*) 
        *(.flashops)
        . = ALIGN(4); 
        _edata = . ;
    } > ram

    /* .bss section which is used for uninitialized data */ 
    .bss (NOLOAD) : 
    { 
        _sbss = . ; 
        . = ALIGN(0x200); 
		*(.ram_vtable .ram_vtable*)
        bss_start = . ; 
        *(.bss .bss.*) 
        *(COMMON) 
        . = ALIGN(4); 
        _ebss = . ; 
        bss_end = . ; 
    } > ram
    
    . = ALIGN(4); 
    _end = . ; 
} 
