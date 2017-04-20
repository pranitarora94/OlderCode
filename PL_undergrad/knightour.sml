fun depthfirst next pred x =
	let
		fun dfs [] = []
		  | dfs (a::stack) =
			if pred(a) then a::(dfs stack)
			else
				dfs(next(a)@stack)
	in
		dfs x
	end;
(* general functions *)

fun secr oper x y = oper(y,x);

fun belongs(a,[]) = false
  | belongs(a,x::xs) =
	if (x=a) then true
	else belongs(a,xs);

fun filter pred [] = []
  | filter pred (x::xs) =
                if pred(x) then x::(filter pred xs)
                else (filter pred xs);


fun isfull n soln = (n*n = length(soln));


fun safe n soln newpos = not(belongs(newpos,soln)) andalso (n>hd(newpos) andalso n>hd(tl(newpos)) andalso hd(newpos)>0 andalso hd(tl(newpos))>0);
	
fun possible(soln) =[hd(soln)+1,hd(tl(soln))+2]::[hd(soln)+1,hd(tl(soln))-2]::[hd(soln)-1,hd(tl(soln))+2]::[hd(soln)-1,hd(tl(soln))-2]::[hd(soln)+2,hd(tl(soln))+1]::[hd(soln),hd(tl(soln))-1]::[hd(soln)-2,hd(tl(soln))+1]::[hd(soln)-2,hd(tl(soln))-1]::[];

fun next n soln =
	map (secr op:: soln)  (filter (safe n soln) (possible(hd(soln))));


fun upto(m,i,n) =			
	if(i>n) then []	
	else if (m>n) then upto(1,i+1,n)
	else [m,i]::upto(m+1,i,n);

fun genmat n=upto(1,1,n);

fun knight n = depthfirst  (next n) (isfull n) (genmat n);

