(* Code *)

fun fact(n)= if n=0 then 1
else n*fact(n-1);

(* Proof 
By PMI-1 on n
Basis: When n=0,fact(n)=0!=1
Ind. Hyp.: for k=n-1,k>=0	
	   fact(k)=k!
Ind. step:
	    fact(n)=n*fact(n-1)		By algorithm
		   =n*(n-1)!		By Ind. Hyp.
		   =(n)!		By definition of factorial
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

- timer fact(5) 1300000;
val it = {sys=TIME {usec=0},usr=TIME {usec=30000}}
  : {sys:Time.time, usr:Time.time}
- timer fact(10) 1300000;
val it = {sys=TIME {usec=0},usr=TIME {usec=60000}}
  : {sys:Time.time, usr:Time.time}
- 
*)

