	.file	"test.cpp"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
	.section	.text._Z12uart_putcharcP6__file,"ax",@progbits
.global	_Z12uart_putcharcP6__file
	.type	_Z12uart_putcharcP6__file, @function
_Z12uart_putcharcP6__file:
.LFB17:
	.cfi_startproc
	push r28
	.cfi_def_cfa_offset 3
	.cfi_offset 28, -2
/* prologue: function */
/* outgoing args size = 0 */
/* frame size = 0 */
/* stack size = 1 */
.L__stack_usage = 1
	mov r28,r24
	cpi r24,lo8(10)
	brne .L3
	ldi r24,lo8(13)
	call _Z12uart_putcharcP6__file
.L3:
	lds r24,192
	sbrs r24,5
	rjmp .L3
	sts 198,r28
	ldi r24,0
	ldi r25,0
/* epilogue start */
	pop r28
	ret
	.cfi_endproc
.LFE17:
	.size	_Z12uart_putcharcP6__file, .-_Z12uart_putcharcP6__file
	.section	.text._Z12uart_getcharP6__file,"ax",@progbits
.global	_Z12uart_getcharP6__file
	.type	_Z12uart_getcharP6__file, @function
_Z12uart_getcharP6__file:
.LFB18:
	.cfi_startproc
/* prologue: function */
/* outgoing args size = 0 */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L7:
	lds r24,192
	sbrs r24,7
	rjmp .L7
	lds r24,198
	ldi r25,0
/* epilogue start */
	ret
	.cfi_endproc
.LFE18:
	.size	_Z12uart_getcharP6__file, .-_Z12uart_getcharP6__file
	.section	.text._Z9uart_initv,"ax",@progbits
.global	_Z9uart_initv
	.type	_Z9uart_initv, @function
_Z9uart_initv:
.LFB16:
	.cfi_startproc
/* prologue: function */
/* outgoing args size = 0 */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sts 197,__zero_reg__
	ldi r24,lo8(103)
	sts 196,r24
	ldi r30,lo8(-64)
	ldi r31,0
	ld r24,Z
	andi r24,lo8(-3)
	st Z,r24
	ldi r24,lo8(6)
	sts 194,r24
	ldi r24,lo8(24)
	sts 193,r24
/* epilogue start */
	ret
	.cfi_endproc
.LFE16:
	.size	_Z9uart_initv, .-_Z9uart_initv
	.section	.text.___extension_debug_output,"ax",@progbits
.global	___extension_debug_output
	.type	___extension_debug_output, @function
___extension_debug_output:
.LFB19:
	.cfi_startproc
/* prologue: function */
/* outgoing args size = 0 */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call puts
	ldi r24,lo8(1)
	ldi r25,0
/* epilogue start */
	ret
	.cfi_endproc
.LFE19:
	.size	___extension_debug_output, .-___extension_debug_output
	.section	.rodata._Z10stack_scanPKc.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%#x "
.LC1:
	.string	""
	.section	.text._Z10stack_scanPKc,"ax",@progbits
.global	_Z10stack_scanPKc
	.type	_Z10stack_scanPKc, @function
_Z10stack_scanPKc:
.LFB23:
	.cfi_startproc
	rcall .
	rcall .
	.cfi_def_cfa_offset 6
/* prologue: function */
/* outgoing args size = 4 */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	movw r16,r24
	ldi r28,lo8(-9)
	ldi r29,lo8(8)
	mov r14,__zero_reg__
	mov r15,__zero_reg__
	ldi r24,lo8(.LC0)
	mov r12,r24
	ldi r24,hi8(.LC0)
	mov r13,r24
.L12:
	cp r28,r16
	cpc r29,r17
	brne .L14
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	call puts
	ldi r24,lo8(2)
	ldi r25,0
	call __cxa_allocate_exception
	movw r30,r24
	ldi r24,lo8(1)
	ldi r25,0
	std Z+1,r25
	st Z,r24
	ldi r20,0
	ldi r21,0
	ldi r22,lo8(_ZTIi)
	ldi r23,hi8(_ZTIi)
	movw r24,r30
	call __cxa_throw
.L14:
	ld r24,Y
	ldi r25,0
	in r30,__SP_L__
	in r31,__SP_H__
	std Z+4,r25
	std Z+3,r24
	std Z+2,r13
	std Z+1,r12
	call printf
	movw r24,r14
	adiw r24,1
	ldi r31,4
	cp r14,r31
	cpc r15,__zero_reg__
	brne .L13
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	call puts
	ldi r24,0
	ldi r25,0
.L13:
	sbiw r28,1
	movw r14,r24
	rjmp .L12
	.cfi_endproc
.LFE23:
	.size	_Z10stack_scanPKc, .-_Z10stack_scanPKc
	.section	.text._Z1gv,"ax",@progbits
.global	_Z1gv
	.type	_Z1gv, @function
_Z1gv:
.LFB24:
	.cfi_startproc
	push r28
	.cfi_def_cfa_offset 3
	.cfi_offset 28, -2
	push r29
	.cfi_def_cfa_offset 4
	.cfi_offset 29, -3
	rcall .
	.cfi_def_cfa_offset 6
	in r28,__SP_L__
	in r29,__SP_H__
	.cfi_def_cfa_register 28
/* prologue: function */
/* outgoing args size = 0 */
/* frame size = 2 */
/* stack size = 4 */
.L__stack_usage = 4
	ldi r24,lo8(12)
	ldi r25,0
	std Y+2,r25
	std Y+1,r24
	movw r24,r28
	adiw r24,1
	call external_func
	call get_SP
	call _Z10stack_scanPKc
	.cfi_endproc
.LFE24:
	.size	_Z1gv, .-_Z1gv
	.section	.rodata._Z1fv.str1.1,"aMS",@progbits,1
