;
; NOOS IA32 arch_init
;

[section .text]

global arch_init
arch_init:
    push    esi
    push    ebx
    pushf
    cli
    
    ; Set gdt and segments
    lgdt    [gdtr]
    mov     ax, 0x10
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    jmp     0x08:.init_cs

.init_cs:

    ; Patch idt entries to point to dummy handler and install idt
    mov     ecx, 256
    mov     ebx, idt_start

.loop:
    mov     esi, dummy_handler
    mov     [ebx], si
    shr     esi, 16
    mov     [ebx + 6], si
    add     ebx, 8
    loop    .loop
    lidt    [idtr]

    ; Init PIC
    ;mov     al, 0x11
    ;out     0x20, al
    ;out     0xA0, al
    ;mov     al, 0x20
    ;out     0x21, al
    ;mov     al, 0x28
    ;out     0xA1, al
    ;mov     al, 4
    ;out     0x21, al
    ;mov     al, 2
    ;out     0xA1, al
    ;mov     al, 1
    ;out     0x21, al
    ;out     0xA1, al
    ;mov     al, 0xFE
    ;out     0x21, al
    ;mov     al, 0xFF
    ;out     0xA1, al

    popf
    pop     ebx
    pop     esi
    ret

; Dummy interrupt handler
dummy_handler:
    iret

[section .data]

gdtr:
    dw      (gdt_end - gdt_start) - 1
    dd      gdt_start

idtr:
    dw      (idt_end - idt_start) - 1
    dd      idt_start

; GDT
gdt_start:
    dq      0                   ; Null
    dq      0x004f9a000000ffff  ; Code
    dq      0x004f92000000ffff  ; Data
gdt_end:

; IDT
; Offsets are set to 0 and will be patched on init
idt_start: 
    times 256 dq 0x00008E0000080000 
idt_end:

