	.file	"big_array.c"
	.comm	array,8,8
	.comm	data_log,8,8
	.comm	log_size,4,4
	.comm	loops,4,4
	.globl	elt_size
	.data
	.align 4
	.type	elt_size, @object
	.size	elt_size, 4
elt_size:
	.long	1
	.globl	cur_log
	.bss
	.align 4
	.type	cur_log, @object
	.size	cur_log, 4
cur_log:
	.zero	4
	.globl	pos
	.data
	.align 4
	.type	pos, @object
	.size	pos, 4
pos:
	.long	100
	.comm	prog,8,8
	.comm	total_cycles,8,8
	.section	.rodata
.LC0:
	.string	"not enough space!"
	.text
	.globl	build_array
	.type	build_array, @function
build_array:
.LFB499:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	log_size(%rip), %eax
	movl	%eax, pos(%rip)
	movl	$0, %edi
	call	time
	movl	%eax, %edi
	call	srand
	movl	log_size(%rip), %eax
	addl	$4096, %eax
	movslq	%eax, %rdx
	movl	elt_size(%rip), %eax
	cltq
	imulq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, array(%rip)
	movq	array(%rip), %rax
	testq	%rax, %rax
	jne	.L2
	movl	$.LC0, %edi
	call	puts
	movl	$1, %edi
	call	exit
.L2:
	movq	array(%rip), %rax
	movl	elt_size(%rip), %edx
	movslq	%edx, %rdx
	salq	$15, %rdx
	addq	%rdx, %rax
	movq	%rax, data_log(%rip)
	movl	$0, -12(%rbp)
	jmp	.L3
