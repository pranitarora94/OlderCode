(* Code *)

fun pow(x,n)=if n=0 then 1
else x*pow(x,n-1);

(* Proof
By PMI-1 on n
Basis:when n=0,pow(x,n)=1

Ind. Hyp:For k>=0
	 pow(x,k)=x^k)

Ind. Step:Consider k+1
	pow(x,k+1)=x*pow(x,k)		By algorithm
		  =x*x^k		By Ind. Hyp.
		  =x^k+1	
*)

(* Time estimation
	T(0)=0
	T(n)=T(n-1)+1
	    =n
	Order=O(n)
*)

(* Space estimation
	As each function call defers one number (n),	
	Order=O(n)
*)

(* Timer outputs

- timer pow(2,9) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=50000}}
  : {sys:Time.time, usr:Time.time}
- timer pow(2,18) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=100000}}
  : {sys:Time.time, usr:Time.time}
- 

*)
