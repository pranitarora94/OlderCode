(* Code *)

fun intsqrt(n)=
let fun square(x)=x*x;
in
if n=0 then 0
else if square(2*intsqrt(n div 4)+1)>n then 2*intsqrt(n div 4)
else 2*intsqrt(n div 4)+1
end;


(* General Comment: 
	The else if condition check if (2i+1)^2 is greater than n. 
	If it is, the 2i+1 cannot be the integer square-root of n.
	Therefore 2i is assigned as the integer square-root in this case.
*)

(* Time estimation
	Order=O((log(n)
*)

(* Space estimation 
	For each step there is either one or two deferments,
	Order=O(log(n))
*)
(* Timer Outputs

- timer intsqrt(10);
val it = fn : int -> {sys:Time.time, usr:Time.time}
- it 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=30000}}
  : {sys:Time.time, usr:Time.time}
- timer intsqrt(40) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=70000}}
  : {sys:Time.time, usr:Time.time}
- timer intsqrt(20) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=70000}}
  : {sys:Time.time, usr:Time.time}
- 
*)


