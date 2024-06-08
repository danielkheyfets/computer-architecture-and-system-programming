
macros:
    %macro begin_func 0
        push ebp
        mov ebp,esp
        pushad
    %endmacro
    %macro print_counter_title 0
        pushad
        push octal_title
        call printf
        add esp,4
        popad
    %endmacro
    %macro db_print 2
        pushad
        cmp byte[debug_mode],1
        jne %%end
        push eax
        mov eax, %1
        push eax
        push %2
        call printf
        add esp, 8
        pop eax
        %%end:
        popad
    %endmacro

    %macro print_error 1
        pushad
        push %1
        push string_format
        call printf
        add esp,8
        popad
    %endmacro

    %macro log_print 2
        pushad
        mov eax, %1
        push eax
        push %2
        call printf
        add esp, 8
        popad
    %endmacro

    %macro end_func 0
        popad
        mov esp,ebp
        pop ebp
        ret
    %endmacro

    %macro cleanBuf 1
        push eax
        mov eax,0
        %%init_loop:
            cmp eax,80
            je %%con1
            mov byte[%1+eax],0
            inc eax
            jmp %%init_loop
        %%con1:
            pop eax
    %endmacro

    %macro sys_call 4
        mov eax, %1
        mov ebx, %2
        mov ecx, %3
        mov edx, %4
        int 0x80
    %endmacro

    %macro myStack_calloc 0 
        push eax
        mov eax, [MAX_SIZE]
        push 4 ;second arg
        push eax ;first arg
        call calloc
        add esp,8
        mov [myStack_ptr], eax ;points to the allocated place in memoryx
        mov [begin_of_stack], eax; save for freeing when quit
        pop eax
    %endmacro


section .rodata
    string_format: db "%s",0
    string_format_newL: db "%s",10,0
    char_format: db "%c",10,0
    print_func_format: db "%c",0
    push_to_stack_format: db "Debug[pushed to stack]: %s",10,0
    pop_from_stack_format: db "Debug[poped from stack]: %s",10,0
    pop_from_stack_format2: db "Debug[poped from stack]: %d",10,0
    read_from_user_format: db "Debug[read from user]: %s",0
    octal_title: db "Num of operations[octal]: ",0
    dec_format: db "%d",10,0
    counter_format: db "%o",0
    string_calc: db "calc: ",0
    new_line: db 10,0
    error_1: db "Error: Operand Stack Overflow", 10, 0
    error_2: db "Error: Insufficient Number of Arguments on Stack", 10, 0

section .bss
    inputBuffer: resb 80 ; max length input it max 80 bytes
    popBuf: resb 80 ; max length input it max 80 bytes
    dupBuf: resb 80 ; max length input it max 80 bytes
    myStack_ptr: resb 4 ; pointer of the stack operation to the next free space

section .data
    begin_of_stack: dd 0 ;pointer to the myStack_ptr
    counter: dd 0       ;counter of success/unsuccessful operations
    stack_size: dd 0    ;size of myStack
    debug_mode: dd 0 ; init false
    result: dd 0 ;after a function calcultes a result put it in
    MAX_SIZE: dd 5      ;default init of stack operation max size

    digits_size: dd 0 ;number of digits read from user input
    digits_size_dup: dd 0 ;number of digits read from user input

    size_link:
    struc link
        data: resb 1
        next: resd 1
    endstruc
    len: equ $-size_link ; size of struct


section .text
  align 16
  global main
  extern printf
  extern fprintf 
  extern fflush
  extern malloc 
  extern calloc 
  extern free 
  extern gets 
  extern getchar 
  extern fgets 
  extern stdout
  extern stdin
  extern stderr
