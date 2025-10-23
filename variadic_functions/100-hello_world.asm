; 100-hello_world.asm
; A 64-bit assembly program that prints "Hello, World" followed by a newline.

section .data
    msg db "Hello, World", 10     ; النص + newline
    len equ $ - msg               ; حساب الطول تلقائياً

section .text
    global main                   ; تعريف نقطة البداية للدالة main

main:
    ; write syscall
    mov rax, 1                    ; رقم system call للـ write (1)
    mov rdi, 1                    ; الملف 1 = stdout
    mov rsi, msg                  ; عنوان النص
    mov rdx, len                  ; عدد البايتات للطباعة
    syscall                       ; نفذ النداء للنظام

    ; exit syscall
    mov rax, 60                   ; رقم system call للـ exit (60)
    xor rdi, rdi                  ; كود الخروج 0
    syscall
