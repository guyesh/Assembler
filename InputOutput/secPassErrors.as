;this file show some of the first pass errors

;legal labels
A:mov r1,r2
B: inc r2
.extern HI

;.entry errors
aaa .entry A
.entry B abc
.entry
.entry WOW

bne C
bne A,B
dec r9
dec #3
dec &A

jsr C
rts A
lea &A
sub &A,B
sub #1,&A
stop #5
mov r1,r2,r3
mov r1,r2,
mov r1
mov r1,

prn #
prn #b
prn #11111111111111111111111111

red &HI
red &BOO

red &A  ,hjghd
