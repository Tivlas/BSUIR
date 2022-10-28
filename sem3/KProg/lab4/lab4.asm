 org $8000
 ldy #$8200
 ldx #fill
 stx $fff6
 ldx #$8000
 swi
 ldaa #10
 ldy #$8209

bubblesort:
 pshy
 pulx
 dex
 staa $200    
 ldab $200
 decb    
 jsr innerloop
 ldy #$8209
 deca
 bne bubblesort


 org $500
fill:
 ldaa $0,x
 staa $0,y
 inx
 iny
 decb
 bne fill
 rti

innerloop:
 staa $10
 stab $11
 ldaa 0,x
 cmpa 0,y
 bcs swap
 jmp continue

swap:
 ldaa 0,y
 ldab 0,x
 staa 0,x
 stab 0,y

continue:
 ldaa $10
 ldab $11 
 dex
 dey
 decb
 bne innerloop
 rts
 

 
 