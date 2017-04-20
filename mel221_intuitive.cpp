//
//  main.cpp
//  MEL221_intuitive
//
//  Created by Pranit Arora on 13/10/14.
//  Copyright (c) 2014 Pranit Arora. All rights reserved.
//

#include <iostream>

using namespace std;




double calcZ(double a1,double a2,double b , double px, double py,double Z, double &opx ,double &opy)
{
    double temp=(a1*px + a2*py +b);
    if(temp>Z)
    {
        Z = temp;
        opx=px;
        opy=py;
    }
    return Z;
}

double find_feas_intercept(double p1,double p2, double a, double b,double Z, double &opx,double &opy)
{
    if(p1<0 && p2>0)
        Z = calcZ(a, 0, b, p2, 0, Z,opx,opy);
    else if (p2<0 )
        Z = calcZ(a, 0, b, p1, 0, Z,opx,opy);
    else if(p1<p2)
        Z = calcZ(a, 0, b, p1, 0, Z,opx,opy);
    else
        Z = calcZ(a, 0, b, p2, 0, Z,opx, opy);
    return Z;
}

int main(int argc, const char * argv[])
{
    double a11,a12,a21,a22,b1,b2,a31,a32,b3;
    double p1x,p1y,p2x,p2y,p3x,p3y,p4x,p4y,p5x,p5y,p6x,p6y,Z,opx,opy;
   
    a11=1;a12=1;b1=6;a21=-2;a22=3;b2=6;a31=4;a32=2;b3=0;
    p1x=0;p1y=0;
    p2x=(b1/a11);p2y=0;
    p3x=(b2/a21);p3y=0;
    p4x=0;p4y=(b1/a21);
    p5x=0;p5y=(b2/a22);
    p6x = ((b1*a22)-(b2*a12) ) / ((a11*a22)-(a21*a12));
    p6y = ((b1*a21)-(b2*a11) ) / ((a12*a21)-(a22*a11));
    if(p6x<0 ||p6y<0)
    {
        p6x=0;p6y=0;
    }
    Z= a31*p1x + a32*p1y +b3;
    opx=p1x;opy=p1y;
    Z = find_feas_intercept(p2x, p3x, a31, b3, Z,opx,opy);
    Z = find_feas_intercept(p4y,p5y, a32, b3, Z,opx,opy);
    Z = calcZ(a31, a32, b3, p6x, p6y, Z, opx, opy);
    cout<<"Maximum value of Z is "<<Z<<" at ("<<opx<<","<<opy<<").\n";
}

