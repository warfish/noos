[section .text]

extern arch_install_interrupt_handler

; void dbg_install_exception_handler(dbg_exception_handler_t handler);
global dbg_install_exception_handler
dbg_install_exception_handler:
    push    ebp
    mov     ebp, esp

    ; Save user handler
    mov     eax, [ebp + 8]
    mov     [g_exception_handler_ptr], eax

    ; Install exception handlers
    push    _except_1
    push    1
    call    arch_install_interrupt_handler
    add     esp, 8
    push    _except_3
    push    3
    call    arch_install_interrupt_handler
    add     esp, 8

    pop     ebp
    ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Common exception handler
;; This is what actually installed in IDT
;; 
;; TODO: switch stacks
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
common_exception_handler:
    ; Form a cpu_context structure on stack
    ; Pushes are in reverse order so that esp will point to cpu_context once we're done
    push    ebp
    lea     ebp, [esp + 4]      ; we can use ebp now, save exception frame pointer
    push    esi
    push    edi
    push    edx
    push    ecx
    push    ebx
    push    eax
    lea     eax, [ebp + 16]     ; original interrupted context ESP was exception frame ptr + 16
    push    eax                 ; save proper ESP
    mov     eax, [ebp + 4]
    push    eax                 ; EIP from exception frame
    mov     eax, [ebp + 12] 
    push    eax                 ; EFLAGS from exception frame
    mov     eax, [ebp + 8] 
    push    ax                  ; CS from exception frame
    push    ds
    push    ss
    push    es
    push    fs
    push    gs
    mov     eax, dr7
    push    eax
    mov     eax, dr6
    push    eax
    mov     eax, dr3
    push    eax
    mov     eax, dr2
    push    eax
    mov     eax, dr1
    push    eax
    mov     eax, dr0
    push    eax
    
    ; Disable all breakpoints while we are here
    xor     eax, eax
    mov     dr7, eax

    ; Call high level handler
    push    esp     ; ESP now points to cpu_context start
    push    dword [ebp] 
    mov     ebx, g_exception_handler_ptr
    call    ebx
    add     esp, 8

    ; Pop cpu_context applying debugger changes
    pop     eax
    mov     dr0, eax
    pop     eax
    mov     dr1, eax
    pop     eax
    mov     dr2, eax
    pop     eax
    mov     dr3, eax
    pop     eax
    mov     dr6, eax
    pop     eax
    mov     dr7, eax
    pop     gs
    pop     fs
    pop     es
    pop     ax          ; Skip SS change. We may possibly not be able to continue if SS:ESP is changed.
    pop     ds
    pop     eax
    mov     [ebp + 8], ax   ; CS on exception frame
    pop     eax 
    mov     [ebp + 12], eax  ; EFLAGS on exception frame
    pop     eax
    mov     [ebp + 4], eax      ; EIP on exception frame
    pop     eax             ; Skip ESP
    pop     eax
    pop     ebx
    pop     ecx
    pop     edx
    pop     edi
    pop     esi
    pop     ebp

    ; Remove exception vector arg from stack and return
    add     esp, 4
    iret
    
_except_1:
    push    1
    jmp     common_exception_handler
    
_except_3:
    push    3
    jmp     common_exception_handler

[section .data]
g_exception_handler_ptr dd 0
