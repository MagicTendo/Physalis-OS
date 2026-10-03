[BITS 32]

section .multiboot_header
header_start:
    dd 0xe85250d6 ; Multiboot2 magic number
    dd 0 ; Protected mode i386
    dd header_end - header_start
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; Checksum
    align 8

    ; Address tag (supposed to be useless with EFI?)
    dd 0 ; header_addr
    dd 0 ; load_addr
    dd 0 ; load_end_addr
    dd 0 ; bss_end_addr
    dd 0 ; entry_addr

    dw 5 ; Tag type: framebuffer
    dw 0 ; Flags
    dd 20 ; Flags size
    dd 640 ; Screen width
    dd 480 ; Screen height
    dd 16 ; Color depth
    align 8

    ; End tag
    dw 0
    dw 0
    dd 8

header_end:


section .bss ; Uninitialised data
align 16
stack_bottom:
    resb 0x4000

stack_top:


section .text ; Executable code
CODE_OFFSET equ 0x8
DATA_OFFSET equ 0x10
KERNEL_LOAD_SEGMENT equ 0x1000
KERNEL_START_ADDRESS equ 0x100000

start:
    cli ; Clear interrupts
    mov ax, 0x00 ; Load immediate value 0x00 into ax
    mov ds, ax ; Set data segment (ds) to 0x00
    mov es, ax ; Set extra segment (es) to 0x00
    mov ss, ax ; Set stack segment (ss) to 0x00
    mov sp, 0x7c00 ; Set stack pointer (sp) to 0x7c00, on top of the bootloader segment
    sti ; Enable interrupts

    ; Set video mode
    mov ah, 00h
    mov al, 03h
    int 10h


load_protected_mode:
    xor ax, ax ; Make it zero
    mov ds, ax ; Set ds at zero
    mov ss, ax ; Stack starts at 0
    mov sp, 0x9c00 ; 2000h past code start
    cli

    lgdt [gdt_descriptor]

    push ds ; Save real mode
    mov eax, cr0
    or al, 1
    mov cr0, eax
    mov bx, 0x08 ; Select descriptor 1
    mov ds, bx

    and al,0xfe ; Back to realmode
    mov cr0, eax ; By toggling bit again

    pop ds ; Get back old segment
    sti

    jmp CODE_OFFSET:protected_mode_main


gdt_start:
    dw 0x0
    dw 0x0

    ; Code segment descriptor (0x8)
    dw 0xffff ; Limit, take all the available space
    dw 0x0 ; Base
    db 0x0 ; Base
    db 10011010b ; Access byte
    db 11001111b ; Flags
    db 0x00 ; Base

    ; Data segment descriptor (0x10)
    dw 0xffff ; Limit, take all the available space
    dw 0x0 ; Base
    db 0x0 ; Base
    db 10010010b ; Access byte
    db 11001111b ; Flags
    db 0x0 ; Base

gdt_end:


gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of GDT - 1
    dd gdt_start


protected_mode_main:
    mov ax, DATA_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax
    mov ebp, 0x9c00
    mov esp, ebp

    in al, 0x92
    or al, 2
    out 0x92, al

    mov bx, KERNEL_LOAD_SEGMENT ; Load the OS on the RAM
    mov dh, 16 ; Load 16 sectors of the disk
    mov dl, 0x80 ; Load from the disk
    mov ah, 0x02 ; Read the disk
    mov al, dh ; How much sectors to read
    mov cl, 0x02 ; Read from sector 2
    mov ch, 0x00 ; At cylinder 0
    mov dh, 0x00 ; At head 0
    int 0x13 ; Interruption for the BIOS

    jc disk_read_error

    mov esi, 0x10000
    mov edi, KERNEL_START_ADDRESS
    mov ecx, 4096
    cld
    rep movsb

    jmp CODE_OFFSET:KERNEL_START_ADDRESS


disk_read_error:
    hlt


global _start
_start:
	mov esp, stack_top

	extern main
	call main ; Load the kernel!

	cli


.hang: ; Infinite loop
	hlt
	jmp .hang

.end:


times 510 - ($ - $$) db 0
dw 0xaa55 ; Boot sector signature