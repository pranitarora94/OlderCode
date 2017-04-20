(* Code *)

fun fib(n)=
let fun iter(x:int * int * int * int,p:int * int * int * int,n)=
let fun odd(m)=(m mod 2=1);
fun matrix (a,b,c,d) (e,f,g,h)=(a*e+b*g,a*f+b*h,c*e+d*g,c*f+d*h);
in
if n=0 then #2(p)
else if odd(n) then iter(matrix x x,matrix x p,n div 2)
else iter(matrix x x,p,n div 2)
end;
in
iter((1,1,1,0),(1,0,0,1),n)
end;

(* Proof
	fib(n)=2nd element of the 2X2 matrix (1,1,1,0)^n
*) 

(* Time estimation
	T(0)=0
	if n=2^m
	T(2^m)=T(2^(m-1))+1			// one step of matrix multiplication x*x
	      =T(1)+m
	      =m+1
	      =log(base 2)n+1
	if n=odd,there will be one more step
	therefore the +1 will become +2 and 	//2nd step of matrix multiplication p*x	
	T(n)=2*log(base 2)n+1
	
	Order=O(log(n))
*)

(* Space estimation
	As there is no deferment,
	Order=O(1)
*)

(* Timer outputs

- timer fib(4) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=50000}}
  : {sys:Time.time, usr:Time.time}
- timer fib(2) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=40000}}
  : {sys:Time.time, usr:Time.time}
- timer fib(8) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=60000}}
  : {sys:Time.time, usr:Time.time}
- timer fib(16) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=70000}}
  : {sys:Time.time, usr:Time.time}

*)

