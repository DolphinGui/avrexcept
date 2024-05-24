	.file	"unwind.s"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
.data
except_ptr:
  .zero 2
return_values:
  .zero 7
.text


.global	_Unwind_Resume
	.type	_Unwind_Resume, @function
.global _Unwind_Resume_or_Rethrow ; resume_or_rethrow can be called from a forced_unwind,
; I am not currently planning to implement forced unwinding
	.type	_Unwind_Resume_or_Rethrow, @function

.global	_Unwind_RaiseException
	.type	_Unwind_RaiseException, @function
_Unwind_RaiseException:
  sts except_ptr+1, r25
  sts except_ptr, r24
tailcall:
  pop r23 ; read the return address
  pop r22
  lds r25, except_ptr+1
  lds r24, except_ptr
  call __fae_get_ptr ; returns in r18-r23
  ; r18 - data, r20 = end; r22 = landing_pad, r24 = type_index
  cpi r18, 0 ; if no entry found
  breq unknown_func ; std::terminate()
  
  cpi r24, 0xff ; if no landing pad
  breq no_landing_pad
  sts return_values+1, r19
  sts return_values,   r18
  sts return_values+3, r21
  sts return_values+2, r20
  in __tmp_reg__, __SP_H__
  sts return_values+5, __tmp_reg__
  in __tmp_reg__, __SP_L__
  sts return_values+4, __tmp_reg__
  movw Z, r22 ; move landing_pad to Z
  mov r22, r24 ; move type_index
  lds r25, except_ptr+1
  lds r24, except_ptr
  ijmp

_Unwind_Resume:
_Unwind_Resume_or_Rethrow:
  in __tmp_reg__, __SREG__
  cli
  lds r19, return_values+5
  out __SP_H__, r19
  lds r19, return_values+4
  out __SREG__, __tmp_reg__
  out __SP_L__, r19
  lds r19, return_values+1
  lds r18, return_values
  lds r21, return_values+3
  lds r20, return_values+2
no_landing_pad:
  cpc r18, r20 ; if data = data_end, no unwind besides return
  cpc r19, r21
  breq unwind_ret
  movw Z, r18

unwind_loop:
  cp r30, r20
  cpc r31, r21
  breq unwind_ret ; if Z == end, end loop
  lpm r26, Z+
  sbrc r26, 7 ; if high bit is clear, it must be a skip
  rjmp is_pop ; else it is pop
  cp r26, __zero_reg__ ; if skip is zero, exit loop
  breq unwind_ret
is_skip:
  in r24, __SP_L__
  in r25, __SP_H__
  add r24, r26
  adc r25, __zero_reg__
  in __tmp_reg__, __SREG__
  cli
  out __SP_L__, r24
  out __SREG__, __tmp_reg__
  out __SP_H__, r25
  rjmp unwind_loop
unwind_ret:
  rjmp tailcall
unknown_func:
  call __fae_terminate ; std::terminate

is_pop:
  cbr r26, 0x80 ; clears high bit
  movw r18, Z
  mov r30, r26
  lsl r30 ; size of each jump is two instructions
  subi r30, lo8(-(pm(reg2)))
  ldi r31, hi8(pm(reg2))
  ijmp
reg2:
  pop r2
  rjmp end_pop
reg3:
  pop r3
  rjmp end_pop
reg4:
  pop r4
  rjmp end_pop
reg5:
  pop r5
  rjmp end_pop
reg6:
  pop r6
  rjmp end_pop
reg7:
  pop r7
  rjmp end_pop
reg8:
  pop r8
  rjmp end_pop
reg9:
  pop r9
  rjmp end_pop
reg10:
  pop r10
  rjmp end_pop
reg11:
  pop r11
  rjmp end_pop
reg12:
  pop r12
  rjmp end_pop
reg13:
  pop r13
  rjmp end_pop
reg14:
  pop r14
  rjmp end_pop
reg15:
  pop r15
  rjmp end_pop
reg16:
  pop r16
  rjmp end_pop
reg17:
  pop r17
  rjmp end_pop
reg28:
  pop r28
  rjmp end_pop
reg29:
  pop r29
  rjmp end_pop
end_pop:
  movw Z, r18
  rjmp unwind_loop

.global	__fae_get_pc
	.type	__fae_get_pc, @function
__fae_get_pc:
  pop r25 ; read the return address
  pop r24
  push r24
  push r25
  ret
