(* Code *)
fun frog(n)=
let fun iter(n,a,b,c,d,e,f)=
if n=1 then 1 
else if n=2 then 2
else if n=3 then 4     
else if n=4 then a*iter(1,a,b,c,d,e,f)+b*iter(3,a,b,c,d,e,f)+d*iter(2,a,b,c,d,e,f)
else iter(n-1,b,c,a+b+c,e,f,d+e+f);
in
iter(n,1,1,2,1,2,3)
end;

(* General comment:
	Code is similar to that of fibonacci ,as the next number(of function calls) is sum of the previous 3 numbers.
	Also, by observation,the sequence of number of calls of f(3) and f(1) are same(1,1,2,4,7...)
	except that f(3)is 1 ahead in the sequnce,i.e., in number of calls of f(3)= s(n),
	then number of calls of f(1) is s(n-1).
	The sequence of number of calls of f(2) is different(1,2,3,6,11...).This is why i have taken 7 variables.
	It is possible to do the program with lesser variable, but then the order might be exponential.
	Here, Order=O(n)
*)

(* Timer outputs
- timer frog(10) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=60000}}
  : {sys:Time.time, usr:Time.time}
- timer frog(20) 1000000;
val it = {sys=TIME {usec=0},usr=TIME {usec=100000}}
  : {sys:Time.time, usr:Time.time}
*)
