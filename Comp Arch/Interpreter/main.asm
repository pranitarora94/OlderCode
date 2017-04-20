@ Before call, r1= N-1
.initMat1:
sub r2, r0 ,1					@r0= N-1, r1 = i, r2=j
cmp r1, 0
bgt .ENTERINNERLOOP1
@ comment
beq .ENTERINNERLOOP1
ret

.ENTERINNERLOOP1:
add r3, r1, r2
mod r3, r3, 17
st r3, [r10]				@ store r3=(i+j)%17 at loc. for a[i][j]
sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2 - 1
cmp r2, 0 
bgt .ENTERINNERLOOP1
beq .ENTERINNERLOOP1
sub r1, r1, 1 
b .initMat1

@ Before call, r1= N
.initMat2:
sub r2, r0, 1 					@r0= N, r1 = i, r2=j
cmp r1, 0
bgt .ENTERINNERLOOP2
beq .ENTERINNERLOOP2
ret

.ENTERINNERLOOP2:
sub r3, r1, r2 
add r3, r3, 64
mod r3, r3, 6 				@ r3 = (i-j +64)%6
mul r4, r1, r2 
mod r4, r4, 8 				@ r4 = (i+j)%8
add r3, r3, r4 
st r3, [r10] 				@ store r3 = r3+ r4

sub r10, r10, 4 
sub r2, r2, 1 			@r2=r2+1
cmp r2, 0 
bgt .ENTERINNERLOOP2
beq .ENTERINNERLOOP2
sub r1, r1, 1 
b .initMat2

@ Before call, r1= N
.matMul:
sub r2, r0, 1 				@r0= N, r1 = i, r2=j
cmp r1, 0 
bgt .INNERMatMulJ
beq .INNERMatMulJ
ret

.INNERMatMulJ:
sub r4, r0, 1 				@ r3 = SUM r2=j , r4 = K
mov r3, 0 					@ SUM=0, 
cmp r2, 0
bgt .INNERMatMulK
beq .INNERMatMulK
sub r12, r12, r13 
add r11, r11, r13 			@ CHECK THIS !!!
sub r1, r1, 1 
b .matMul

.INNERMatMulK:						@ start with K=N, till K=0
ld r5, [r12]			@ load r5 = a[i][k]
sub r12, r12, 4 		@ move 1 col. right
ld r6, [r11] 			@load r6 = b [k][j]
sub r11, r11, r13 		@ move one row down
mul r5, r5, r6 			@ r5 = a[i][k] * b[k][j]
add r3, r3, r5 			@ Sum += r5
sub r4, r4, 1 			@ dec K
cmp r4, 0 
bgt .INNERMatMulK
beq .INNERMatMulK
add r11, r11, r9 
add r12, r12, r13 
st r3, [r10]
sub r10, r10, 4 
sub r2, r2, 1 
b .INNERMatMulJ

@ Before call, r1= N
.sumMat:
sub r2, r0, 1 					@r0= N, r1 = i, r2=j
cmp r1, 0
bgt .InnerLoopSum
beq .InnerLoopSum
ret

.InnerLoopSum:
ld r3, [r11] 			@ load r3 = c[i][j]
sub r11, r11, 4 
add r4, r4, r3 			@ SUM+= c[i][j]
sub r2, r2, 1 			@r2=r2+1
cmp r2, 0 
bgt .InnerLoopSum
beq .InnerLoopSum
sub r1, r1, 1 
b .sumMat

.main:
sub r1, r0, 1 				@ r0 = N, user input.
call .initMat1 
sub r1, r0, 1 
call .initMat2 
sub r1, r0, 1 
mul r12, r0, r0 
mul r12, r12, 4
add r11, r10, r12 
add r12, r11, r12 
mul r13, r0, 4 				@ needed for row jumps
mov r9, r0 
mul r9, r9, r13 			@ r9 = 4*N * N
sub r9, r9, 4 				@ for col jumps
call .matMul 
sub r1, r0, 1 
mul r11, r0, r0 
mul r11, r11, 4 
add r11, r11, r10 
mov r4, 0 
call .sumMat		@ result sum is in r4
.print r4
