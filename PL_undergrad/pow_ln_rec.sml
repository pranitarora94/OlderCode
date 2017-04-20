(* Code *)

fun pow(x,n)=
let fun odd(m)=(m mod 2=1);
fun square(x)=x*x;
in 
if n=0 then 1
else if odd(n) then x*square(pow(x,n div 2))
else square(pow(x,n div 2))
end;

(* Proof
By PMI-3 on n
Basis:when n=0,pow(x,n)=1

Ind. Hyp:For all n-1>=m>=0
	 pow(x,m)=x^m

Ind. Step: Consider n
	  Case 1:Let n be odd
	n=2k+1
	n div 2 =k
pow(x,n)=x*square(pow(x,n div 2)) 	By algorithm
	=x*sqruare(pow(x,k))
	=x*square(x^k)			By ind. hyp.
	=x*x^2k
	=x^2k+1
	=x^n
	
	Case 2:Let n be even
	n=2k
pow(x,n)=square(pow(x,n div2))		By algorithm
	=square(pow(x,k))
	=square(x^k)
	=x^2k
	=x^n

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
	As there is deferment of 1 step in case of even, and 2 in case of odd,
	Space needed lies between log(base 2)n and 2*log(base 2)n
	Order=O(log(n))
*)

(* Timer Output

- timer pow(2,4) 1500000;
val it = {sys=TIME {usec=0},usr=TIME {usec=30000}}
  : {sys:Time.time, usr:Time.time}
- timer pow(2,8) 1500000;
val it = {sys=TIME {usec=0},usr=TIME {usec=40000}}
  : {sys:Time.time, usr:Time.time}

*)


