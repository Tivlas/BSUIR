.model small
  
.data                      
hello db "This program find the most frequent number in the array and work with digits in range [-127,127]",10,13,"Press 'A' to get answer",10,13,"You can enter 30 digits$"
error db 0ah,0dh,"Error, enter digit in range [-127, 127]$"     
message db 0ah,0dh,"arr[$"    
result db 0ah,0dh,"Result: $"

buffer db 30 dup('$')
   
 
valid db dup(1)   

countArray db 0ffh dup(0)    
centerElemOffset db 7fh
mostFreqNumAmount db 0    
freqNumOffset dw 0      
tempNum db 0
answer db 4 dup(' ')

.code

    
start:
    mov ax, @data
    mov ds, ax
    mov cx, 30
    mov al,0 
    
    lea dx,hello                                                                                                                            /
    mov ah,9
    int 21h                       
    
    for: 
    call showMessage                
    call enterString
    call addZeroCharacter     
    call shift
    call isValid    
    
    cmp valid,0
    je invalid  
    
    cmp valid,1
    push cx
    je addToArray 
    
    added: 
     
    cmp valid,2
    je getResult
    
          
    loop for 
     
    jmp getResult       
    
getResult:
    cmp cx,30
    je exit              
    call findTheMostFrequent
    call printResult
    jmp exit
     
addToArray:
    or cx,cx
    mov cl,buffer[1]  
    lea si,buffer+2
    cmp [si],'-'
    je negativeConv:
    convert:       

        mov al,tempNum;
        mov ah,10
        mul ah
        mov tempNum,al ; mul. by 10

        mov al,[si]
        add tempNum,al
        sub tempNum,030h
        inc si
    loop convert
    
    pop cx
    
    lea si,buffer+2 
    cmp [si],'-'
    je isNeg
    lea bx,countArray
    push ax 
    or ax,ax
    mov al, tempNum
    add bx,ax 
    mov al, centerElemOffset
    add bx, ax   
    pop ax
    inc [bx] 
    mov tempNum,0
    jmp added

negativeConv:
lea si,buffer+3
dec cl
jmp convert

isNeg:
lea bx,countArray     
push ax 
or ax,ax
mov al, tempNum 
sub bx,ax 
mov al,centerElemOffset
add bx,ax
pop ax
inc [bx]
mov tempNum,0
jmp added

invalid:                     
    mov [si],0         
    lea dx, error
    mov ah,9
    int 21h    
    mov valid,1
    jmp for

proc showMessage 
    
    lea dx,message
    mov ah,9      
    
    push ax
    int 21h  
    pop ax
    
    xor ax,ax
    mov al,30
    sub al,cl
    call showDigit
    
    mov ah,2
    mov dx,']'
    int 21h
    mov ah,2
    mov dx,'='
    int 21h    
       
    ret  
endp showMessage

proc enterString
    mov ah, 0Ah
    lea dx, buffer
    int 21h
    mov ah, buffer+1
    cmp ah, 0
    je enterString
    
     
    
    ret
endp enterString  
    
proc addZeroCharacter
    lea di, buffer+2
    mov al, buffer+1
    xor ah,ah    
    add di, ax
    mov [di], '$'              
    ret
addZeroCharacter endp 

proc shift
    call replaceTabsOnSpaces
    
    lea bx, buffer+2
    
    whileNotEnd:
        cmp [bx],' ' 
        jne prepareToShift    
        inc bx
        jmp whileNotEnd
    
    prepareToShift:
        lea di, buffer+2
        sub bx, di
        sub buffer+1,bl 
     
    shiftStr:   
        push [di+bx]
        pop [di]
        cmp [di],'$'  
        je endShift
        inc di
        jne shiftStr  
     
    endShift:          
    ret
endp shift    

proc replaceTabsOnSpaces
    lea di, buffer+2
                 
    while:
         cmp [di],9
         je replaceOnSpace
         continueFind: 
         inc di
         cmp [di],'$'
         jne while
         je endReplace
    
    replaceOnSpace:
        mov [di],' '
        jmp continueFind
    
    endReplace:
    ret
endp raplaceTabsOnSpaces

