//
//  main.cpp
//  CSL333_A1_1.0
//
//  Created by Pranit Arora on 26/01/15.
//  Copyright (c) 2015 Pranit Arora. All rights reserved.
//

 //#include <iostream>
 //#include <fstream>
 //#include <map>
 //#include <vector>
 //#include <string>
 //#include <limits.h>
 //#include <algorithm>
 //#include <queue>
 //#include <numeric>
 //#include <limits.h>
#include<bits/stdc++.h>

using namespace std;



vector<vector<int> > MC;
int CC;
int size_V,K;
vector<char> V;
vector<string> XStr;
vector<string> CurrentStrings;
map<string, int> CostMap;
map<char, int> MyMap;
vector< int> InitLengths;
vector< vector<int> > HCostMatrix;
vector< vector<int> > XInd;
map<int, vector<int> > VectorMap;
map<int, vector<vector<int> > > SolnMap;
int HMatSize;


void CreateCostMap(vector<int> VecTillNow,int Cost, string StrTillNow)


// This calculates the cost of any possible step(as per the Vocab) and stores it in a Map
{
    //increase size of vector, if size=K, put in Map, else Expand further
    int tempCost;
    vector<int> temp;
    for(int i=0;i<size_V;i++)
    {
        
        temp= VecTillNow;
        temp.push_back(i);
        tempCost = Cost;
        for(int j=0; j<VecTillNow.size();j++)
        {
            tempCost= tempCost + MC[i][VecTillNow[j]];          //increase cost for so far
        }
        if(temp.size()<K)                                       //check size
        {
            CreateCostMap(temp, tempCost, StrTillNow+V[i]);                      //increase size
        }
        else
        {
            CostMap[StrTillNow+V[i]] = tempCost;                           //add to map
        }
    }
    //Code for '-'
    temp= VecTillNow;
    temp.push_back(size_V);
    
    tempCost = Cost;
    for(int j=0; j<VecTillNow.size();j++)
    {
        tempCost= tempCost + MC[size_V][VecTillNow[j]];          //increase cost for so far
    }
    tempCost+=CC;
    
    if(temp.size()<K)                                           //check size
    {
        CreateCostMap(temp, tempCost, StrTillNow+'-');
    }
    else
    {
        CostMap[StrTillNow+'-'] = tempCost;
    }
    return;
}


int CostColumn(string Str)
{
    int Cost=0;
    if(Str[0]=='-')
    {
        Cost+=CC;
    }
    for(int i=1;i<Str.length();i++)
    {
        for(int j=0;j<i;j++)
        {
            Cost+= MC[MyMap[Str[i]]][MyMap[Str[j]]];
        }
        if(Str[i]=='-')
        {
            Cost+=CC;
        }
    }
    return Cost;
}



bitset<28> AddOneBit(bitset<28> bitnum)
//increases the bitset representation by one. Seems to run faster like this
{
    bool ToSwitch;
    for(int i=K;i>=0;i--)
    {
        ToSwitch=true;
        int j=0;
        while(ToSwitch && j<i)
        {
            ToSwitch= (ToSwitch && bitnum.test(j));
            j++;
        }
        if(ToSwitch)
        {
            bitnum.flip(i);
        }
    }
    return bitnum;
}


vector<int> ConvertStringToVec(string Str,map<char, int> MyMap)
//convert the 'n' length string to an 'n' length vector
{
    vector<int> Ind(Str.length());
    for(int i=0;i<Str.length();i++)
    {
        Ind[i]=MyMap[Str[i]];           //converts string to vec<int>
    }
    return Ind;
}


void GenerateHeuristicCost()
//one time execution. Creates Pairwise Cost for all the possible substrings and stores in matrix, refered to by indices of the pair, ie for strings(vectors) i,j, the cost matrix is HMAtrices[i-1][j]
{
    vector<int> v1,v2;
    HCostMatrix.resize(K);
    
    vector<vector<int> > HMatrix(K,vector<int>(K));
    int t1,t2,t3,t4,t5,t6;
    int L1,L2;
    pair<int, int> MyPair;
    for(int i=1;i<K;i++)
    {
        for(int j=0;j<i;j++)
        {
            v1=XInd[i];
            v2=XInd[j];
            L1= v1.size();
            L2=v2.size();
            vector<vector<int> > Matrix(L1+1,vector<int>(L2+1,0));               //check if 0 init.
            for (int k=1; k<=L1; k++)
            {
                Matrix[k][0]= Matrix[k-1][0]+ MC[v1[k-1]][size_V];
            }
            for (int k=1; k<=L2; k++)
            {
                Matrix[0][k]= Matrix[0][k-1]+ MC[size_V][v2[k-1]];
            }
            for(int k=1;k<=L1;k++)
            {
                for(int l=1;l<=L2;l++)
                {
                    t1= Matrix[k-1][l-1];
                    t2= MC[v1[k-1]][v2[l-1]];
                    t3= Matrix[k-1][l] ;
                    t4= MC[v1[k-1]][size_V];
                    t5=  Matrix[k][l-1] ;
                    t6= MC[size_V][v2[l-1]];
                    Matrix[k][l] = min(t1+t2  ,min( t3+t4 , t5+t6));
                }
            }
            HMatrix[i][j] = Matrix[L1][L2];
            HMatrix[j][i] = Matrix[L1][L2];
        }
    }
    HCostMatrix = HMatrix;
    
}

vector<vector<int> > MatchStrings(vector<vector<int> > v1, vector<vector<int> > v2)
{
    vector<vector<int> > Final1 (v1.size());
    vector<vector<int> > Final2 (v2.size());
    int L1= v1[0].size();
    int v1_Size = v1.size();
    int L2=v2[0].size();
    int v2_Size =v2.size();
    for(int m=0;m<v1_Size;m++)
    {
        reverse(v1[m].begin(), v1[m].end());              //reverse strings for ease of future reference
    }
    for(int m=0;m<v2_Size;m++)
    {
        reverse(v2[m].begin(), v2[m].end());              //reverse strings for ease of future reference
    }
    
    int t1,t2,t3,t4,t5,t6;
    vector<vector<int> > Matrix(L1+1,vector<int>(L2+1,0)),  DirMatrix(L1+1,vector<int>(L2+1,1));               //check if 0 init.

    for(int k=1;k<=L1;k++ )
    {
        DirMatrix[k][0] = 2;
    }
    
    for (int k=1; k<=L1; k++)
    {
        Matrix[k][0]= Matrix[k-1][0] + v2_Size*CC;
        for(int m=0;m<v2_Size;m++)
        {
            for(int n=0;n<v1_Size;n++)
            {
                Matrix[k][0]+= MC[v1[n][k-1]][size_V];
            }
        }
        
    }
    for (int k=1; k<=L2; k++)
    {
        Matrix[0][k]= Matrix[0][k-1] + v1_Size*CC;
        for(int m=0;m<v1_Size;m++)
        {
            for(int n=0;n<v2_Size;n++)
            {
                Matrix[0][k]+= MC[size_V][v2[n][k-1]] ;
            }
        }
    }
    
    
    for(int k=1;k<=L1;k++)
    {
        for(int l=1;l<=L2;l++)
        {
            t1= Matrix[k-1][l-1];
            t2=0;
            for(int m=0;m<v1_Size;m++)
            {
                for(int n=0;n<v2_Size;n++)
                {
                    t2+= MC[v1[m][k-1]][v2[n][l-1]];
                }
            }
            t3= Matrix[k-1][l] ;
            t4= v2_Size*CC;
            for(int m=0;m<v2_Size;m++)
            {
                for(int n=0;n<v1_Size;n++)
                {
                    t4+= MC[v1[n][k-1]][size_V];
                }
            }
            
            t5 = Matrix[k][l-1] ;
            t6=v1_Size*CC;
            for(int m=0;m<v1_Size;m++)
            {
                for(int n=0;n<v2_Size;n++)
                {
                    t6+= MC[size_V][v2[n][l-1]] ;
                }
            }
            t1 = t1+t2;
            t2 = t3+t4;
            t3 = t5+t6;
            Matrix[k][l] = min(t1  ,min( t2 , t3));
            
            if(t1<=t2 && t1<=t3)
            {
                DirMatrix[k][l] = 3;
            }
            else if (t2<= t3)
            {
                DirMatrix[k][l] = 2;
            }
        }
    }
    int k=L1, l=L2;
    
    
    while(k>0 && l>0)
    {
        if(DirMatrix[k][l]==3)
        {
            k--;
            l--;
            for(int m=0;m<v1_Size;m++)
            {
                Final1[m].push_back(v1[m][k]);
            }
            for(int m=0;m<v2_Size;m++)
            {
                Final2[m].push_back(v2[m][l]);
            }
           
        }
        else if(DirMatrix[k][l]==2)
        {
            k--;
            for(int m=0;m<v1_Size;m++)
            {
                Final1[m].push_back(v1[m][k]);
            }
            for(int m=0;m<v2_Size;m++)
            {
                Final2[m].push_back(size_V);
            }
        }
        else
        {
            l--;
            for(int m=0;m<v1_Size;m++)
            {
                Final1[m].push_back(size_V);
            }
            for(int m=0;m<v2_Size;m++)
            {
                Final2[m].push_back(v2[m][l]);
            }
        }
    }
    while(k>0)
    {
        k--;
        for(int m=0;m<v1_Size;m++)
        {
            Final1[m].push_back(v1[m][k]);
        }
        for(int m=0;m<v2_Size;m++)
        {
            Final2[m].push_back(size_V);
        }
    }
    while (l>0)
    {
        l--;
        for(int m=0;m<v1_Size;m++)
        {
            Final1[m].push_back(size_V);
        }
        for(int m=0;m<v2_Size;m++)
        {
            Final2[m].push_back(v2[m][l]);
        }
    }
    for(int n=0;n<v2_Size;n++)
    {
        Final1.push_back(Final2[n]);
    }
    return Final1;
}



