 
 org $8000

 ldaa #%11110000
 staa $800
 ldaa #%00001111
 staa $801

 ldaa #%10100000
 ldab #%01011100
 staa $700
 stab $701

 anda $800
 lsrb
 lsrb
 lsrb
 lsrb
 aba
 staa $500

 ldaa $700
 lsla
 lsla
 lsla
 lsla
 ldab $701
 andb $801
 aba
 staa $501
 ldx $500
