;this file show some of the first pass errors
; this is a very long line aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

; labels errors

2label: mov r1,r2
lbl with spaces:  mov r1,r2
label&: mov r1,r2
:mov r1,r2
r1: mov r1,r2
LONGLONGLONGLONGLONGLONGLONGLONGLONGLONG: r2,r7
LABEL: 
LABEL2: mov r1,r2
LABEL2: mov r1,r7

; extern label errors
.extern LABEL2
.extern
lkkd .extern A
.extern A  b
.extern 2A
.extern a%a
.extern LONGLONGLONGLONGLONGLONGLONGLONGLONG

;errors in data/string

.data 1,2,,3
.data ,1,2,3
.data 1 2
.data 1.1
.data 1-
.data 11111111111111111111

.data .string "aaa"
aa .string "anckjdks"
.string "aaaa
.string "assfv" dsjkvds
.entry .data 1,2,3
.extern .data 1,2,3

;other errors in first pass
what r1,r3
.entry .extern X
mov r1 r2