main:
    begin_func
    mov eax, [ebp+8] ; argc
    cmp eax, 1 ; if argc == 1 then no arguments therefore its MAX_SIZE = 5 and no debugmode
    je calc ;
    mov ebx, [ebp+12] ;argv**
    mov ebx, [ebx+4] ; pointer to first arg
    cmp eax,2 ; if 1 arguments [number,"-d"]
    je oneArg ; handle 1 arg
    jmp twoArg ;handle 2 arg


    oneArg:
        ;1 argument so check if "-d" or number
        cmp word [ebx], "-d" ; if debug mode
        je set_debug
        jmp update_max_size        
        set_debug:
            mov byte[debug_mode],1 ; set true;
            jmp calc

    twoArg:
    ;2 argument assume the first arg is number and the second is "-d"
        mov byte[debug_mode],1 ; set true;
        cmp word [ebx], "-d"
        jne update_max_size
        mov ebx, [ebp+12] ;argv**
        mov ebx, [ebx+8] ; pointer to second arg
    update_max_size:
        push ebx
        call convert_string_to_number
        add esp,4 ;clean
        mov eax, [result]
        ;now eax has the octal input
        mov ebx, 1; exp
        mov dword [MAX_SIZE],0 ; set init sum
        loop:
            cmp eax,0 ; eax:25
            je end_loop
            mov edx,0
            mov ecx, 10
            div ecx  ; -> eax:25 / ecx:10    
            push eax ; save dev:eax:2    remainder: edx: 5
            mov eax, edx ;eax=5
            mul ebx ; 
            add eax, dword[MAX_SIZE]
            mov [MAX_SIZE], eax
            mov eax,8
            imul ebx ,eax
            pop eax
            jmp loop
        end_loop:
            jmp calc

    calc:
        myStack_calloc ;allocate memory and now myStack_ptr points to the first free place
        while_loop:
            ;clean inputBuffer
            cleanBuf inputBuffer
            push string_calc
            push string_format
            call printf ; print "calc: "
            add esp, 8 ;clean args
            push 0
            call fflush
            add esp,4
            sys_call 3, 0, inputBuffer, 80 ;read from stdin to inputBuffer with max 80 chars
            mov dword[digits_size], eax ;size of digits read -> size of string
            dec dword[digits_size] ;dont count the \n char at the end of input
            db_print inputBuffer, read_from_user_format
            ;checkOption
            cmp byte[inputBuffer], 'q'
            je quit
            cmp byte[inputBuffer], 'p'
            je popAndPrint
            cmp byte[inputBuffer], '+'
            je addNumbers
            cmp byte[inputBuffer], 'd'
            je duplicate
            cmp byte[inputBuffer], '&'
            je bitwiseAnd
            cmp byte[inputBuffer], 'n'
            je n_op
            ; add number
            call myPush
            jmp while_loop


            n_op:
                call numberOfDigits
                inc dword[counter]
                jmp while_loop    
            addNumbers:
                 push '+'
                 call two_operands
                 add esp,4
                 inc dword[counter]
                jmp while_loop
          

            bitwiseAnd:
                push '&'
                call two_operands
                add esp,4
                inc dword[counter]
                jmp while_loop

            duplicate:
                ;check if size > 0
                push eax ;save
                mov eax, dword[stack_size]
                cmp eax, 0
                je error2
                ;duplicate the last number in stack
                call updateInputAndDigit               
                call myPush
                ;mov eax, dword[myStack_ptr]
                

                end_duplicate:
                    pop eax
                    inc dword[counter]
                    jmp while_loop
                
                error2:
                    print_error error_2
                    jmp end_duplicate

            popAndPrint:
                push eax ;save
                mov eax, dword[stack_size]
                cmp eax, 0
                je print_error2
                mov eax, dword[myStack_ptr]
                sub eax,4
                push dword[eax]
                call printNumber
                add esp,4
                ;push dword[eax]
                ;call freeList
               ;add esp,4
                mov dword[eax],0
                mov dword[myStack_ptr], eax
                dec dword[stack_size]
                
                end_popAndPrint:
                    pop eax
                    inc dword[counter]
                    jmp while_loop
                print_error2:
                    print_error error_2
                    pop eax
                    jmp while_loop

            quit:
              ; decimal to octal of counter
            push '#'
            mov eax, [counter]
            cmp eax,0
            je zero
            while_loop1:
                cmp eax,0
                je next_step
                mov edx,0
                mov ebx ,8
                div ebx ; remainder[edx] dev[eax]
                push edx
                jmp while_loop1
            next_step:
                pop eax
                while_loop2:
                    cmp eax,'#'
                    je end_loop2
                    push eax
                    push counter_format
                    call printf
                    add esp,8
                    pop eax
                    jmp while_loop2
            end_loop2:
                push 10
                push print_func_format
                call printf
                add esp,8
                ;call freeMyStack
                mov esp,ebp ;check why popad makes segmentation fault
                pop ebp
                ret 

            zero:
                push 0
                push counter_format
                call printf
                add esp,8
                jmp end_loop2


