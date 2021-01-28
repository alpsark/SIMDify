SEARCH_DIR(.)
__DYNAMIC  =  0;

MEMORY
{
    /*ram   : ORIGIN = 0, LENGTH = 28K
    stack : ORIGIN = 28K, LENGTH = 4K */

    rom   : ORIGIN = 0, LENGTH = 16K 

    ram   : ORIGIN = 16K, LENGTH = 24K
	
    myadress_par : ORIGIN = 24K, LENGTH = 4 
    myadress_par_num : ORIGIN = 24K+4, LENGTH = 4 
    myadress_arr_str : ORIGIN = 24K+8, LENGTH = 4 
    myadress_arr_end : ORIGIN = 24K+12, LENGTH = 4 


    stack : ORIGIN = 24K+16, LENGTH = 8K-16 

    /* rom   : ORIGIN = 0, LENGTH = 100k
    ram   : ORIGIN = 100K, LENGTH = 15K
     stack : ORIGIN =115K, LENGTH = 5K */
	/*ram   : ORIGIN = 0, LENGTH = 25K
   	 stack : ORIGIN = 25K, LENGTH = 5K*/

}

/* Stack information variables */
_min_stack      = 0x0;   /* (1K - 4) minimum stack space to reserve */
_stack_len     = LENGTH(stack);
_stack_end     = ORIGIN(stack);
_stack_start   = ORIGIN(stack) + LENGTH(stack);
__global_pointer$ = 0; /* 2K */


/* We have to align each sector to word boundaries as our current s19->slm
 * conversion scripts are not able to handle non-word aligned sections. */

SECTIONS
{

    .mysegment_par : {
        . = ALIGN(4);
	KEEP(*(.mySection_par))
    } > myadress_par

    .mysegment_par_num : {
        . = ALIGN(4);
	KEEP(*(.mySection_par_num))
    } > myadress_par_num

    .mysegment_arr_str : {
        . = ALIGN(4);
	KEEP(*(.mySection_arr_str))
    } > myadress_arr_str


    .mysegment_arr_end : {
        . = ALIGN(4);
	KEEP(*(.mySection_arr_end))
    } > myadress_arr_end


    .vectors :
    {
        . = ALIGN(4);
        KEEP(*(.vectors))
    } > ram

    .text : {
        . = ALIGN(4);
        _stext = .;
        *(.text)
        _etext  =  .;
        __CTOR_LIST__ = .;
        LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
        *(.ctors)
        LONG(0)
        __CTOR_END__ = .;
        __DTOR_LIST__ = .;
        LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
        *(.dtors)
        LONG(0)
        __DTOR_END__ = .;
        *(.lit)
        *(.shdata)
        _endtext = .;
    }  > rom

    /*--------------------------------------------------------------------*/
    /* Global constructor/destructor segement                             */
    /*--------------------------------------------------------------------*/

    .preinit_array     :
    {
      PROVIDE_HIDDEN (__preinit_array_start = .);
      KEEP (*(.preinit_array))
      PROVIDE_HIDDEN (__preinit_array_end = .);
    } > ram

    .init_array     :
    {
      PROVIDE_HIDDEN (__init_array_start = .);
      KEEP (*(SORT(.init_array.*)))
      KEEP (*(.init_array ))
      PROVIDE_HIDDEN (__init_array_end = .);
    } > ram

    .fini_array     :
    {
      PROVIDE_HIDDEN (__fini_array_start = .);
      KEEP (*(SORT(.fini_array.*)))
      KEEP (*(.fini_array ))
      PROVIDE_HIDDEN (__fini_array_end = .);
    } > ram

    .rodata : {
        . = ALIGN(4);
        *(.rodata);
        *(.rodata.*)
    } > ram

    .shbss :
    {
        . = ALIGN(4);
        *(.shbss)
    } > ram

    .data : {
        . = ALIGN(4);
        sdata  =  .;
        _sdata  =  .;
        *(.data);
        *(.data.*)
        edata  =  .;
        _edata  =  .;
    } > ram

    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
        _bss_end = .;
    } > ram

    /* ensure there is enough room for stack */

    .stack (NOLOAD): {
        . = ALIGN(4);
        . = . + _min_stack ;
        . = ALIGN(4);
        stack = . ;
        _stack = . ;
    } > stack


    .stab  0 (NOLOAD) :
    {
        [ .stab ]
    }

    .stabstr  0 (NOLOAD) :
    {
        [ .stabstr ]
    }

    .bss :
    {
        . = ALIGN(4);
        _end = .;
    } > ram
}

