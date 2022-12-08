    .model tiny
    .code 
    org 100h
start:
      mov ah, 9
      mov dx, offset message
      int 21h
      int 20h
message db "Hello!",10,13,"Lab5 is done!$"