numberOfDigits:
        begin_func
        cmp dword[stack_size],1 ;check is stack is not empty
        jl print_r
        call myPop
        mov eax, dword[result] ;eax has the octal value of the poped number
        push eax
        call octal_to_decimal
        mov eax, dword[result] ;now eax is in decimal rep
        add esp,4 
        ;count chars of hexa
        mov esi,0 ; counter of hexa digits
        count_loop:
            cmp eax,0
            je finish_count
            mov edx,0
            mov ebx,16
            div ebx    ;eax: div      edx:remainder
            inc esi
            jmp count_loop
        finish_count:
            ;now esi is the number of byes in hexa and we need to devide by 2 and if there is remainder then round up
            mov eax, esi
            mov edx,0
            mov ebx,2
            div ebx  ; eax: div,    edx: remainder
            cmp edx,0
            jne round_up
            jmp end_num_of_d
        end_num_of_d:
            ;convert the decimal amount to octal
            push eax
            call convert_decimal_to_octal
            mov eax, dword[result] ;octal value
            add esp,4
            ;convert to buffer string
            cleanBuf inputBuffer
            mov ecx,0
            loop_5:
                cmp eax,0
                je con5
                mov ebx,10
                mov edx,0
                div ebx
                add edx,48 ;chage to char value
                mov [popBuf+ecx],edx
                inc ecx
                jmp loop_5
            con5:
            mov dword[digits_size], ecx 
            dec ecx
            loop_6:
                cmp ecx,-1
                je con6
                mov bl, byte[popBuf+ecx]
                mov [inputBuffer+eax],ebx    
                inc eax
                dec ecx
                jmp loop_6
            con6:
                call myPush
                cleanBuf popBuf
                end_func
        round_up:
            inc eax
            jmp end_num_of_d
        print_r:
            print_error error_1
            jmp end_num_of_d




;convert_string_to_number(char*)
convert_string_to_number:
    begin_func
    mov ebx, [ebp+8] ; get first argument char*
    ;convert string into number
    mov eax, 0 
    loop_1: 
        movzx ecx, byte [ebx] ; get a character
        inc ebx ; ready for next one
        cmp ecx, '0' ; valid?
        jb end_loop_1
        cmp ecx, '9'
        ja end_loop_1
        sub ecx, '0' ; "convert" character to number
        mov edx,10
        mul edx ; multiply "result so far" by ten
        add eax, ecx ; add in current digit
        jmp loop_1 ; until done
    end_loop_1:
        mov [result], eax
        end_func


myPop:
    begin_func
    mov eax, dword[myStack_ptr]
    sub eax,4
    mov edx, eax ;save
    mov eax, dword[eax] ;link1*
    mov ecx,0 ;index
    add_loop:
        cmp eax,0
        je finish
        
        mov ebx, dword[eax+data]
        mov dword[popBuf+ecx], ebx
        mov ebx, dword[popBuf+ecx]
        inc ecx
        mov eax, dword[eax+next]
        jmp add_loop
    finish:
        db_print popBuf, pop_from_stack_format
        push popBuf
        call convert_string_to_number
        add esp,4
        cleanBuf popBuf
        ;push dword[edx]
       ; call freeList
        ;add esp,4
        mov dword[edx],0
        sub dword[myStack_ptr], 4
        dec dword[stack_size]
        end_func
    

