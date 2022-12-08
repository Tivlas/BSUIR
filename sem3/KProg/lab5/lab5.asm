       .model small
       .stack 100h
       .code
start: mov ax,@DATA
       mov ds,ax
       mov dx,offset message       
       mov ah,9
       int 21h       
       int 20h                 
       .data
message db "Hello World!", 10, 13, "Lab5 is done!$"
       end start