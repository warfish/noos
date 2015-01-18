;
;   NoOS boot sector
;
;   Stack top at 0x8000
;

%define LBA 1
%define SECTORS 13
%define SEGMENT 0x1000
%define OFFSET 0x0
%define BASE 0x10000

org 0x7c00

; Bios entry
boot0:
    use16
    cli

    ; Set data and stack segments to 0
    xor     ax, ax    
    mov     ds, ax
    mov     es, ax
    mov     ss, ax

    ; Set stack top
    mov     sp, 0x8000
       
    ; Set code segment to 0
    push    0x0
    push    .rm_start
    retf

; Sanitized real mode entry
.rm_start:
    sti

    ; Clear screen and set video mode
    xor     ah, ah
    mov     al, 0x03
    int     10h

    mov     si, msg
    call    pstr

    ; Load kernel at 0x10000, sector 1
    mov     ah, 42h
    mov     si, dap
    int     13h
    jc      .disk_error

    mov     si, msg
    call    pstr

    ; Enter protected mode because we can
    cli
    lgdt    [gdtr]
    lidt    [idtr]
    mov     eax, cr0
    or      eax, 1
    mov     cr0, eax
    jmp     0x08:.pm_start

; Protected mode entry
.pm_start:
    use32
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     ss, ax 
    movzx   esp, sp     ; Sanitize ESP
    
    ; Enter kernel
    jmp     BASE    

.halt:
    jmp     $

.disk_error:
    use16
    mov     si, msg_disk_error
    call    pstr
    jmp     .halt

msg:
    db      ".", 0

msg_disk_error:
    db      "disk read error", 0

dap:
    db      10h
    db      0h
    dw      SECTORS
    dw      OFFSET
    dw      SEGMENT
    dq      LBA
 
gdtr:
    dw      (gdt_end - gdt_start) + 1
    dd      gdt_start

idtr:
    dw      0
    dd      0

gdt_start:
    dq      0                   ; Null
    dq      0x004f9a000000ffff  ; Code
    dq      0x004f92000000ffff  ; Data
gdt_end:


; Print string proc.
; Input: SI = string start
; Clobbered: AX
pstr:
    mov     ah, 0x0e
.strloop:
    lodsb
    test    al, al
    jz      .strend
    int     0x10
    jmp     .strloop
.strend:
    ret


; Boot sector signature
times 510-($-$$) db 0xcc
db 0x55
db 0xaa

