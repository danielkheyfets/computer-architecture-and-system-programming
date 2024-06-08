section .rodata
        format_string: db "%d", 10, 0 ;format for printf
section .text
    global assFunc
    extern printf
    extern c_checkValidity
assFunc: 
        push ebp                    ;backup ebp    
        mov ebp, esp                ;set ebp to assFunc activation frame 
        pushad                      ;backup registers 
        mov ebx, [ebp+8]            ;get first argument
        push ebx                    ;add argument to c_checkValdity arguments
        call c_checkValidity        ;call function from C
        cmp  eax, '1'               ;check if return value from C function is 1
        je is_Even                  ;jump if return '1' 
        shl ebx, 3                  ;shift left 3 times like mul 8
        jmp end_Label               ;jump to end
        is_Even:                    ;label
            shl ebx, 2              ;shift left 3 times like mul 4
        end_Label:                  ;label
            push ebx                      ;push first argument for printf
            push format_string
            call printf                 ;call printf
            add esp, 8                  ;clean up stack after call
            mov esp, ebp               ;free function acitvation frame
            pop ebp                     ;retsore base pointer previous value
            ret                         ;reutnrs from printf
