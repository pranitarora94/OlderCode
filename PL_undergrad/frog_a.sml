(* Code *)
fun frog(n) =
if n =1 then 1 
else if n =2 then 2 
else if n =3 then 4
else frog(n-1)+frog(n-2)+frog(n-3);

(* Explaination
	As the frog can do 3 different jumps, 1 step jump, 2 step jump or a 3 step jump,
	we make 3 cases, and reduce the number of steps left to be made by 1, 2 and 3 respectively.
	However, as (1,2) and (2,1) are to be counted separately(order matters), 
	the frog can take any of the 3 types of jumps again(unless 3>=n). 
	So we make 3 cases again until the number of steps remaining, n becomes less than or equal to 3
*)

(* Time and space estimation
	Order=O(a^n)
*)

(* Timer outputs-1
- timer frog(5) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=30000}}
  : {sys:Time.time, usr:Time.time}
- timer frog(10) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=830000}}
  : {sys:Time.time, usr:Time.time}
*)
(* Timer outputs-2
- timer frog(20) 10000;
val it = {sys=TIME {usec=0},usr=TIME {usec=3740000}}
  : {sys:Time.time, usr:Time.time}
- timer frog(10) 10000;
val it = {sys=TIME {usec=0},usr=TIME {usec=20000}}
  : {sys:Time.time, usr:Time.time}
- 
*)