pair<int, int> MinPair()
{
    pair<int, int> MyPair;
    int min = numeric_limits<int>::max();
    for(int i=1;i<=HMatSize;i++)
    {
        for(int j=0;j<i;j++)
        {
            if(HCostMatrix[i][j]<min)
            {
                min =HCostMatrix[i][j];
                MyPair = make_pair(i, j);
            }
        }
    }
    return MyPair;
}

vector<vector<int> > ClustalW()
{
    GenerateHeuristicCost();
    vector<int> Sum(K);
    int NumStr = K;
    pair<int,int> MyPair;
    
    while(NumStr!=0)
    {
        vector<int> NextVec;
        vector<vector<int> > NewSoln;
        int firstNum, secondNum;
        vector<vector<int> > First, Second;
        
        MyPair = MinPair();
        firstNum = MyPair.first;
        secondNum = MyPair.second;
        //cout<<firstNum<<" "<<secondNum<<" = "<<HMatSize+1<<"\n";
        
        if(firstNum<K)
        {
            First.push_back(XInd[firstNum]);
            NextVec.push_back(firstNum);
        }
        else
        {
            vector<int> temp = VectorMap[firstNum];
            for(int i=0;i<temp.size();i++)
            {
                vector<vector<int> > tempMat = SolnMap[firstNum];
                vector<int> tempVec = tempMat[i];
                First.push_back(tempVec);
                NextVec.push_back(temp[i]);
            }
            
        }
        if(secondNum<K)
        {
            Second.push_back(XInd[secondNum]);
            NextVec.push_back(secondNum);
        }
        else
        {
            vector<int> temp = VectorMap[secondNum];
            for(int i=0;i<temp.size();i++)
            {
                vector<vector<int> > tempMat = SolnMap[secondNum];
                vector<int> tempVec = tempMat[i];
                Second.push_back(tempVec);
                NextVec.push_back(temp[i]);
            }
        }
        NewSoln =  MatchStrings(First, Second);
        HMatSize++;
        VectorMap[HMatSize] = NextVec;
        SolnMap[HMatSize] = NewSoln;
        
        vector<int> BestVec= HCostMatrix[firstNum];
        for(int i=0;i<HMatSize;i++)
        {
            if(HCostMatrix[secondNum][i]<BestVec[i])
            {
                BestVec[i] = HCostMatrix[secondNum][i];
            }
            HCostMatrix[i].push_back(BestVec[i]);
        }
        BestVec.push_back(numeric_limits<int>::max());
        HCostMatrix.push_back(BestVec);
        for(int i=0;i<NextVec.size();i++)
        {
            for(int j=0;j<=HMatSize;j++)
            {
                HCostMatrix[NextVec[i]][j] = numeric_limits<int>::max();
                HCostMatrix[j][NextVec[i]] = numeric_limits<int>::max();
            }
        }
        if(firstNum>=K)
        {
            for(int j=0;j<=HMatSize;j++)
            {
                HCostMatrix[firstNum][j] = numeric_limits<int>::max();
                HCostMatrix[j][firstNum] = numeric_limits<int>::max();
            }
        }
        if(secondNum>=K)
        {
            for(int j=0;j<=HMatSize;j++)
            {
                HCostMatrix[secondNum][j] = numeric_limits<int>::max();
                HCostMatrix[j][secondNum] = numeric_limits<int>::max();
            }
        }
        
    	if(NextVec.size()>=K) {
			vector<vector<int> > ReturnAns(K);
			for(int i=0;i<K;i++) {
				ReturnAns[NextVec[i]] = NewSoln[i];
			}
			return ReturnAns;
		}
        NumStr = K-NextVec.size();
    }
    return XInd;    //NEVER CALLED
}



