//
//  main.cpp
//  AI_assignment2
//
//  Created by Pranit Arora on 25/02/15.
//  Copyright (c) 2015 Pranit Arora. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(int argc , const char * argv[]){
	int v , e , k;
	fstream File;
	File.open(argv[1], ios::in);
	File>>v>>e>>k;
	int edge[e][2];
    
	//for(int i = 0 ; i < e ; i++){
	//	File>>e[i][0]>>e[i][1];
	//}
	bool a[v][k];
	bool b[e][k];
    
    //Define uniary condition for edge matrix e (v*v)
    //for every 1 in edge matrix, put pos. clause. For every 0, put negative
    vector<vector<int> > c(v, vector<int> (v));        //declare zeros
    for(int i=0;i<e;i++)
    {
        int temp1,temp2;
        File>>temp1>>temp2;
        c[temp1-1][temp2-1] = 1;
        c[temp2-1][temp1-1] = 1;
    }
    File.close();
    
    File.open("input.satinput", ios::out);
	int NumVar=0, NumCons=0;
    NumVar = (v*k) + (v-1)*v + (k*(k-1))*v + k* ((v*(v-1))/2);
    NumCons = (v-1)*v + v + k + 3 * (k*(k-1))*v + k*(k-1) + 3 * k * ((v*(v-1))/2) + ((v*(v-1))/2) + k* ((v*(v-1))/2);
	File<<"p cnf "<<NumVar<<" "<<NumCons<<endl;
    
    //at least 1 vertex per subgraph
	for(int i = 0 ; i < v ; i++){
		for(int j = 1 ; j <= k ; j++){
			File<<i*k+j<<" ";
		}
		File<<"0"<<endl;
	}
    
    for(int i=1;i<v;i++)
    {
        for(int j=0;j<i;j++)
        {
            if(c[i][j]==1)
            {
                //2 conditions, both upper and lower [i][j] & [j][i]
                File<< ((v)*k) + (i*v+j+1) - (i)<<" 0"<<endl;
                File<< ((v)*k) + (j*v+i+1) - (j+1)<<" 0"<<endl;
            }
            else
            {
                File<<(-1) * (((v)*k) + (i*v+j+1) -(i))<<" 0"<<endl;
                File<<(-1) * (((v)*k) + (j*v+i+1) - (j+1))<<" 0"<<endl;
            }
        }
    }
    //numbering correct as per copy
    
    
	
    /*
     //at least 1 edge per subgraph
     for(int i = 0 ; i < e ; i++){
     File<<endl;
     for(int j = 1 ; j <= k ; j++){
     File<<(v*k)+(i*k)+j<<" ";
     }
     }
     */
	//no empty subgraph
	for(int j = 1 ; j <= k ; j++){
		
		for(int i = 0 ; i < v ; i++){
			File<<i*k+j<<" ";
		}
		File<<"0"<<endl;
	}
    //defining d1 matrix (n*k^2) =1 if apq ^ ~apr (p->v , q,r->K)
    // Simply, =1 if vertex p is in subgraph q, and not in subgraph r
    //column order of matrix is 21, 31, 32 ,41 ,42, 43 .....
    for(int p=0;p<v;p++)
    {
        for(int q=1;q<k;q++)
        {
            for(int r=0; r<q;r++)
            {
                //~D[p][qr] OR A[p][q]
                File<<(-1)*(((v)*k + v*(v-1)) + (p* (k*(k-1))/2) + ((q*(q-1))/2) + r + 1)<<" "<<p*k+q+1<<" 0"<<endl;
                //~D[p][qr] OR A[p][r]
                File<<(-1)*(((v)*k + v*(v-1)) + (p* (k*(k-1))/2) + ((q*(q-1))/2) + r + 1)<<" "<<-1*(p*k+r+1)<<" 0"<<endl;
                //~a[p][q] OR a[p][r] OR D[p][qr]
                File<<(( (v)*k + v*(v-1)) + (p* (k*(k-1))/2) + ((q*(q-1))/2) + r + 1)<<" "<<p*k+r+1<<" "<<-1*(p*k+q+1)<<" 0"<<endl;
            }
        }
    }
    //defining d2 matrix, reverse of d1
    // Simply, =1 if vertex p is in subgraph r, and not in subgraph q
    for(int p=0;p<v;p++)
    {
        for(int q=1;q<k;q++)
        {
            for(int r=0; r<q;r++)
            {
                //~D[p][qr] OR A[p][q]
                File<<(-1)*(((v)*k + v*(v-1) + v*((k*(k-1))/2)) + (p* (k*(k-1))/2) + ((q*(q-1))/2) + r + 1)<<" "<<(-1)* (p*k+q+1)<<" 0"<<endl;
                //~D[p][qr] OR A[p][r]
                File<<(-1)*(((v)*k + v*(v-1) + v*((k*(k-1))/2)) + (p* (k*(k-1))/2) + ((q*(q-1))/2) + r + 1)<<" "<<p*k+r+1<<" 0"<<endl;
                //~a[p][q] OR a[p][r] OR D[p][qr]
                File<<(( (v)*k + v*(v-1) + v*((k*(k-1))/2)) + (p* (k*(k-1))/2) + ((q*(q-1))/2) + r + 1)<<" "<<(-1)*(p*k+r+1)<<" "<<p*k+q+1<<" 0"<<endl;
            }
        }
    }
    //define d3 matrix, 1 if vertex p and vertex q in subgraph r
    for(int p=1;p<v;p++)
    {
        for(int q=0;q<p;q++)
        {
            for(int r=0; r<k;r++)
            {
                //~D[pq][r] OR A[p][q]
                File<<(((v)*k + v*(v-1) + v*(k*(k-1))) + (((p*(p-1))/2) + q)*k + r + 1)<<" "<<(-1)* (p*k+r+1)<<" "<<(-1)* (q*k+r+1)<<" 0"<<endl;
                File<<(-1) * (((v)*k + v*(v-1) + v*(k*(k-1))) + (((p*(p-1))/2) + q)*k + r + 1)<<" "<<(p*k+r+1)<<" 0"<<endl;
				File<<(-1) * (((v)*k + v*(v-1) + v*(k*(k-1))) + (((p*(p-1))/2) + q)*k + r + 1)<<" "<< (q*k+r+1)<<" 0"<<endl;
                
                
            }
        }
    }
    
    
    
    //CHECK THE ABOVE
    //DO IT BOTH WAYS (checking )
    
    
    //subgraph of subgraph
    //Atleast one 1 in column of D1 and D2 (implies atleast one vertex in subgraph j and not in subgraph k)
    
    
    for(int i = 0 ; i < ((k*(k-1))/2) ; i++)
    {
        for(int j=0; j<v;j++)
        {
            File<< (( (v)*k + v*(v-1)) + 1 + j*((k*(k-1))/2) + i)<<" ";
        }
        File<<"0"<<endl;
    }
    for(int i = 0 ; i < ((k*(k-1))/2) ; i++)
    {
        for(int j=0; j<v;j++)
        {
            File<< (( (v)*k + v*(v-1) + v*((k*(k-1))/2)) + 1 + j*((k*(k-1))/2) + i) <<" ";
        }
        File<<"0"<<endl;
    }
    //CHECK EXP.
    
    
    //complete subgraphs
    //if vertex i and j both are in subgraph k, then must exist edge i<->j .
    //Note: All edges will be fixed earlier by an upper loop, so this will set A matrix values
    
    /*
     for(int i=1;i<v;i++)
     {
     for(int j=0;j<i;j++)
     {
     for(int l=0;l<k;l++)
     {
     //Can reduce, find i,j for that e[i][j] ,then put or on K?? Is this wrong
     // ~a[i][k] OR ~a[j][k] OR e [i][k]
     File<< (-1)* ((i*k)+l+1)<<" "<<(-1)* ((j*k)+l+1)<<" "<< ((v)*k) + (i*v+j+1) - (i)<<" 0"<<endl;
     //Also, if not in same graph, no edge
     File<< (-1)* ((i*k)+l+1)<<" "<<((j*k)+l+1)<<" "<<(-1)* (((v)*k) + (i*v+j+1) - (i))<<" 0"<<endl;
     File<< ((i*k)+l+1)<<" "<<(-1)* ((j*k)+l+1)<<" "<<(-1)* (((v)*k) + (i*v+j+1) - (i))<<" 0"<<endl;
     }
     }
     }
     */
    for(int i=1;i<v;i++)
    {
        for(int j=0;j<i;j++)
        {
            for(int l=0;l<k;l++)
            {
                
                //D3 [ij][l] OR...
                File<<(((v)*k + v*(v-1) + v*(k*(k-1))) + (((i*(i-1))/2) + j)*k + l + 1)<<" ";
            }
            //OR not edge i,j
            File<<(-1)*(((v)*k) + (i*v+j+1) - (i))<<" 0"<<endl;
        }
    }
    //counterpositive of above
    for(int i=1;i<v;i++)
    {
        for(int j=0;j<i;j++)
        {
            for(int l=0;l<k;l++)
            {
                
                
                File<<(-1)*(((v)*k + v*(v-1) + v*(k*(k-1))) + (((i*(i-1))/2) + j)*k + l + 1)<<" "<<(((v)*k) + (i*v+j+1) - (i))<<" 0"<<endl;
            }
        }
    }
    File.close();
    return 0;
}

