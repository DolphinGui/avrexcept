	.file	"unwind.s"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text

.global	_Unwind_RaiseException
	.type	_Unwind_RaiseException, @function
_Unwind_RaiseException:
  pop r23 ; read the return address
  pop r22
  call __fae_get_ptr ; returns in r18-r23
  ; r18 - data, r20 = end; r22 = lsda
  cpi r18, 0 ; if no entry found
  breq unknown_func ; std::terminate()
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
  pop __tmp_reg__
  dec r26
  cpse r26, __zero_reg__ ; if pop is zero, must be the end
  rjmp is_skip
  rjmp unwind_loop

unwind_ret:
  rjmp _Unwind_RaiseException
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
