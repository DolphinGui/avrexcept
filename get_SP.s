	.file	"get_SP.s"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	get_SP
	.type	get_SP, @function
get_SP:
	in r24,__SP_L__
	in r25,__SP_H__
  ret
