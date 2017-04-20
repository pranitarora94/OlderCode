fun depthfirst next pred x =
	let
		fun dfs [] = []
		  | dfs (a::stack) =
			if pred(a) then a::(dfs stack)
			else
				dfs(next(a)@stack)
	in
		dfs [x]	
	end;
(* general functions *)

fun secr oper x y = oper(y,x);

fun belongs(a,[]) = false
  | belongs(a,x::xs) =
	if (x = a) then true
	else belongs(a,xs);

fun filter pred [] = []
  | filter pred (x::xs) =
                if pred(x) then x::(filter pred xs)
                else (filter pred xs);

(* n queens  *)
(* soln = [qm,...,q2,q1]: pos list
	  where qi is the pos of the ith queen row
   stack = pos list list
*)

fun isfull n soln = (n = length(soln));


fun safe soln newq =
	let
		fun nodiag(i,[],newq) = true
                  | nodiag(i,q::qs,newq) =
			if (Int.abs(q-newq) = i) then false
			else nodiag(i+1,qs,newq)
	in
		not(belongs(newq,soln)) andalso nodiag(1,soln,newq)
	end;

fun upto(m,n) =
	if (m > n) then []
	else m::upto(m+1,n);

fun next n soln =
	map (secr op:: soln)  (filter (safe soln) (upto(1,n)));
   
fun queens n = depthfirst  (next n) (isfull n) [];

