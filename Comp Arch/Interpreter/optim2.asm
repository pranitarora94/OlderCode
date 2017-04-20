@ Before call, r1= N-1
.initMat1:
sub r2, r0 ,1					@r0= N-1, r1 = i, r2=j
cmp r1, 0
bgt .ENTERINNERLOOP1
@ comment
beq .ENTERINNERLOOP1
ret

.ENTERINNERLOOP1:
cmp r2, 2;				@ if r2 > 2, unroll
bgt .UNROLL1
cmp r2, 0;
beq .COMPLETEREM1
bgt .COMPLETEREM1
sub r1, r1, 1 
b .initMat1

.COMPLETEREM1:
add r3, r1, r2
mod r3, r3, 17
st r3, [r10]				@ store r3=(i+j)%17 at loc. for a[i][j]
sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2 - 1
cmp r2, 0 
bgt .COMPLETEREM1
beq .COMPLETEREM1
sub r1, r1, 1 
b .initMat1

.UNROLL1:
add r3, r1, r2
mod r3, r3, 17
st r3, [r10]				@ store r3=(i+j)%17 at loc. for a[i][j]

sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2 - 1
add r3, r1, r2
mod r3, r3, 17
st r3, [r10]				@ store r3=(i+j)%17 at loc. for a[i][j]

sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2 - 1
add r3, r1, r2
mod r3, r3, 17
st r3, [r10]				@ store r3=(i+j)%17 at loc. for a[i][j]

sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2 - 1
add r3, r1, r2
mod r3, r3, 17
st r3, [r10]				@ store r3=(i+j)%17 at loc. for a[i][j]
sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2 - 1
b .ENTERINNERLOOP1

@ Before call, r1= N
.initMat2:
sub r2, r0, 1 					@r0= N, r1 = i, r2=j
cmp r1, 0
bgt .ENTERINNERLOOP2
beq .ENTERINNERLOOP2
ret

.ENTERINNERLOOP2:
cmp r2, 2;					@ if r2 > 2, unroll
bgt .UNROLL2
cmp r2, 0;
beq .COMPLETEREM2
bgt .COMPLETEREM2
sub r1, r1, 1
b .initMat2


.COMPLETEREM2:
sub r3, r1, r2 
add r3, r3, 64
mod r3, r3, 6 				@ r3 = (i-j +64)%6
mul r4, r1, r2 
mod r4, r4, 8 				@ r4 = (i+j)%8
add r3, r3, r4 
st r3, [r10] 				@ store r3 = r3+ r4

sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2 -4
cmp r2, 0 
bgt .COMPLETEREM2
beq .COMPLETEREM2
sub r1, r1, 1 
b .initMat2


.UNROLL2:
sub r3, r1, r2 
add r3, r3, 64
mod r3, r3, 6 				@ r3 = (i-j +64)%6
mul r4, r1, r2 
mod r4, r4, 8 				@ r4 = (i+j)%8
add r3, r3, r4 
st r3, [r10] 				@ store r3 = r3+ r4
sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2-1
sub r3, r1, r2 
add r3, r3, 64
mod r3, r3, 6 				@ r3 = (i-j +64)%6
mul r4, r1, r2 
mod r4, r4, 8 				@ r4 = (i+j)%8
add r3, r3, r4 
st r3, [r10] 				@ store r3 = r3+ r4
sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2-1
sub r3, r1, r2 
add r3, r3, 64
mod r3, r3, 6 				@ r3 = (i-j +64)%6
mul r4, r1, r2 
mod r4, r4, 8 				@ r4 = (i+j)%8
add r3, r3, r4 
st r3, [r10] 				@ store r3 = r3+ r4
sub r10, r10, 4 
sub r2, r2, 1 				@r2=r2-1
sub r3, r1, r2 
add r3, r3, 64
mod r3, r3, 6 				@ r3 = (i-j +64)%6
mul r4, r1, r2 
mod r4, r4, 8 				@ r4 = (i+j)%8
add r3, r3, r4 
st r3, [r10] 				@ store r3 = r3+ r4
sub r10, r10, 4 

sub r2, r2, 1 				@r2=r2-1
b .ENTERINNERLOOP2

/*
void matMul(){
	int i,j,k,sum;
	for (i=0; i<N; i++){
		for(j=0; j< N; j++){
			sum = 0    CHANGE;
			for(k=0; k< N; k++){
				sum += a[i][k] * b[k][j];
			}
			c[i][j] = sum;
		}
	}
}
*/


