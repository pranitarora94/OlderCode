//
//  output.cpp
//  AI_assignment2
//
//  Created by Pranit Arora on 25/02/15.
//  Copyright (c) 2015 Pranit Arora. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc , const char * argv[]){
	int v , e , k;
	fstream File;
	File.open(argv[2], ios::in);
	File>>v>>e>>k;
	int edge[e][2];
	for(int i = 0 ; i < e ; i++){
		File>>edge[i][0]>>edge[i][1];
	}
	File.close();
	File.open(argv[1],ios::in);
	int NumVar;
    NumVar = (v*k) + (v-1)*v + (k*(k-1))*v + k* ((v*(v-1))/2);
    string check;
    bool values[v][k];
    int temp;
	File>>check;
    int G[k];
    for(int i = 0 ; i < k ; i++){
    	G[i] = 0;
    }
	if(check == "UNSAT"){
    	cout<<0<<endl;
    }
    else{
    	for(int i = 0 ; i < v ; i++){
    		for(int j = 0 ; j < k ; j++){
    			File>>temp;
    			temp /= (i*v)+j+1;
    			if(temp == 1){
    				values[i][j] = true;
    				G[j]++;
    			}
    			else{
    				values[i][j] = false;
    			}
    		}
    	}
    	File.close();
    	File.open("test.subgraphs" , ios::out);
    	for(int i = 0 ; i < k ; i++){
    		File<<"#"<<i+1<<" "<<G[i]<<endl;
			for(int j = 0 ; j < v ; j++){
    			if(values[j][i] == true){
    				File<<j+1<<" ";
    			}
    		}
    		cout<<endl;
   	 	}
    }
    File.close();
    return 0;
}