; ;parameter: char: '+' ->plus , '&' -> bitwise AND
two_operands:
    begin_func
    cmp dword[stack_size],2
    jl print_error_size
    call myPop
    mov eax, dword[result]
    call myPop
    mov ebx, dword[result]
    cmp byte[ebp+8], '+'
    je add_nums
    ;case of bitwise &
    push eax
    call octal_to_decimal
    mov eax, dword[result]
    add esp,4
    push ebx
    call octal_to_decimal
    mov ebx, dword[result]
    add esp,4
    and eax,ebx   ;bitwise and
    push eax
    call convert_decimal_to_octal
    mov eax, dword[result]
    add esp,4
    ;contiue
    jmp translte
    add_nums:
        push eax
        call octal_to_decimal
        mov eax, dword[result]
        add esp,4
        push ebx
        call octal_to_decimal
        mov ebx, dword[result]
        add esp,4
        add eax,ebx   ;add 
        push eax
        call convert_decimal_to_octal
        mov eax, dword[result]
        add esp,4
        jmp translte
    translte: ;convert octal number to buff string for myPush func
    cleanBuf inputBuffer
    mov ecx,0
    loop_3:
        cmp eax,0
        je con
        mov ebx,10
        mov edx,0
        div ebx
        add edx,48 ;chage to char value
        mov [popBuf+ecx],edx
        inc ecx
        jmp loop_3
    con:
    mov dword[digits_size], ecx 
     dec ecx
    loop_4:
        cmp ecx,-1
        je con2
        mov bl, byte[popBuf+ecx]
        mov [inputBuffer+eax],ebx    
        inc eax
        dec ecx
        jmp loop_4
    con2:
        call myPush
        cleanBuf popBuf
    end_addNums:
        end_func

    print_error_size:
        print_error error_2
        jmp end_addNums




;param: octal_numer value
;return decimal value number -> in [result]
octal_to_decimal:
    begin_func
    mov eax, [ebp+8] ; octal number in argument value
    mov ebx, 1; exp
    mov dword [result],0 ; set init sum
    cmp eax,0
    je end_dto
    loop_oc:
        cmp eax,0 ; eax:25
        je end_loop_oc
        mov edx,0
        mov ecx, 10
        div ecx  ; -> eax:25 / ecx:10    
        push eax ; save dev:eax:2    remainder: edx: 5
        mov eax, edx ;eax=5
        mul ebx ; 
        add eax, dword[result]
        mov [result], eax
        mov eax,8
        imul ebx ,eax
        pop eax
        jmp loop_oc
    end_loop_oc:
        end_func


;param: dec_numer value
;return octal value number -> in [result]
convert_decimal_to_octal:
    begin_func
    mov eax, [ebp+8] ; decimal number in argument value 
    mov esi,0 ; counter of the amount of digits
    loop_dto:; 25
        cmp eax,0
        je next_step1
        mov edx,0 
        mov ebx ,8
        div ebx ; remainder[edx]=1 dev[eax]=3
        push edx
        inc esi
        jmp loop_dto
    next_step1:
        dec esi ;exp
        mov dword[result],0
        ;calculate the base
        mov ebx,1
        mov ecx,0; iCounter
        base_loop:
            cmp ecx,esi
            je sum_loop
            mov eax,10
            imul ebx,eax ;ebx=ebx*10
            inc ecx ;i++
            jmp base_loop
        ;ebx is the base
        sum_loop:
            cmp ebx,0
            je end_dto
            pop eax ;value to mult
            imul eax,ebx ; eax = eax*ebx
            add dword[result],eax ;result+=eax
            mov eax, ebx ; to divide by 10
            mov ecx,10
            mov edx,0
            div ecx ;now eax has ebx/10
            mov ebx, eax ; ebx = ebx/10
            jmp sum_loop
            end_dto: 
                end_func


; Procedure: myPush
; push an element to the next free place in myStack -> myStack_ptr
; If stack_size == MAX_SIZE print error
; else add a linked list representation of initBuffer, inc stack_size,
; Return: none
myPush:
    begin_func
    mov eax, [stack_size]
    cmp eax, [MAX_SIZE]
    je no_space
    mov edx,0 ;index to run in buffer
    cmp dword[digits_size],0
    je link_zero
    link_loop: 
        cmp edx, dword[digits_size]
        je end_link_loop
        mov cl, byte[inputBuffer+edx] ;the char -> cl = data
        ;append call (data,first_ptr)
        push dword [myStack_ptr] ;first_ptr
        push ecx ;data
        call append ;exmaple: '1'->'2'->'3'
        add esp,8
        inc edx
        jmp link_loop
    end_link_loop:
        mov byte[inputBuffer+edx],0 ;null for debug only
        db_print inputBuffer, push_to_stack_format
        inc dword[stack_size]
        add dword[myStack_ptr],4  ;point to the next free place in myStack buffer of struct link*
        call updateDupBufAndDigit
        end_func
    link_zero:
        push dword [myStack_ptr]
        push '0'
        call append
        add esp,8
        jmp end_link_loop
    no_space:
        print_error error_1
        end_func

