format elf 
use32


section '.text' executable
    public load_idt
    extrn handle_interrupt
    load_idt:
        pusha

        lidt [idt_info]

        call handle_interrupt

        popa
        ret

section '.data' writeable
    msg:
        db "rawr",0
    idt_start:
        irq0:
            dw 0x0000
            dw 0x0008
            db 0x00
            db 10101110b
            dw 0x0000
    idt_end:
    idt_info:
        dw idt_end - idt_start - 1
        dd idt_start