; Prints the following histogram:
; *
; **
; ****
; ********
; ****************
; ********************************
; ****************************************************************

; R0: The main loop index
; R1: The number of stars per current line
; R2: Where to jump to next
; R3: Values for comparison
; R4: The * character
; R5: The \n character

Main:
    ; Initialize Values
    LDI R0,0
    LDI R4,0x2a
    ; Enter PrintHistogram subroutine
    LDI R2,PrintHistogram
    CALL R2
    ; End program
    HLT

PrintHistogram:
    ; Calculate the number of stars on current line
    LDI R1,1
    SHL R1,R0
    ; Enter PrintLine subroutine
    LDI R3,0
    LDI R2,PrintLine
    CALL R2
    ; Increment index
    INC R0
    ; Continue loop if index is less than 7
    LDI R3,7
    CMP R0,R3
    LDI R2,PrintHistogram
    JLT R2
    ; Otherwise return from subroutine
    RET

PrintLine:
    PRA R4
    INC R3
    CMP R3,R1
    JLT R2
    ; Pring newline
    LDI R5,13; CR
    PRA R5
    LDI R5,10; LF
    PRA R5
    RET
