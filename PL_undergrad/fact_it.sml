(* code *)

fun fact(n)=
let fun iter(n,f,r)=if n=r then f
else iter(n,f*(r+1),r+1);
in iter(n,1,0)
end;

(* Proof 
INV.: (n>=c>=c0) ^ (f=f0 * ((product of)(i=c0+1 to c) i) ^ (f0 *(product of)(i=c0+1 to n) i=f*(product of)(i=c+1 to n) i) 
*)

(* Time estimation
	t(0)=0
	t(n)=t(n-1)+1
	    =n
	Order=O(n)
*) 

(* Space estimation
	As there is no deferment,	
	Order=O(1)
*)

(* timer outputs

- timer fact(10) 1500000;
val it = {sys=TIME {usec=0},usr=TIME {usec=40000}}
  : {sys:Time.time, usr:Time.time}
- timer fact(5) 1500000;
val it = {sys=TIME {usec=0},usr=TIME {usec=20000}}
  : {sys:Time.time, usr:Time.time}
-

 *)
