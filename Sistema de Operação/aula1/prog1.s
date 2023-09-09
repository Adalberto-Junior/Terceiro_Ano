.equ print_str, 8
.data
str: .asciiz "AC2 - Aulas Praticas\n"
.text
.globl main
main:
   la $a0, str  
   li $v0, print_str
   syscall 
   li $v0, 0 
   jr $ra




