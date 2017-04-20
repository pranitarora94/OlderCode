function  F = mel221_cost(X)

F= 2*X(1) + 1* (X(1)^2);                 %opt1: 3, opt2:8 
F= F+ 3*X(2) - 1* (X(2)^2);              %opt1: 2, opt2:2    
F= F+ 10*X(3) - 3* (X(3)^2);             %opt1: 7, opt2:8
F= F+ 2*X(4);                            %opt1: 2, opt2:4
F= F+ 4*X(5) - 2* (X(5)^2);              %opt1: 2, opt2:0
F= F+ 4*X(6) - 1* (X(6)^2);              %opt1: 3, opt2:4
F= F+ 7*X(7) - 3* (X(7)^2);              %opt1: 4, opt2:2
F= F+ 7*X(8) - 2* (X(8)^2);              %opt1: 5, opt2:6
F= F+ 3*X(9) - 1* (X(9)^2);              %opt1: 2, opt2:4


end

