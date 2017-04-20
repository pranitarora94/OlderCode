(* Code *)

fun perfect(n)=
let fun add_factors (n) =
let fun f(i)=if n mod i = 0 then i
else 0;
fun sum(a,b)=if a > b then 0
else f(b)+sum(a,b-1);
in
sum(1,n div 2) 
end;
in n=add_factors(n)
end; 

(* Time estimation
	Time required=n div 2	
	Order=O(n)
*)

(* Space estimation
	Order=O(n)
*)

(* Timer outputs

- timer perfect(10);
val it = fn : int -> {sys:Time.time, usr:Time.time}
- timer perfect(10) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=50000}}
  : {sys:Time.time, usr:Time.time}
- timer perfect(20) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=110000}}
  : {sys:Time.time, usr:Time.time}
- timer perfect(5) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=30000}}
  : {sys:Time.time, usr:Time.time}
- 

*)