.LC2:
	.string	"f SP: %u\n"
	.section	.text._Z1fv,"ax",@progbits
.global	_Z1fv
	.type	_Z1fv, @function
_Z1fv:
.LFB25:
	.cfi_startproc
	rcall .
	rcall .
	.cfi_def_cfa_offset 6
/* prologue: function */
/* outgoing args size = 4 */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	call get_SP
	in r30,__SP_L__
	in r31,__SP_H__
	std Z+4,r25
	std Z+3,r24
	ldi r24,lo8(.LC2)
	ldi r25,hi8(.LC2)
	std Z+2,r25
	std Z+1,r24
	call printf
	call get_SP
	call _Z10stack_scanPKc
	.cfi_endproc
.LFE25:
	.size	_Z1fv, .-_Z1fv
	.section	.rodata.main.str1.1,"aMS",@progbits,1
.LC3:
	.string	"Beginning"
.LC4:
	.string	"SP: %u\n"
.LC5:
	.string	"Calling f"
.LC6:
	.string	"Catch"
.LC7:
	.string	"End loop"
	.section	.text.startup.main,"ax",@progbits
.global	main
	.type	main, @function
main:
.LFB26:
	.cfi_startproc
	.cfi_personality 0,__gxx_personality_v0
	.cfi_lsda 0,.LLSDA26
	rcall .
	rcall .
	.cfi_def_cfa_offset 6
/* prologue: function */
/* outgoing args size = 4 */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	ldi r24,lo8(.LC3)
	ldi r25,hi8(.LC3)
.LEHB0:
	call puts
	call get_SP
	in r30,__SP_L__
	in r31,__SP_H__
	std Z+4,r25
	std Z+3,r24
	ldi r24,lo8(.LC4)
	ldi r25,hi8(.LC4)
	std Z+2,r25
	std Z+1,r24
	call printf
.LEHE0:
.L19:
	ldi r24,lo8(.LC5)
	ldi r25,hi8(.LC5)
.LEHB1:
	call puts
	call _Z1fv
.LEHE1:
.L21:
	movw r24,r18
	call __cxa_begin_catch
	ldi r24,lo8(.LC6)
	ldi r25,hi8(.LC6)
.LEHB2:
	call puts
.LEHE2:
.LEHB3:
	call __cxa_end_catch
	ldi r24,lo8(.LC7)
	ldi r25,hi8(.LC7)
	call puts
.LEHE3:
	ldi r31,lo8(3199999)
	ldi r18,hi8(3199999)
	ldi r24,hlo8(3199999)
1:	subi r31,1
	sbci r18,0
	sbci r24,0
	brne 1b
	rjmp .
	nop
	rjmp .L19
.L22:
	mov r29,r18
	mov r28,r19
	call __cxa_end_catch
	mov r24,r29
	mov r25,r28
.LEHB4:
	call _Unwind_Resume
.LEHE4:
	.cfi_endproc
.LFE26:
.global	__gxx_personality_v0
	.section	.gcc_except_table,"aw",@progbits
	.p2align	1
.LLSDA26:
	.byte	0xff
	.byte	0
	.uleb128 .LLSDATT26-.LLSDATTD26
.LLSDATTD26:
	.byte	0x1
	.uleb128 .LLSDACSE26-.LLSDACSB26
.LLSDACSB26:
	.uleb128 .LEHB0-.LFB26
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB26
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L21-.LFB26
	.uleb128 0x1
	.uleb128 .LEHB2-.LFB26
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L22-.LFB26
	.uleb128 0
	.uleb128 .LEHB3-.LFB26
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB4-.LFB26
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSE26:
	.byte	0x1
	.byte	0
	.p2align	1
	.word	0
.LLSDATT26:
	.section	.text.startup.main
	.size	main, .-main
	.section	.text.startup._GLOBAL__sub_I__Z9uart_initv,"ax",@progbits
	.type	_GLOBAL__sub_I__Z9uart_initv, @function
_GLOBAL__sub_I__Z9uart_initv:
.LFB28:
	.cfi_startproc
/* prologue: function */
/* outgoing args size = 0 */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call _Z9uart_initv
	ldi r30,lo8(__iob)
	ldi r31,hi8(__iob)
	ldi r24,lo8(uart_output)
	ldi r25,hi8(uart_output)
	std Z+3,r25
	std Z+2,r24
	ldi r24,lo8(uart_input)
	ldi r25,hi8(uart_input)
	std Z+1,r25
	st Z,r24
/* epilogue start */
	ret
	.cfi_endproc
.LFE28:
	.size	_GLOBAL__sub_I__Z9uart_initv, .-_GLOBAL__sub_I__Z9uart_initv
	.global __do_global_ctors
	.section .ctors,"a",@progbits
	.p2align	1
	.word	gs(_GLOBAL__sub_I__Z9uart_initv)
.global	i
	.section	.bss.i,"aw",@nobits
	.type	i, @object
	.size	i, 1
i:
	.zero	1
.global	uart_input
	.section	.data.uart_input,"aw"
	.type	uart_input, @object
	.size	uart_input, 14
uart_input:
	.word	0
	.byte	0
	.byte	1
	.word	0
	.word	0
	.word	0
	.word	gs(_Z12uart_getcharP6__file)
	.word	0
.global	uart_output
	.section	.data.uart_output,"aw"
	.type	uart_output, @object
	.size	uart_output, 14
uart_output:
	.word	0
	.byte	0
	.byte	2
	.word	0
	.word	0
	.word	gs(_Z12uart_putcharcP6__file)
	.word	0
	.word	0
	.ident	"GCC: (GNU) 13.2.0"
.global __do_copy_data
.global __do_clear_bss
