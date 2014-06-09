BITS 16
start:
mov ax, 07C0h ; Set up 4K stack space after this bootloader
add ax, 288 ; (4096 + 512) / 16 bytes per paragraph
mov ss, ax
mov sp, 4096
mov ax, 07C0h ; Set data segment to where we're loaded
mov ds, ax

mov si, boot_message ; Put message string position into SI
call print_string ; Call printing routine

mov si, linefeed
call print_string ; Call printing routine

mov si, cs_message ; Put message string position into SI
call print_string ; Call printing routine

mov si, linefeed
call print_string ; Call printing routine

mov si, copyright ; Put message string position into SI
call print_string ; Call printing routine

mov si, linefeed
call print_string ; Call printing routine

jmp $ ; Jump here - infinite loop!

boot_message db 'CS-DOS Operating System v1.0', 0
cs_message db 'Created for CS3423, Summer 2014', 0
copyright db '(c) Copyright 2014 by Matthew Tinney and Shade Alabsa. All rights reserved.', 0

linefeed db 13,10, 0

print_string: ; Routine: output string in SI to screen
mov ah, 0Eh ; 'print char' function is denoted by 0Eh
.repeat:
lodsb ; load next byte from SI and increment SI.
cmp al, 0 ; compare to zero to see if end of string.
je .done ; if char is zero, we’re at end of string.
int 10h ; Otherwise, print it.
jmp .repeat
.done:
ret
times 510-($-$$) db 0 ; fill remainder of boot sector with 0s up
; to byte 510.
dw 0xAA55 ; The standard PC boot signature of AA 55
; is placed in bytes 511 and 512.
