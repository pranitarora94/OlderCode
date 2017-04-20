(* Code *)

fun pow(x,n)=
let fun iter(x,n,p)=if n=0 then p
else iter(x,n-1,p*x);
in 
iter(x,n,1)
end;

(* Proof:
INV: n0>=n>=0and p*x^n=p0*x^n0  where n0 is n knot
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

(* Timer outputs

- timer pow(2,10) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=30000}}
  : {sys:Time.time, usr:Time.time}
- timer pow(2,20) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=60000}}
  : {sys:Time.time, usr:Time.time}
- 

*)