/*
void bijk(array A, array B, array C, int n, int bsize)
2 {
3 	int i, j, k, kk, jj;
4 	double sum;
5 	int en = bsize * (n/bsize); /* Amount that fits evenly into blocks */
6
7 	for (i = 0; i < n; i++)
8 		for (j = 0; j < n; j++)
9 			C[i][j] = 0.0;
10
11	for (kk = 0; kk < en; kk += bsize) {
12		for (jj = 0; jj < en; jj += bsize) {
13 			for (i = 0; i < n; i++) {
14 				for (j = jj; j < jj + bsize; j++) {
15 					sum = C[i][j];
16 					for (k = kk; k < kk + bsize; k++) {
17 						sum += A[i][k]*B[k][j];
18		 			}
19 					C[i][j] = sum;
20 				}
21 			}
22 		}
23 	}
24 }
*/

@ Before call, r1= N
.matMul:
mov r14, r10
b .SETcZeroOuter
mov r10, r14
sub r7, r0, 4																		@How much 

b .Blocking
ret

.SETcZeroOUTER:
sub r2, r0, 1 				@r0= N, r1 = i, r2=j
mov r3, 0					@ r3 = initValForC
cmp r1, 0 
bgt .SETcZeroI
beq .SETcZeroI

ret

.SETcZeroI:
cmp r2, 0
bgt .SETcZeroJ
beq .SETcZeroJ
sub r1, r1, 1
b .SETcZeroOuter

.SETcZeroJ
st r3, [r10]
sub r10, r10, 4 
sub r2, r2, 1 
.SETcZeroI


@before call r7=0
.blocking
@r7 = jj, r8 = kk, r1 =i, r2 =j, r3 = Sum, r4 =K
sub r8, r0, 4
cmp r7, 0;
bgt .OUTERMatMulKK
beq .OUTERMatMulKK
ret

.OUTEREMatMulJJ
add r1, r8, 3
cmp r8, 0;
bgt .INNERMatMulJ
beq .INNERMAtMulK
sub r7, r7, 4
b .blocking

.OUTEREMatMulJJ
add r2, r7, 3 
cmp r1, r8;
bgt .INNERMatMulJ
beq .INNERMAtMulK
sub r8, r8, 4
b .OUTERMatMulKK


.INNERMatMulJ:
sub r4, r0, 1 				@ r3 = SUM r2=j , r4 = K
ld r3, [r10]  				@ SUM=C[i][j], 
cmp r2, r7
bgt .INNERMatMulK
beq .INNERMatMulK
sub r12, r12, r13 
add r11, r11, r13 			@ CHECK THIS !!!
sub r1, r1, 1 
b .OUTERMatMulKK

.INNERMatMulK:						@ start with K=N, till K=0
ld r5, [r12]			@ load r5 = a[i][k]
sub r12, r12, 4 		@ move 1 col. right
ld r6, [r11] 			@ load r6 = b [k][j]
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
sub r2, r0, 1 				@r0= N, r1 = i, r2=j
cmp r1, 0
bgt .InnerLoopSum
beq .InnerLoopSum
ret



.InnerLoopSum:
cmp r2, 2;				@ if r2 > 2, unroll
bgt .UNROLL4
cmp r2, 0;
beq .COMPLETEREM4
bgt .COMPLETEREM4
sub r1, r1, 1 
b .sumMat


.COMPLETEREM4:
ld r3, [r11] 				@ load r3 = c[i][j]
sub r11, r11, 4 
add r4, r4, r3 				@ SUM+= c[i][j]
sub r2, r2, 1 				@r2=r2+1
cmp r2, 0 
bgt .COMPLETEREM4
beq .COMPLETEREM4
sub r1, r1, 1 
b .sumMat

.UNROLL4:
ld r3, [r11] 				@ load r3 = c[i][j]
sub r11, r11, 4 
add r4, r4, r3 				@ SUM+= c[i][j]
sub r2, r2, 1 				@r2=r2+1
ld r3, [r11] 				@ load r3 = c[i][j]
sub r11, r11, 4 
add r4, r4, r3 				@ SUM+= c[i][j]
sub r2, r2, 1 				@r2=r2+1
ld r3, [r11] 				@ load r3 = c[i][j]
sub r11, r11, 4 
add r4, r4, r3 				@ SUM+= c[i][j]
sub r2, r2, 1 				@r2=r2+1
ld r3, [r11] 				@ load r3 = c[i][j]
sub r11, r11, 4 
add r4, r4, r3 				@ SUM+= c[i][j]
sub r2, r2, 1 				@r2=r2+1
b .InnerLoopSum

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