int main(int argc, const char * argv[])
{
    //Take input
    fstream File;
    File.open("input.txt",ios::in);
    
    float Time;
    int i=0, j;
    char comm;
    File>>Time;
    File>>size_V;
    Time= 60*Time;                      //convert to seconds
    V.resize(size_V);
    while(i<(size_V-1))
    {
        File>>V[i]>>comm;                //input of vocab
        MyMap[V[i]] = i;                //stores map for each letter
        i++;
    }
    File>>V[i];
    MyMap[V[i]] = i;
    File>>K;
    XStr.resize(K);
    vector< int> temp;
    i=0;
    int totalLengthOfString=0;
    
    while(i<K)
    {
        File>>XStr[i];               
        temp= ConvertStringToVec(XStr[i],MyMap);
        totalLengthOfString +=XStr[i].length();
        InitLengths.push_back(XStr[i].length());
        XInd.push_back(temp);
        i++;
    }
    File>>CC;
    i=0;
    while(i<(size_V+1))
    {
        j=0;
        vector< int> temp(size_V+1);
        while (j<(size_V+1))
        {
            File>>temp[j];
            j++;
        }
        MC.push_back(temp);
        i++;
    }
    char hash;
    File>>hash;
    File.close();
    HMatSize = K-1;
    vector<int> empty;
    //CreateCostMap(empty, 0, "");
    //Search(Time);
   
    vector<vector<int> > Answer = ClustalW();
    
    V.push_back('-');
    File.open("output.txt", ios::out);
    for(int i=0;i<Answer.size();i++)
    {
        for(int j =0;j<Answer[i].size();j++)
        {
            //cout<<V[Answer[i][j]];
            File<<V[Answer[i][j]];
        }
        //cout<<"\n";
        File<<"\n";
    }
    int Cost=0;
    
    
    for(int i=0;i<Answer[0].size();i++)
    {
        string Col ="";
        for(int j=0;j<K;j++)
        {
            Col+=V[Answer[j][i]];
        }
        Cost += CostColumn(Col) ;
    }
    //cout<<"Cost = "<<Cost<<"\n";
    File.close();
    return 0;
}

