.section .text
.global _start
_start:

reset_handler:
	li x1, 0
	li x2, 0
	li x3, 0
	li x4, 0
	li x5, 0
	li x6, 0
	li x7, 0
	li x8, 0
	li x9, 0
	li x10, 0
	li x11, 0
	li x12, 0
	li x13, 0
	li x14, 0
	li x15, 0

    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

    la sp, _stack_start

	la x14, _bss_start
	la x15, _bss_end

	bge x14, x15, zero_loop_end

zero_loop:
	sw x0, 0(x14)
	addi x14, x14, 4
	ble x14, x15, zero_loop
zero_loop_end:

	call __libc_init_array
    call main
    ebreak

.global _init
.global _fini
_init:
_fini:
	ret
