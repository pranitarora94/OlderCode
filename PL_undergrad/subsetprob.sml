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


fun filter pred [] = []
  | filter pred (x::xs) =
                if pred(x) then x::(filter pred xs)
                else (filter pred xs);



fun sum1 []=0
   |sum1 (x::xs) =x+sum1 xs;
fun sum n soln= (sum1 soln =n);


fun safe soln newq =
	(sum1(soln)>0 andalso 0>newq) orelse (0>sum1(soln) andalso newq>0);


fun search (a:int,[])=false
    search (a,x::xs)=   if(a=x) then true 
			else search(a,xs);

fun remset set soln=
	if(length(set)=length(soln)) then []
	else if(search(hd(set),soln)) then remset tl(set) soln
	else hd(set)::remset tl(set) soln;

fun next remset soln =
	map (secr op:: soln)  (filter (safe soln) (remset soln)));
   
fun subset m set = depthfirst  (next remset set) (sum m) [];