updateDupBufAndDigit:
    begin_func
    cleanBuf dupBuf
    mov eax, dword[digits_size]
    mov dword[digits_size_dup],eax ; update digits size dup to be the last that pushed
    mov edx,0 ; counter
    copy_loop:
        cmp edx,eax
        je end_update
        mov ebx, dword[inputBuffer+edx]
        mov dword[dupBuf+edx], ebx
        inc edx
        jmp copy_loop
    end_update:
        end_func

updateInputAndDigit:
    begin_func
    cleanBuf inputBuffer
    mov eax, dword[digits_size_dup]
    mov dword[digits_size],eax ; update digits size to digits_size_dup
    mov edx,0 ; counter
    copy_loop1:
        cmp edx,eax
        je end_update1
        mov ebx, dword[dupBuf+edx]
        mov dword[inputBuffer+edx], ebx
        inc edx
        jmp copy_loop1
    end_update1:
        end_func

; Procedure: append
; Appends an element at the end of a linked list
; If the linked list is empty, initialize the list
; Params (in order of pushing on the stack):
; dword data - data to be added
; dword prim - first element in the list
; Return: none
; Modifies the value of prim if it is null
;void append(data,first_ptr)
append:
    begin_func
    push 5
    call malloc ;allocate memory size of struct_link
    add esp,4
    mov bl, [ebp+8] ; get data argument ->first argument
    mov [eax+data], bl ;put in struct link in data property
    mov dword[eax+next], 0 ; next is null (is the last link in the list)
    mov ebx, [ebp+12] ; get first_ptr from list ->second argument
    cmp dword[ebx], 0 ;check if list is empty
    je isFirst
    mov ebx,[ebx] ; the address of the first element in the list
    loop_next:
        cmp dword[ebx+next],0
        je found
        mov ebx,[ebx+next] ; go to next
        jmp loop_next
        found:
        mov dword[ebx+next], eax ; last.next = this new link
        jmp end_append
    isFirst:
        mov dword[ebx], eax
    end_append:
        end_func




; Procedure: pop
; Retrieves the first element in the list in eax and deletes it.
; The list acts like a queue
; Params:
; - address to the address of the first element -> [myStack_ptr + len]
; Return: [result] = the element removed - the char
; link pop(first_ptr) 
;
pop:
    begin_func
    mov ebx, [ebp+8] ; first argument
    mov eax, [ebx] ; address of the data property ->pointer to the first link
    cmp eax,0 ; list is empty?
    je end_pop
    mov edx, eax ;edx = the address of data propery -> pointer to the first link
    mov eax, [eax+data] ;eax = the data itself in eax [the char]
    mov ecx, [edx+next] ;ecx = the pointer to first.next
    mov [ebx], ecx ; first_prt = first_ptr.next
    ;free the link
    push edx
    call free
    add esp,4
    end_pop:
        mov [result], eax
        end_func


;recursive
; freeList: 
;     begin_func
;     mov eax,[ebp+8] ;get first argument ->pointer to link struct
;     cmp eax,0
;     je done
;     mov bl, byte[eax+data]
;     ;mov edx, dword[eax+next]
;     ;mov cl, byte[edx+data] 
;     b:
;     push dword[eax+next]
;     call freeList
;     push eax
;     call free
;     add esp,4
;     done:
;         end_func




; Procedure: printNumber
; Prints the elements of a linked list
; Params (in order of pushing on the stack):
; dword prim - address of the first element
; Return: none

printNumber:
    begin_func
    mov ebx, [ebp+8]   ; ebx = first_ptr (link* pointer ->myStack_ptr+5)
    mov edx,0
    print_loop:
        cmp ebx,0 ;is empty?
        je end_printNumber
        mov ecx, dword[ebx+data]
        mov [popBuf+edx],ecx ;save
        inc edx ;i++
        mov ebx, [ebx+next] ;next link
        jmp print_loop
    end_printNumber:
        db_print popBuf, pop_from_stack_format
        log_print popBuf, string_format_newL
        end_func


