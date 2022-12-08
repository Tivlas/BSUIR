.data 
     
    space db ' '
    enter  db 0dh,0ah,'$' 
    InputString db 'Enter the string: ','$'  
    
    InputWord db 'Enter the word to delete: ', '$'    
    Result db 'Result: ', '$'
    cmpSymbol db 'ø'
    errorStr db 'error (the string is empty/contains only spaces). Try again!',0dh,0ah,'$'   
    bufferForFinish db 40  
    bufferForString db 40 
    len db 0
    str db 200 dup('ø')
    bufferForWord db 40  
    len1 db 0
    str1 db 200 dup('$')
    
       
ends

.stack
    dw   256  dup(0)
ends

.code      

print macro strToPrint
    mov ah,09h
    lea dx, strToPrint
    int 21h
endm

readStr macro buffer
    mov ah,0Ah   
    lea dx, buffer     
    int 21h  
endm    

       
isVoid proc
    xor cx, cx
    mov cl, bufferForString+1
    xor bx, bx
    cmp cl, 0h
    je error
    
    mov si, offset bufferForString+2
    isVoidLoop:
        mov al, [si]
        cmp al, 20h
        je PlusSpace
        jmp skip
        
        PlusSpace:
            inc bx
        
        skip:
            inc si    
    loop isVoidLoop
    
    cmp bl, bufferForString+1
    je error
    jmp return
    
    error:
    mov dx, offset errorStr
    mov ah, 9h
    int 21h 
    jmp reenter    
    
    return:
        ret
    
isVoid endp
          
start:
    
    mov ax, data
    mov ds, ax
    mov es, ax
    reenter:                          
    print InputString  
    readStr bufferForString 
    call isVoid     
    print enter      
    print InputWord
    readStr bufferForWord
    print enter      
             
    mov cx,0
    mov cl,bufferForString[1]
     
                       
    ;if word is empty  go end                   
    mov dx,0
    mov dl,bufferForWord[1]
    or dx,dx
    jz end          ; if 0
     
    lea si,str[0] 
    mov di,si    
    
    
     
for:             ;looking for symbol
    lodsb      
    cmp al,space    ;compare to symbol  si++(=pointre1++)
    jz newWord     ;if != " " go to newWord
    loop for       ;else go to for
    inc si          ;when i want to delete last word

newWord:  
    pushf
    cld
    mov ax,si      ; ax = si count length of word in string
    sub ax,di      ; ax = ax - di
    dec ax         ; ax - 1
    cmp ax,dx      ; if ax == dx
     
    jne next       ; if(wordInString.Length != wordForDelete.Length)      if !=
     
    ;comparer
    push    si     ; add in stack
    push    di
    push    cx
    push    es
    push    ds
    pop     es     
    mov     cx, dx    
    lea     si, str1[0]  ;comparer if (word in string - bufferForWord = 0) word in string=buferForWord
    repe    cmpsb    ; cmp while symbols = (sym1 = sym2)
    pop     es
    pop     cx
    pop     di
    pop     si 
    
    jne     next             ; if !=     
    jcxz    skipCopy         ; if cx = 0
    
    ;remove 
    push    cx
    push    si
    push    di
    push    es
    push    ds
    pop     es 
    inc     cx    
    rep movsb      ; repeat while cx != 0
    pop     es
    pop     di
    pop     si
    pop     cx
    mov     si, di 
     
    
    loop for 
     
skipCopy:   
    inc cx
    rep     movsb

next:
    popf
    mov     di, si         ; di = si
    jcxz    end            ; if(cx==0) go to end
    loop    for            ; else go to for

end: 

    print Result     
           
     xor cx, cx
     mov si, offset str
     mov cl, 40 
     mov ah, 02h
     
     printLoop:
        mov dl, [si]
        mov bl,cmpSymbol
        mov al,[si]
        cmp al,bl
        je final
        int 21h
        inc si
        loop printLoop
         
    
    final:
    mov ax, 4c00h 
    int 21h      
    int 20h
ends

end start 