proc isValid
    lea di, buffer+2  
    cmp [di],'$'
    je notValid
    
    cmp [di],'A'
    je isExit 
    
    cmp [di],'-'
    jne for0
    
    inc di
    cmp [di],'0'
    jl invalid
    cmp [di],'9'
    jg invalid
                          
    for0:
        cmp [di],'$'  
        je checkOnOverflow
        
        cmp [di],' '
        je justSpacesInEnd
        
        cmp [di],'0'   
        jl  notValid
      
        cmp [di],'9'
        jg  notValid  
    
        inc di    
        jmp for0
        
    justSpacesInEnd:
        mov bx,di
        justSpacesCycle:
            inc di
            cmp [di],'$'
            je setZeroCharacter      
            cmp [di],' '
            jne invalid
            je justSpacesCycle    
    
    setZeroCharacter:
         mov [bx],'$'
         xchg di,bx
         sub bx,di
         sub buffer+1,bl       
    
    checkOnOverflow:     
    lea di, buffer+2
    mov al, buffer+1
    xor ah,ah
    cmp [di], '-' 
    je threeDigitWithSign      
    jne threeDigit  
                           
    threeDigit:
        cmp ax,4
        je firstIsZero     
    
    continueCheck:
        cmp ax,3
        jl allRight  
        cmp [di],'1'
        jg notValid    
        cmp [di+1],'2'
        jg notValid
        cmp [di+2],'7'
        jg notValid
        jmp exitV
    
    firstIsZero:
        cmp [di],'0'
        jne notValid
        inc di
        je continueCheck
    
    threeDigitWithSign:
        inc di           
        dec ax
        jmp continueCheck    
    
    allRight:
        mov valid,1
        jmp exitV
    
    notValid:
        mov valid,0
        jmp exitV
    
    isExit:
        mov valid,2
    
    exitV:
    
    ret
isValid endp
  
proc findTheMostFrequent    
    
    mov cx,255 
    lea si,countArray
    mov mostFreqNumAmount,0 
    mov freqNumOffset,si
    findLoop:
    mov al,[si]
    cmp al,mostFreqNumAmount
    ja setFreqNumOffset 
    back: 
     
    inc si  
    loop findLoop               
    ret     
findTheMostFrequent endp                        
             
             
setFreqNumOffset: 
    mov freqNumOffset,si 
    mov mostFreqNumAmount,al
    jmp back 

proc printResult 
    lea bx,countArray 
    or ax,ax
    mov al,centerElemOffset
    add bx,ax
    cmp bx,freqNumOffset  
    mov ax,freqNumOffset
    ja negAns
    jbe posAns 
    
    final: 
    mov ah,09h
    lea dx,result
    int 21h
    mov ah,02h
    lea si,answer
    mov cx,4
    printLoop:
    mov dl,[si]
    int 21h
    inc si
    loop printLoop
    ret
printResult endp    

negAns:
    sub bx,freqNumOffset 
    lea si,answer 
    mov [si],'-'
    mov tempNum, 3 
    mov ax,bx
    
    convLoop: 
    mov bx,10
    mov ah,0
    div bl
    add ah,'0' 
    or bx,bx
    mov bl,tempNum
    mov [si+bx],ah  
    dec tempNum
    cmp al,0
    je final
    loop convLoop


jmp final

posAns: 
sub FreqNumOffset,bx
mov bx,freqNumOffset
lea si,answer
    mov tempNum, 3 
    mov ax,bx
    
    convLoop2: 
    mov bx,10
    mov ah,0
    div bl
    add ah,'0' 
    or bx,bx
    mov bl,tempNum
    mov [si+bx],ah  
    dec tempNum
    cmp al,0
    je final
    loop convLoop2
jmp final
  

proc showDigit
    push cx
    push 1
    push 10   
    
    xor cx,cx
    mov cl,2
    
    showDigitCycle:
        pop bx
        div bl
    
        mov dl,al
        add dl,30h
        mov al,ah
        mov ah,2        
      
        push ax
        int 21h
        pop ax 
        xor ah,ah  
    loop showDigitCycle
    pop cx           
    ret   
showDigit endp
      
exit:              
    mov ax,4c00h
    int 21h



end start
