	.file	"testBp.cpp"
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.globl	array_size
	.data
	.align 4
	.type	array_size, @object
	.size	array_size, 4
array_size:
	.long	8192
	.globl	array
	.bss
	.align 8
	.type	array, @object
	.size	array, 8
array:
	.zero	8
	.globl	log
	.align 8
	.type	log, @object
	.size	log, 8
log:
	.zero	8
	.text
	.globl	_Z6warmupi
	.type	_Z6warmupi, @function
_Z6warmupi:
.LFB1518:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %eax
	cltq
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, array(%rip)
	movl	-20(%rbp), %eax
	addl	%eax, %eax
	cltq
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, log(%rip)
	movl	$0, -12(%rbp)
.L3:
	movl	-12(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jge	.L4
	call	rand
	movl	%eax, -8(%rbp)
	call	rand
	movl	%eax, -4(%rbp)
	call	mcsim_log_begin
	movq	log(%rip), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rax, %rdx
	movl	-8(%rbp), %eax
	movl	%eax, (%rdx)
	movq	log(%rip), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	$4, %rdx
	addq	%rax, %rdx
	movl	-4(%rbp), %eax
	movl	%eax, (%rdx)
	call	mcsim_mem_fence
	call	mcsim_log_end
	call	mcsim_mem_fence
	movq	array(%rip), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rax, %rdx
	movl	-8(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -12(%rbp)
	jmp	.L3
.L4:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1518:
	.size	_Z6warmupi, .-_Z6warmupi
	.globl	_Z3runi
	.type	_Z3runi, @function
_Z3runi:
.LFB1519:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	call	_Z5rdtscv
	movq	%rax, -24(%rbp)
	movl	$0, -36(%rbp)
.L9:
	cmpl	$4, -36(%rbp)
	jg	.L6
	movl	$0, -40(%rbp)
.L8:
	movl	-40(%rbp), %eax
	cmpl	-52(%rbp), %eax
	jge	.L7
	call	rand
	movl	%eax, -32(%rbp)
	call	rand
	movl	%eax, -28(%rbp)
	call	mcsim_log_begin
	movq	log(%rip), %rax
	movl	-40(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rax, %rdx
	movl	-32(%rbp), %eax
	movl	%eax, (%rdx)
	movq	log(%rip), %rax
	movl	-40(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	$4, %rdx
	addq	%rax, %rdx
	movl	-28(%rbp), %eax
	movl	%eax, (%rdx)
	call	mcsim_mem_fence
	call	mcsim_log_end
	call	mcsim_mem_fence
	movq	array(%rip), %rax
	movl	-40(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rax, %rdx
	movl	-32(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -40(%rbp)
	jmp	.L8
.L7:
	addl	$1, -36(%rbp)
	jmp	.L9
.L6:
	call	_Z5rdtscv
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	subq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1519:
	.size	_Z3runi, .-_Z3runi
	.globl	main
	.type	main, @function
main:
.LFB1520:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %edi
	call	time
	movl	%eax, %edi
	call	srand
	movl	array_size(%rip), %eax
	movl	%eax, %edi
	call	_Z6warmupi
	movl	array_size(%rip), %eax
	movl	%eax, %edi
	call	_Z3runi
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1520:
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1521:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L14
	cmpl	$65535, -8(%rbp)
	jne	.L14
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	call	__cxa_atexit
.L14:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1521:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I_array_size, @function
_GLOBAL__sub_I_array_size:
.LFB1522:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1522:
	.size	_GLOBAL__sub_I_array_size, .-_GLOBAL__sub_I_array_size
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_array_size
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