.L6:
	call	rand
	movl	%eax, -4(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L4
.L5:
	movq	array(%rip), %rdx
	movl	elt_size(%rip), %eax
	imull	-12(%rbp), %eax
	movl	%eax, %ecx
	movl	-8(%rbp), %eax
	addl	%ecx, %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-4(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -8(%rbp)
.L4:
	movl	elt_size(%rip), %eax
	cmpl	%eax, -8(%rbp)
	jl	.L5
	addl	$1, -12(%rbp)
.L3:
	cmpl	$8191, -12(%rbp)
	jle	.L6
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE499:
	.size	build_array, .-build_array
	.section	.rodata
.LC1:
	.string	"cc : cycles = %lu\n"
	.text
	.globl	rcn_frdly_cc
	.type	rcn_frdly_cc, @function
rcn_frdly_cc:
.LFB500:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -72(%rbp)
	movq	-72(%rbp), %rax
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -16(%rbp)
	movb	$1, -53(%rbp)
	movq	$0, -24(%rbp)
	movq	$0, -40(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L8
.L15:
	movq	-40(%rbp), %rax
	cmpq	-72(%rbp), %rax
	jne	.L9
	movb	$0, -53(%rbp)
	movl	$0, %eax
	call	rdtsc
	movq	%rax, -24(%rbp)
.L9:
	movq	-32(%rbp), %rax
	cmpq	-72(%rbp), %rax
	jne	.L10
	movq	$0, -32(%rbp)
.L10:
	cmpb	$0, -53(%rbp)
	je	.L11
	call	rand
	movl	%eax, %edx
	movl	%edx, %eax
	sarl	$31, %eax
	shrl	$19, %eax
	addl	%eax, %edx
	andl	$8191, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -52(%rbp)
	movq	-32(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-16(%rbp), %rax
	addq	%rax, %rdx
	movl	-52(%rbp), %eax
	movl	%eax, (%rdx)
	jmp	.L12
.L11:
	movq	-32(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -52(%rbp)
.L12:
	movl	$0, -48(%rbp)
	jmp	.L13
.L14:
	call	rand
	movl	%eax, -44(%rbp)
	movl	-52(%rbp), %edx
	movl	-44(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	cacheLog
#APP
# 95 "big_array.c" 1
	sfence
# 0 "" 2
#NO_APP
	movq	array(%rip), %rdx
	movl	elt_size(%rip), %eax
	imull	-52(%rbp), %eax
	movl	%eax, %ecx
	movl	-48(%rbp), %eax
	addl	%ecx, %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-44(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -48(%rbp)
.L13:
	movl	elt_size(%rip), %eax
	cmpl	%eax, -48(%rbp)
	jl	.L14
	addq	$1, -40(%rbp)
	addq	$1, -32(%rbp)
.L8:
	movl	loops(%rip), %eax
	cltq
	cmpq	-40(%rbp), %rax
	ja	.L15
	movl	$0, %eax
	call	rdtsc
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-24(%rbp), %rax
	movq	%rax, total_cycles(%rip)
	movq	total_cycles(%rip), %rdx
	movq	stderr(%rip), %rax
	movl	$.LC1, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE500:
	.size	rcn_frdly_cc, .-rcn_frdly_cc
	.section	.rodata
.LC2:
	.string	"bp : cycles = %lu\n"
	.text
	.globl	rcn_frdly_bp
	.type	rcn_frdly_bp, @function
rcn_frdly_bp:
.LFB501:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	-56(%rbp), %rax
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -16(%rbp)
	movb	$1, -45(%rbp)
	movq	$0, -24(%rbp)
	movl	$0, -40(%rbp)
	movl	$0, -36(%rbp)
	jmp	.L17
.L24:
	movl	-40(%rbp), %eax
	cltq
	cmpq	-56(%rbp), %rax
	jne	.L18
	movb	$0, -45(%rbp)
	movl	$0, %eax
	call	rdtsc
	movq	%rax, -24(%rbp)
.L18:
	movl	-36(%rbp), %eax
	cltq
	cmpq	-56(%rbp), %rax
	jne	.L19
	movl	$0, -36(%rbp)
.L19:
	cmpb	$0, -45(%rbp)
	je	.L20
	call	rand
	movl	%eax, %edx
	movl	%edx, %eax
	sarl	$31, %eax
	shrl	$19, %eax
	addl	%eax, %edx
	andl	$8191, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -44(%rbp)
	movl	-36(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-16(%rbp), %rax
	addq	%rax, %rdx
	movl	-44(%rbp), %eax
	movl	%eax, (%rdx)
	jmp	.L21
.L20:
	movl	-36(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -44(%rbp)
.L21:
	movl	$0, -32(%rbp)
	jmp	.L22
.L23:
	call	rand
	movl	%eax, -28(%rbp)
	movl	-44(%rbp), %edx
	movl	-28(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	bypassLog
#APP
# 139 "big_array.c" 1
	sfence
# 0 "" 2
#NO_APP
	movq	array(%rip), %rdx
	movl	elt_size(%rip), %eax
	imull	-44(%rbp), %eax
	movl	%eax, %ecx
	movl	-32(%rbp), %eax
	addl	%ecx, %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-28(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -32(%rbp)
.L22:
	movl	elt_size(%rip), %eax
	cmpl	%eax, -32(%rbp)
	jl	.L23
	addl	$1, -40(%rbp)
	addl	$1, -36(%rbp)
.L17:
	movl	loops(%rip), %eax
	cmpl	%eax, -40(%rbp)
	jl	.L24
	movl	$0, %eax
	call	rdtsc
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-24(%rbp), %rax
	movq	%rax, total_cycles(%rip)
	movq	total_cycles(%rip), %rdx
	movq	stderr(%rip), %rax
	movl	$.LC2, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE501:
	.size	rcn_frdly_bp, .-rcn_frdly_bp
	.globl	warmup_cc
	.type	warmup_cc, @function
warmup_cc:
.LFB502:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -72(%rbp)
	movq	-72(%rbp), %rax
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -24(%rbp)
	movb	$1, -53(%rbp)
	movq	$0, -16(%rbp)
	movq	$0, -40(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L26
.L34:
	movq	-40(%rbp), %rax
	cmpq	-72(%rbp), %rax
	jne	.L27
	movb	$0, -53(%rbp)
	movl	$0, %eax
	call	rdtsc
	movq	%rax, -16(%rbp)
	jmp	.L25
.L27:
	movq	-32(%rbp), %rax
	cmpq	-72(%rbp), %rax
	jne	.L29
	movq	$0, -32(%rbp)
.L29:
	cmpb	$0, -53(%rbp)
	je	.L30
	call	rand
	movl	%eax, %edx
	movl	%edx, %eax
	sarl	$31, %eax
	shrl	$19, %eax
	addl	%eax, %edx
	andl	$8191, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -52(%rbp)
	movq	-32(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-52(%rbp), %eax
	movl	%eax, (%rdx)
	jmp	.L31
.L30:
	movq	-32(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -52(%rbp)
.L31:
	movl	$0, -48(%rbp)
	jmp	.L32
.L33:
	call	rand
	movl	%eax, -44(%rbp)
	movl	-52(%rbp), %edx
	movl	-44(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	cacheLog
#APP
# 185 "big_array.c" 1
	sfence
# 0 "" 2
#NO_APP
	movq	array(%rip), %rdx
	movl	elt_size(%rip), %eax
	imull	-52(%rbp), %eax
	movl	%eax, %ecx
	movl	-48(%rbp), %eax
	addl	%ecx, %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-44(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -48(%rbp)
.L32:
	movl	elt_size(%rip), %eax
	cmpl	%eax, -48(%rbp)
	jl	.L33
	addq	$1, -40(%rbp)
	addq	$1, -32(%rbp)
.L26:
	movl	loops(%rip), %eax
	cltq
	cmpq	-40(%rbp), %rax
	ja	.L34
	movl	$0, %eax
	call	rdtsc
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	movq	%rax, total_cycles(%rip)
.L25:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE502:
	.size	warmup_cc, .-warmup_cc
	.globl	warmup_bp
	.type	warmup_bp, @function
warmup_bp:
.LFB503:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	-56(%rbp), %rax
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -24(%rbp)
	movb	$1, -45(%rbp)
	movq	$0, -16(%rbp)
	movl	$0, -40(%rbp)
	movl	$0, -36(%rbp)
	jmp	.L36
.L44:
	movl	-40(%rbp), %eax
	cltq
	cmpq	-56(%rbp), %rax
	jne	.L37
	movb	$0, -45(%rbp)
	movl	$0, %eax
	call	rdtsc
	movq	%rax, -16(%rbp)
	jmp	.L35
.L37:
	movl	-36(%rbp), %eax
	cltq
	cmpq	-56(%rbp), %rax
	jne	.L39
	movl	$0, -36(%rbp)
.L39:
	cmpb	$0, -45(%rbp)
	je	.L40
	call	rand
	movl	%eax, %edx
	movl	%edx, %eax
	sarl	$31, %eax
	shrl	$19, %eax
	addl	%eax, %edx
	andl	$8191, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -44(%rbp)
	movl	-36(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-44(%rbp), %eax
	movl	%eax, (%rdx)
	jmp	.L41
.L40:
	movl	-36(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -44(%rbp)
.L41:
	movl	$0, -32(%rbp)
	jmp	.L42
.L43:
	call	rand
	movl	%eax, -28(%rbp)
	movl	-44(%rbp), %edx
	movl	-28(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	bypassLog
#APP
# 229 "big_array.c" 1
	sfence
# 0 "" 2
#NO_APP
	movq	array(%rip), %rdx
	movl	elt_size(%rip), %eax
	imull	-44(%rbp), %eax
	movl	%eax, %ecx
	movl	-32(%rbp), %eax
	addl	%ecx, %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-28(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -32(%rbp)
.L42:
	movl	elt_size(%rip), %eax
	cmpl	%eax, -32(%rbp)
	jl	.L43
	addl	$1, -40(%rbp)
	addl	$1, -36(%rbp)
.L36:
	movl	loops(%rip), %eax
	cmpl	%eax, -40(%rbp)
	jl	.L44
	movl	$0, %eax
	call	rdtsc
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	movq	%rax, total_cycles(%rip)
.L35:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE503:
	.size	warmup_bp, .-warmup_bp
	.globl	streaming
	.type	streaming, @function
streaming:
.LFB504:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L46
.L48:
	cmpl	$8191, -4(%rbp)
	jle	.L47
	movl	$0, -4(%rbp)
.L47:
	addl	$1, -8(%rbp)
	addl	$1, -4(%rbp)
.L46:
	movl	loops(%rip), %eax
	cmpl	%eax, -8(%rbp)
	jl	.L48
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE504:
	.size	streaming, .-streaming
	.section	.rodata
.LC3:
	.string	"%d "
	.text
	.globl	show
	.type	show, @function
show:
.LFB505:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	$0, -8(%rbp)
	jmp	.L50
.L51:
	movq	array(%rip), %rdx
	movl	elt_size(%rip), %eax
	cltq
	imulq	-8(%rbp), %rax
	salq	$2, %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	addq	$1, -8(%rbp)
.L50:
	cmpq	$8191, -8(%rbp)
	jbe	.L51
	movl	$10, %edi
	call	putchar
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE505:
	.size	show, .-show
	.globl	bypassLog
	.type	bypassLog, @function
bypassLog:
.LFB506:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	cur_log(%rip), %edx
	movl	log_size(%rip), %eax
	cmpl	%eax, %edx
	jl	.L53
	movl	$0, cur_log(%rip)
.L53:
	movl	$0, -28(%rbp)
	jmp	.L54
.L55:
	movq	data_log(%rip), %rcx
	movl	cur_log(%rip), %eax
	leal	1(%rax), %edx
	movl	%edx, cur_log(%rip)
	cltq
	salq	$2, %rax
	addq	%rcx, %rax
	movq	%rax, -16(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, -20(%rbp)
	movq	-16(%rbp), %rax
	movl	-20(%rbp), %edx
	movnti	%edx, (%rax)
	movq	data_log(%rip), %rcx
	movl	cur_log(%rip), %eax
	leal	1(%rax), %edx
	movl	%edx, cur_log(%rip)
	cltq
	salq	$2, %rax
	addq	%rcx, %rax
	movq	%rax, -8(%rbp)
	movl	-40(%rbp), %eax
	movl	%eax, -24(%rbp)
	movq	-8(%rbp), %rax
	movl	-24(%rbp), %edx
	movnti	%edx, (%rax)
	addl	$1, -28(%rbp)
.L54:
	movl	elt_size(%rip), %eax
	cmpl	%eax, -28(%rbp)
	jl	.L55
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE506:
	.size	bypassLog, .-bypassLog
	.globl	cacheLog
	.type	cacheLog, @function
cacheLog:
.LFB507:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	movl	cur_log(%rip), %edx
	movl	log_size(%rip), %eax
	cmpl	%eax, %edx
	jl	.L57
	movl	$0, cur_log(%rip)
.L57:
	movl	$0, -4(%rbp)
	jmp	.L58
.L59:
	movq	data_log(%rip), %rcx
	movl	cur_log(%rip), %eax
	leal	1(%rax), %edx
	movl	%edx, cur_log(%rip)
	cltq
	salq	$2, %rax
	leaq	(%rcx,%rax), %rdx
	movl	-20(%rbp), %eax
	movl	%eax, (%rdx)
	movq	data_log(%rip), %rcx
	movl	cur_log(%rip), %eax
	leal	1(%rax), %edx
	movl	%edx, cur_log(%rip)
	cltq
	salq	$2, %rax
	leaq	(%rcx,%rax), %rdx
	movl	-24(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -4(%rbp)
.L58:
	movl	elt_size(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L59
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE507:
	.size	cacheLog, .-cacheLog
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.2) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
