(* Code *)

fun fib(n)=
let fun iter(n,a,b)=if n=1 then a
else iter(n-1,b,a+b);
in
iter(n,1,1)
end;


(* Proof
INV.: n>=3 ^ (a=fib(n0-n+1))  ^ (b=fib(n0-n+2))
*)

(* Time estimation
	T(1)=0	
	T(n)=T(n-1)+1
	    =n-1
	Order=O(n)
*)

(* Space estimation
	As there is no deferment,
	Order=O(1)	
*)

(* Timer outputs

- timer fib(15) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=30000}}
  : {sys:Time.time, usr:Time.time}
- timer fib(30) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=60000}}
  : {sys:Time.time, usr:Time.time}
- 

*) 

