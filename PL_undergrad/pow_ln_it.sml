(* Code *)

fun pow(x,n)=
let fun iter(xseq,n,p)=
let fun odd(m)=(m mod 2=1);
in 
if n=0 then p
else if odd(n) then iter(xseq*xseq,n div 2,p*xseq)
else iter(xseq*xseq,n div 2,p)
end;
in
iter(x,n,1)
end;


(* Proof:
INV: n0>=n>=0and p*xseq^n=p0*x^n0  where n0 is n knot
*)

(* Time estimation
	T(0)=0
	if n=2^m
	T(2^m)=T(2^(m-1))+1
	      =T(1)+m
	      =m+1
	      =log(base 2)n+1
	if n=odd,there will be one more step
	therefore the +1 will become +2 and 
	T(n)=2*log(base 2)n+1

	Order=O(log(n))
*)

(* Space estimation
	As there is no deferment,
	Order=O(1)
*)

(* Timer Output

- timer pow(3,4) 1000000 
= ;
val it = {sys=TIME {usec=0},usr=TIME {usec=20000}}
  : {sys:Time.time, usr:Time.time}
- timer pow(3,8) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=30000}}
  : {sys:Time.time, usr:Time.time}

*)


