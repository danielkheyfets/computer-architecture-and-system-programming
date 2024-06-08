section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s", 10, 0	; format string

section .data                    	; we define (global) initialized variables in .data section
        sum: dd 0              		; an is a local variable of size double-word, we use it to sum to decimal
		tmpSum: dd 0				; an is a local variable of size double-word, we use it to calclate sum
		index: dd 0					; an is a local variable of size double-word, we use it for index
section .bss			; we define (global) uninitialized variables in .bss section
	an: resb 12		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]
section .text
	global convertor
	extern printf

convertor:
	push ebp
	mov ebp, esp	
	pushad			

	mov ecx, dword [ebp+8]	; get function argument (pointer to string)
	mov esi, 0				; init 0
	cmp byte[ecx], 'q'		; check if string is "q" or not
	je to_end				; if is q jump to end
	count_length:		   ; count the lenght of the string 
		mov ebx, ecx 			;backup ecx
		add ebx, esi			
		cmp byte [ebx], 0xA		;if we arrived '\n' jump to_decimal
		je to_decimal
		inc esi					; esi++
		jmp count_length 

	to_decimal:			
		mov edi, 0 			; counter for string char
		dec esi				;si = length of string
		mov eax, esi		; backup
		mov ebx,1			; base^0   -> will shl,4 during loop
		start_loop:
			cmp edi, esi	;if edi == esi finish to_dicimal and the decimal is in sum
			jg to_binary
			mov dl,[ecx+eax]	;dl = char of string [ecx+eax]
			cmp dl,'9'			;if less then 9 its number else is letter
			jle is_digit
			jmp is_letter
			after_mul:
				mov dword [index], 0	;init index 0
				mov edx, [tmpSum]		;edx = tmpSum
				add [sum], edx    		;sum = sum+tmpsum
				mov dword [tmpSum], 0	
			inc edi       ; edi++
			dec eax		  ; eax --
			shl ebx, 4	  ; ebx=ebx*16
			jmp start_loop	


	is_digit:
		sub dl, 48		
		jmp mul_dl_ebx

	is_letter:
		sub dl, 55
		jmp mul_dl_ebx

	mul_dl_ebx:     ;calculate tmpsum - dl*ebx
		while_loop:
			cmp [index], dl      
			je after_mul
			add dword [tmpSum], ebx
			inc dword [index]
			jmp while_loop
		

	to_binary:        ;from dicimal(sum) to binary (push into stack)
		mov ecx, 0
		mov esi, dword [sum]
		loop:
			cmp esi, 0 
			je to_reverse
			shr esi, 1     ;esi/2
			setc dl
			movzx ebx ,dl
			push ebx
			inc ecx
			jmp loop

	to_reverse: 		;pop from stack and put in an
		dec ecx
		mov edi, 0        ;index
		loop2:
			cmp ecx, 0
			jl to_print
			pop eax
			cmp eax, 0
			je is_zero
			mov byte [an+edi], 49
			ret_jump:
			inc edi 
			dec ecx
			jmp loop2
			is_zero:
			mov byte[an+edi], 48
			jmp ret_jump
		

	to_print:
		mov byte[an+edi], 0
		mov dword [tmpSum], 0
		mov dword [sum], 0
		mov dword [index], 0
		push an			; call printf with 2 arguments -  
		push format_string	; pointer to str and pointer to format string
		call printf
		add esp, 8		; clean up stack after call
		popad	
		mov esp, ebp	
		pop ebp
		ret
	to_end:	
		mov eax, -1	
		mov esp, ebp	
		pop ebp
		ret		

