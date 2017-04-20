(* Code *)

fun fib(n)=if n=1 then 1
else if n=2 then 1
else fib(n-1)+fib(n-2);

(* Proof 
	By definition of Fibonacci numbers, 
	f(1)=f(2)=1 and
	f(n)=f(n-1)+f(n-2) for n>2 
*)

(* Time estimation
	Order=O(a^n)
*)

(* Space estimation
	As number of deferments=fib(n) and fib(n)  is of exp order,
	Order=O(a^n)	// more pricisely phi^n -psi^n
*)

(* Timer outputs

- timer fib(5) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=40000}}
  : {sys:Time.time, usr:Time.time}
- timer fib(10) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=500000}}
  : {sys:Time.time, usr:Time.time}
- 

*)


