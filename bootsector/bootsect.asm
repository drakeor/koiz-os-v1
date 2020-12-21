; Boot sector
use16

; Set global memory offset
org 0x7c00

; Set up the stack
mov bp, 0x8000
mov sp, bp

; Print name of Bootloader
mov si, startup_message
call print_string

; Print out stack location
mov bx, bp
call print_hex_word
call print_newline

; BIOS puts the boot drive in dl. We'll store it.
mov [boot_drive], dl

; Load 5 sectors from 0x0000(ES):0x9000(BX)
mov bx, 0x9000 
mov dh, 5
mov dl, [boot_drive]
call disk_load

; Move to 32 bit protected mode
call use_32bpm

; Shouldn't get here...
jmp $


; Include 16-bit things
include 'diskload.asm'
include 'printfuncs.asm'
include 'gdt.asm'

; Include 32-bit things
include 'protmode.asm'
include 'printfuncs32.asm'

; Variables
startup_message:
    db 'KoiZ OS. Stack Start: ', 0x00
bit32_mode_message:
    db 'Entered 32-bit protected mode', 0x00

; 32 bit main func
; Main logic. For now, it's just an infinite loop
use32
main_32b:
    mov ebx, bit32_mode_message
    call print_string_pm
    jmp $

; Buffer bootsector out to 512 bytes
times 510-($-$$) db 0
dw 0xaa55 