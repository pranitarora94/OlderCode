//
//  main.cpp
//  AI_A5
//
//  Created by Pranit Arora on 23/04/15.
//  Copyright (c) 2015 Pranit Arora. All rights reserved.
//
//
//  main.cpp
//  Testing123
//
//  Created by Pranit Arora on 14/03/15.
//  Copyright (c) 2015 Pranit Arora. All rights reserved.
//



#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
//#include <bits/stdc++.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <set>
#include <limits.h>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <stack>
#include <queue>
#include <utility>

using namespace std;
/* Complete the function below to print 1 integer which will be your next move
 */

double MaxVal(struct State* initState, double alpha, double beta, int depth, int MaxDepth);
double MinVal(struct State* initState, double alpha, double beta, int depth, int MaxDepth);

int N,M,K, time_left, player;
int Opp_Goal, My_Goal;
bool GameWon;
int NodesExplored;
float TL;

vector<vector<bool> > isHwallAllowed, isVwallAllowed ;

vector< vector< bool> > AdjMatrix;      //have to resize after getting N,M
int bestAction;
pair<int, int> bestMove;

set< pair<int, int> > HWallsPosRem;
set< pair<int, int> > VWallsPosRem;

struct State
{
    int My_Row, My_Col, Opp_Row, Opp_Col;
    int My_Walls_Rem, Opp_Walls_Rem;
};

pair<int,int> bfs(struct State* MyState)
{
    int x = MyState->My_Row;
    int y = MyState->My_Col;
    int g = My_Goal;
    
    
    queue<pair<int, int> > q;
    pair<int,int> p;
    p.first = x;
    p.second = y;
    q.push(p);
    vector<vector<bool> > discovered(N);
    for (int i = 0; i < N; ++i)
    {
        vector<bool> vec(M,0);
        discovered[i] = vec;
    }
    
    vector<vector<pair<int, int> > > parent(N);
    for (int i = 0; i < N; ++i)
    {
        pair<int, int> dummy = make_pair(-1, -1);
        vector<pair<int, int> > vec(M, dummy);
        parent[i] = vec;
    }
    
    discovered[x][y] = true;
    
    while (!q.empty())
    {
        p = q.front();
        q.pop();
        
        if (p.first == g)
        {
            pair<int, int> tom = parent[p.first][p.second];
            while (tom.first != x || tom.second != y)
            {
                p = tom;
                tom = parent[p.first][p.second];
            }
            return p;
        }
        
        int a = M * p.first + p.second;
        
        pair<int,int> pu;
        
        if (a-1 >= 0)
        {
            if (AdjMatrix[a][a-1] && !discovered[p.first][p.second-1])
            {
                pu.first = p.first;
                pu.second = p.second - 1;
                q.push(pu);
                discovered[pu.first][pu.second] = true;
                parent[pu.first][pu.second] = make_pair(p.first, p.second);
            }
        }
        if (a+1 < N*M)
        {
            if (AdjMatrix[a][a+1] && !discovered[p.first][p.second+1])
            {
                pu.first = p.first;
                pu.second = p.second + 1;
                q.push(pu);
                discovered[pu.first][pu.second] = true;
                parent[pu.first][pu.second] = make_pair(p.first, p.second);
            }
        }
        if (a-M >= 0)
        {
            if (AdjMatrix[a][a-M] && !discovered[p.first-1][p.second])
            {
                pu.first = p.first-1;
                pu.second = p.second;
                q.push(pu);
                discovered[pu.first][pu.second] = true;
                parent[pu.first][pu.second] = make_pair(p.first, p.second);
            }
        }
        if (a+M < N*M)
        {
            if (AdjMatrix[a][a+M] && !discovered[p.first+1][p.second])
            {
                pu.first = p.first+1;
                pu.second = p.second;
                q.push(pu);
                discovered[pu.first][pu.second] = true;
                parent[pu.first][pu.second] = make_pair(p.first, p.second);
            }
        }
    }
    return p;
}


bool connected(int x, int y, int g)
{
    stack<pair<int, int> > stk; // add #include <stack>
    pair<int,int> p;
    p.first = x;
    p.second = y;
    stk.push(p);
    
    vector<vector<bool> > explored(N);
    for (int i = 0; i < N; ++i)
    {
        vector<bool> vec(M,0);
        explored[i] = vec;
    }
    
    while (!stk.empty())
    {
        p = stk.top();
        stk.pop();
        
        if (p.first == g)
        {
            return true;
        }
        
        if (!explored[p.first][p.second])
        {
            explored[p.first][p.second] = true;
            int a = M * p.first + p.second;
            
            pair<int,int> q;
            if (a-1 >= 0)
            {
                if (AdjMatrix[a][a-1])
                {
                    q.first = p.first;
                    q.second = p.second - 1;
                    stk.push(q);
                }
            }
            if (a+1 < N*M)
            {
                if (AdjMatrix[a][a+1])
                {
                    q.first = p.first;
                    q.second = p.second + 1;
                    stk.push(q);
                }
            }
            if (a-M >= 0)
            {
                if (AdjMatrix[a][a-M])
                {
                    q.first = p.first - 1;
                    q.second = p.second;
                    stk.push(q);
                }
            }
            if (a+M < N*M)
            {
                if (AdjMatrix[a][a+M])
                {
                    q.first = p.first + 1;
                    q.second = p.second;
                    stk.push(q);
                }
            }
        }
    }
    return false;
}


int distance_to_goal(int x, int y, int g)
{
    queue<pair<int, int> > q;
    pair<int,int> p;
    p.first = x;
    p.second = y;
    q.push(p);
    
    vector<vector<bool> > discovered(N);
    for (int i = 0; i < N; ++i)
    {
        vector<bool> vec(M,0);
        discovered[i] = vec;
    }
    
    vector<vector<int> > distance(N);
    for (int i = 0; i < N; ++i)
    {
        vector<int> vec(M, 200000000);
        distance[i] = vec;
    }
    
    discovered[x][y] = true;
    distance[x][y] = 0;
    
    while (!q.empty())
    {
        p = q.front();
        q.pop();
        
        if (p.first == g)
        {
            return distance[p.first][p.second];
        }
        
        int a = M * p.first + p.second;
        
        pair<int,int> pu;
        
        if (a-1 >= 0)
        {
            if (AdjMatrix[a][a-1] && !discovered[p.first][p.second-1])
            {
                pu.first = p.first;
                pu.second = p.second - 1;
                q.push(pu);
                discovered[pu.first][pu.second] = true;
                distance[pu.first][pu.second] = distance[p.first][p.second] + 1;
            }
        }
        if (a+1 < N*M)
        {
            if (AdjMatrix[a][a+1] && !discovered[p.first][p.second+1])
            {
                pu.first = p.first;
                pu.second = p.second + 1;
                q.push(pu);
                discovered[pu.first][pu.second] = true;
                distance[pu.first][pu.second] = distance[p.first][p.second] + 1;
            }
        }
        if (a-M >= 0)
        {
            if (AdjMatrix[a][a-M] && !discovered[p.first-1][p.second])
            {
                pu.first = p.first-1;
                pu.second = p.second;
                q.push(pu);
                discovered[pu.first][pu.second] = true;
                distance[pu.first][pu.second] = distance[p.first][p.second] + 1;
                
            }
        }
        if (a+M < N*M)
        {
            if (AdjMatrix[a][a+M] && !discovered[p.first+1][p.second])
            {
                pu.first = p.first+1;
                pu.second = p.second;
                q.push(pu);
                discovered[pu.first][pu.second] = true;
                distance[pu.first][pu.second] = distance[p.first][p.second] + 1;
                
            }
        }
    }
    return -1;
}


int evaluateState(struct State* MyState, int depth)
{
    
    static const int walls_value[15] = {0, 150, 225, 275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550};
    
    int his_distance = distance_to_goal(MyState->Opp_Row, MyState->Opp_Col, Opp_Goal);
    int my_distance = distance_to_goal(MyState->My_Row, MyState->My_Col, My_Goal);
    if(GameWon)
    {
        
        if( MyState->My_Row !=My_Goal)
        {
            return 50 - 100 * my_distance;
        }
        else
        {
            return 100 * his_distance + 50;
        }
        
    }
    if (his_distance == 0)
    {
        return -200000000 + 100*depth - 10* my_distance;
    }
    if (my_distance == 0)
    {
        return 200000000 - 100*depth + 10* his_distance;
    }
    
    int score = 100 * (his_distance - my_distance) + 50;
    
    score += (std::rand()%20)-10;
    
    score += walls_value[MyState->My_Walls_Rem];
    score -= walls_value[MyState->Opp_Walls_Rem];
    
    
    score +=5*(MyState->My_Walls_Rem - MyState->Opp_Walls_Rem) * (MyState->Opp_Walls_Rem);
    
    
    if (his_distance >= my_distance && MyState->Opp_Walls_Rem==0)
        score += 1000;
    if (my_distance > his_distance && MyState->My_Walls_Rem==0)
        score -= 1000;
    
    return score;
}



/*
 double evaluateState (struct State* myState, int depth)
 {
 //double val=rand()%3;
 double val = 0;
 val += abs(Opp_Goal - myState->My_Row);
 val -= abs(My_Goal - myState->Opp_Row);
 if(myState->My_Row ==My_Goal)
 {
 val += 100 - depth;
 }
 if(myState->Opp_Row == Opp_Goal)
 {
 val -= 100 - depth;
 
 }
 return val;
 
 }
 
 */

double MaxVal(struct State* initState, double alpha, double beta, int depth, int MaxDepth)       //can make maxDepth global?
{
    if(initState->Opp_Row == Opp_Goal && !GameWon)
    {
        return evaluateState(initState, depth);
    }
    
    depth++;
    vector<pair<int, int> > BoardChanges;
    struct State* newState = (struct State*) malloc(sizeof(struct State));;
    newState->My_Row = initState->My_Row;
    newState->My_Col = initState->My_Col;                   //should I exchange my and opp row,col.If yes,needed for both Max & Min
    newState->Opp_Row = initState->Opp_Row;                 //change of ply
    newState->Opp_Col = initState->Opp_Col;
    newState->My_Walls_Rem = initState->My_Walls_Rem;
    newState->Opp_Walls_Rem = initState ->Opp_Walls_Rem;
    vector< pair<int, int > > PossMoves;
    PossMoves.resize(0);
    double ChildVal;
    
    //possible nodes: movement * 4
    int DiffRow = initState->My_Row - initState->Opp_Row, DiffCol = initState->My_Col - initState ->Opp_Col;
    
    if(initState->My_Row >0)
    {
        if(DiffRow==1 && DiffCol==0 && AdjMatrix[(initState->My_Row)*M + initState->My_Col][(initState->My_Row-1)*M + initState->My_Col] && initState->Opp_Row!=Opp_Goal)
        {
            if(initState->My_Row>1)
            {
                if(AdjMatrix[(initState->My_Row-1)*M + initState->My_Col ][(initState->My_Row-2)*M + initState->My_Col ])
                {
                    PossMoves.push_back(make_pair(initState->My_Row-2, initState->My_Col));
                }
            }
            if(initState->My_Row<=1 || !AdjMatrix[(initState->My_Row-1)*M + initState->My_Col ][(initState->My_Row-2)*M + initState->My_Col ])
            {
                if(initState->My_Col>0)
                {
                    if(AdjMatrix[(initState->My_Row-1)*M + initState->My_Col][(initState->My_Row -1)*M + initState->My_Col -1])
                    {
                        PossMoves.push_back(make_pair(initState->My_Row-1, initState->My_Col-1));
                    }
                }
                //if(initState->My_Col<8)
                if(initState->My_Col<M-1)
                {
                    if(AdjMatrix[(initState->My_Row-1)*M + initState->My_Col][(initState->My_Row -1)*M + initState->My_Col+1])
                    {
                        PossMoves.push_back(make_pair(initState->My_Row-1, initState->My_Col+1));
                    }
                }
            }
        }
        else
        {
            if(AdjMatrix[(initState->My_Row)*M + initState->My_Col ][(initState->My_Row-1)*M + initState->My_Col])
            {
                PossMoves.push_back(make_pair(initState->My_Row-1, initState->My_Col));
            }
        }
    }
    //if(initState->My_Row <8)
    if(initState->My_Row < N-1)
    {
        if(DiffRow==-1 && DiffCol==0 && AdjMatrix[(initState->My_Row)*M + initState->My_Col][(initState->My_Row+1)*M + initState->My_Col] && initState->Opp_Row!=Opp_Goal)
        {
            //if(initState->My_Row<7)
            if(initState->My_Row < N-2)
            {
                if(AdjMatrix[(initState->My_Row+1)*M + initState->My_Col][(initState->My_Row+2)*M + initState->My_Col])
                {
                    PossMoves.push_back(make_pair(initState->My_Row+2, initState->My_Col));
                }
            }
            //if(initState->My_Row>=7 || !AdjMatrix[(initState->My_Row+1)*M + initState->My_Col][(initState->My_Row+2)*M + initState->My_Col])
            if(initState->My_Row>= N-2 || !AdjMatrix[(initState->My_Row+1)*M + initState->My_Col][(initState->My_Row+2)*M + initState->My_Col])
            {
                if(initState->My_Col>0)
                {
                    if(AdjMatrix[(initState->My_Row+1)*M + initState->My_Col][(initState->My_Row +1)*M + initState->My_Col -1])
                    {
                        PossMoves.push_back(make_pair(initState->My_Row+1, initState->My_Col-1));
                    }
                }
                //if(initState->My_Col<8)
                if(initState->My_Col< M-1)
                {
                    if(AdjMatrix[(initState->My_Row+1)*M + initState->My_Col][(initState->My_Row +1)*M + initState->My_Col+1])
                    {
                        PossMoves.push_back(make_pair(initState->My_Row+1, initState->My_Col+1));
                    }
                }
            }
        }
        else
        {
            if(AdjMatrix[(initState->My_Row)*M + initState->My_Col][(initState->My_Row+1)*M + initState->My_Col])
            {
                PossMoves.push_back(make_pair(initState->My_Row+1, initState->My_Col));
            }
        }
    }
    //if(initState->My_Col<8)
    if(initState->My_Col< M-1)
    {
        if(DiffRow==0 && DiffCol==-1 && AdjMatrix[(initState->My_Row)*M + initState->My_Col ][(initState->My_Row)*M + initState->My_Col+1] && initState->Opp_Row!=Opp_Goal)
        {
            //if(initState->My_Col<7)
            if(initState->My_Col< M-2)
            {
                if(AdjMatrix[(initState->My_Row)*M + initState->My_Col +1][(initState->My_Row)*M + initState->My_Col+2])
                {
                    PossMoves.push_back(make_pair(initState->My_Row, initState->My_Col+2));
                }
            }
            //if (!AdjMatrix[(initState->My_Row)*M + initState->My_Col +1][(initState->My_Row)*M + initState->My_Col+2] || initState->My_Col>=7)
            if (!AdjMatrix[(initState->My_Row)*M + initState->My_Col +1][(initState->My_Row)*M + initState->My_Col+2] || initState->My_Col>=M-2)
            {
                if(initState->My_Row>0)
                {
                    if(AdjMatrix[(initState->My_Row-1)*M + initState->My_Col+1][(initState->My_Row)*M + initState->My_Col+1])
                    {
                        PossMoves.push_back(make_pair(initState->My_Row-1, initState->My_Col+1));
                    }
                }
                //if(initState->My_Row<8)
                if(initState->My_Row< N-1)
                {
                    if(AdjMatrix[(initState->My_Row+1)*M + initState->My_Col+1][(initState->My_Row)*M + initState->My_Col+1])
                    {
                        PossMoves.push_back(make_pair(initState->My_Row+1, initState->My_Col+1));
                    }
                }
            }
        }
        else
        {
            if(AdjMatrix[(initState->My_Row)*M + initState->My_Col][(initState->My_Row)*M + initState->My_Col+1])
            {
                PossMoves.push_back(make_pair(initState->My_Row, initState->My_Col+1));
            }
        }
    }
    
    if(initState->My_Col>0)
    {
        if(DiffRow==0 && DiffCol==1 && AdjMatrix[(initState->My_Row)*M + initState->My_Col][(initState->My_Row)*M + initState->My_Col -1] && initState->Opp_Row!=Opp_Goal)
        {
            if(initState->My_Col>1)
            {
                if(AdjMatrix[(initState->My_Row)*M + initState->My_Col -2][(initState->My_Row)*M + initState->My_Col -1])
                {
                    PossMoves.push_back(make_pair(initState->My_Row, initState->My_Col-2));
                }
            }
            if(initState->My_Col <=1 || !AdjMatrix[(initState->My_Row)*M + initState->My_Col -2][(initState->My_Row)*M + initState->My_Col -1])
            {
                if(initState->My_Row>0)
                {
                    if(AdjMatrix[(initState->My_Row-1)*M + initState->My_Col -1][(initState->My_Row)*M + initState->My_Col -1])
                    {
                        PossMoves.push_back(make_pair(initState->My_Row-1, initState->My_Col-1));
                    }
                }
                //if(initState->My_Row<8)
                if(initState->My_Row< N-1)
                {
                    if(AdjMatrix[(initState->My_Row+1)*M + initState->My_Col -1][(initState->My_Row)*M + initState->My_Col -1])
                    {
                        PossMoves.push_back(make_pair(initState->My_Row+1, initState->My_Col-1));
                    }
                }
            }
        }
        else
        {
            if(AdjMatrix[(initState->My_Row)*M + initState->My_Col][(initState->My_Row)*M + initState->My_Col -1])
            {
                PossMoves.push_back(make_pair(initState->My_Row, initState->My_Col-1));
            }
        }
    }
    
    
    if(initState->My_Row != My_Goal)
    {
        for(int Vi=0;Vi<PossMoves.size();Vi++)
        {
            NodesExplored++;
            newState->My_Row = PossMoves[Vi].first;
            newState->My_Col = PossMoves[Vi].second;
            //if(depth==1)
            //{
            //   cout<<" Poss Moves = "<<Vi<<" "<<PossMoves[Vi].first<<" "<<PossMoves[Vi].second<<endl;
            
            //}
            if (depth >= MaxDepth)
            {
                ChildVal = evaluateState(newState,depth);
            }
            else
            {
                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
            }
            if(ChildVal>alpha)
            {
                alpha = ChildVal;
                if(depth==1)
                {
                    bestAction = 0;
                    bestMove = make_pair(newState->My_Row, newState->My_Col);
                    //cout<<"Best: "<<bestMove.first<<" "<<bestMove.second<<endl;
                    if(depth ==1)
                    {
                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                    }
                }
            }
            if(alpha>=beta)
            {
                //keep track of best moves at each level??(Need move at depth 1)
                return ChildVal;
            }
        }
        
    }
    else
    {
        //pass
        NodesExplored++;
        if (depth >= MaxDepth)
        {
            ChildVal = evaluateState(initState,depth);
        }
        else
        {
            ChildVal = MinVal(initState, alpha, beta, depth, MaxDepth);
        }
        if(ChildVal>alpha)
        {
            alpha = ChildVal;
            bestAction = 0;
            bestMove = make_pair(0,0);
            if(depth ==1)
            {
                cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
            }
            
        }
        if(alpha>=beta)
        {
            //keep track of best moves at each level??(Need move at depth 1)
            return ChildVal;
        }
    }
    
    newState->My_Row = initState->My_Row;
    newState->My_Col = initState->My_Col;
    //only change My_Row etc or Opp_row etc
    
    //iterate through H & V possWallsRem
    set<pair<int, int> >::iterator it, temp;
    if(newState->My_Walls_Rem <=0)
    {
        return alpha;
    }
    
    newState->My_Walls_Rem--;
    
    
    bool DoEval = true;
    int First_Pos_Row = initState->Opp_Row + 2;
    int First_Pos_Col = initState->Opp_Col + 2;
    if(First_Pos_Row> N)
    {
        First_Pos_Row --;
    }
    if(First_Pos_Col>M)
    {
        First_Pos_Col --;
    }
    
    int SWidth = 3, SHeight = 4;
    int tempw = 0;
    int temp_go_left = First_Pos_Col-1 , temp_go_right  = First_Pos_Col ;
    while(tempw<SWidth)
    {
        //row_left col
        tempw++;
        if(temp_go_left>1)
        {
            int temp_go_up = First_Pos_Row -1, temp_go_down = First_Pos_Row;
            int temph = 0;
            while(temph<SHeight)
            {
                //col up check
                temph++;
                if(temp_go_up>1)
                {
                    if(isHwallAllowed[temp_go_up][temp_go_left])
                    {
                        //place Hwall at left, up
                        //check value
                        //undo everything
                        
                        BoardChanges.resize(0);
                        //temp =it;
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col= temp_go_left;
                        int Row = temp_go_up;
                        pair<int, int> MyPair = make_pair(Row, Col);
                        //cout<<"H"<<Row<<" "<<Col<<endl;
                        it = HWallsPosRem.find (MyPair);
                        temp =it;
                        
                        //Change Curr Board
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = false;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-1)*M + Col-2));
                            
                        }
                        if(AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] )
                        {
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-1, (Row-1)*M + Col-1));
                        }
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = true;
                            //it = HWallsPosRem.find(MyPair);
                            //it++;
                            
                            //continue;
                            DoEval = false;
                        }
                        
                        if(DoEval)
                        {
                            NodesExplored++;
                            
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(1);
                            HWallsPosRemove[0] = MyPair;
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(0);
                            
                            
                            // 1)The wall itself
                            HWallsPosRem.erase(temp);
                            isHwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = VWallsPosRem.find (MyPair);
                            if(temp!=VWallsPosRem.end())
                            {
                                VWallsPosRem.erase(temp);
                                VWallsPosRemove.push_back(MyPair);
                                isVwallAllowed[Row][Col] = false;
                            }
                            
                            // 3)The wall to left
                            if(Col>2)
                            {
                                MyPair.second = Col -1;
                                temp = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col-1] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            //if(Col<9)
                            if(Col < M)
                            {
                                MyPair.second = Col + 1;
                                temp = HWallsPosRem.find(MyPair);
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col+1] =false;
                                }
                            }
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal>alpha)
                            {
                                alpha = ChildVal;
                                if(depth==1)
                                {
                                    bestAction = 1;
                                    bestMove = make_pair(Row,Col);
                                    if(depth ==1)
                                    {
                                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                                    }
                                }
                            }
                            
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                        
                    }
                    if(isVwallAllowed[temp_go_up][temp_go_left])
                    {
                        //place Vwall at left, up
                        //check value
                        //undo everything
                        
                        
                        
                        //temp = it;
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        
                        int Col = temp_go_left;
                        int Row = temp_go_up;
                        pair<int, int> MyPair = make_pair(Row, Col);
                        it = VWallsPosRem.find (MyPair);
                        temp = it;
                        //cout<<"V"<<Row<<" "<<Col<<endl;
                        //ChangeCurrBoard
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = false;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-2)*M + Col-1));
                            
                        }
                        if(AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1])
                        {
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-1)*M + Col-2,(Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = true;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(0);
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(1);
                            VWallsPosRemove[0]= MyPair;
                            
                            // 1)The wall itself
                            VWallsPosRem.erase(temp);
                            isVwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = HWallsPosRem.find (MyPair);
                            if(temp!= HWallsPosRem.end())
                            {
                                HWallsPosRem.erase(temp);
                                HWallsPosRemove.push_back(MyPair);
                                isHwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Row>2)
                            {
                                MyPair.first = Row -1;
                                temp = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row-1][Col] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            //if(Row<9)
                            if(Row< N)
                            {
                                MyPair.first = Row + 1;
                                temp = VWallsPosRem.find(MyPair);
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row+1][Col] = false;
                                }
                            }
                            
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal>alpha)
                            {
                                alpha = ChildVal;
                                if(depth==1)
                                {
                                    bestAction = 2;
                                    bestMove = make_pair(Row,Col);
                                    if(depth ==1)
                                    {
                                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                                    }
                                }
                            }
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                            //MyPair.first = Row;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                    }
                }
                
                //col down check
                if(temp_go_down<N+1)
                {
                    if(isHwallAllowed[temp_go_down][temp_go_left])
                    {
                        //place Hwall at left, down
                        //check value
                        //undo everything
                        BoardChanges.resize(0);
                        
                        //temp =it;
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col= temp_go_left;
                        int Row = temp_go_down;
                        pair<int, int> MyPair = make_pair(Row, Col);
                        it = HWallsPosRem.find (MyPair);
                        //cout<<"H"<<Row<<" "<<Col<<endl;
                        temp =it;
                        //Change Curr Board
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = false;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-1)*M + Col-2));
                            
                        }
                        if(AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] )
                        {
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-1, (Row-1)*M + Col-1));
                        }
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = true;
                            //it = HWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval =false;
                        }
                        if(DoEval)
                        {
                            
                            NodesExplored++;
                            
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(1);
                            HWallsPosRemove[0] = MyPair;
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(0);
                            
                            
                            // 1)The wall itself
                            HWallsPosRem.erase(temp);
                            isHwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = VWallsPosRem.find (MyPair);
                            if(temp!=VWallsPosRem.end())
                            {
                                VWallsPosRem.erase(temp);
                                VWallsPosRemove.push_back(MyPair);
                                isVwallAllowed[Row][Col] = false;
                            }
                            
                            // 3)The wall to left
                            if(Col>2)
                            {
                                MyPair.second = Col -1;
                                temp = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col-1] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            //if(Col<9)
                            if(Col < M)
                            {
                                MyPair.second = Col + 1;
                                temp = HWallsPosRem.find(MyPair);
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col+1] =false;
                                }
                            }
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal>alpha)
                            {
                                alpha = ChildVal;
                                if(depth==1)
                                {
                                    bestAction = 1;
                                    bestMove = make_pair(Row,Col);
                                    if(depth ==1)
                                    {
                                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                                    }
                                }
                            }
                            
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                    }
                    if(isVwallAllowed[temp_go_down][temp_go_left])
                    {
                        //place Vwall at left, down
                        //check value
                        //undo everything
                        
                        
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        
                        int Col = temp_go_left;
                        int Row = temp_go_down;
                        pair<int, int> MyPair = make_pair(Row, Col);
                        it = VWallsPosRem.find (MyPair);
                        temp = it;
                        //cout<<"V"<<Row<<" "<<Col<<endl;
                        //ChangeCurrBoard
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = false;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-2)*M + Col-1));
                            
                        }
                        if(AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1])
                        {
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-1)*M + Col-2,(Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = true;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval= false;
                        }
                        if(DoEval)
                        {
                            
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(0);
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(1);
                            VWallsPosRemove[0]= MyPair;
                            
                            // 1)The wall itself
                            VWallsPosRem.erase(temp);
                            isVwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = HWallsPosRem.find (MyPair);
                            if(temp!= HWallsPosRem.end())
                            {
                                HWallsPosRem.erase(temp);
                                HWallsPosRemove.push_back(MyPair);
                                isHwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Row>2)
                            {
                                MyPair.first = Row -1;
                                temp = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row-1][Col] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            //if(Row<9)
                            if(Row< N)
                            {
                                MyPair.first = Row + 1;
                                temp = VWallsPosRem.find(MyPair);
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row+1][Col] = false;
                                }
                            }
                            
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal>alpha)
                            {
                                alpha = ChildVal;
                                if(depth==1)
                                {
                                    bestAction = 2;
                                    bestMove = make_pair(Row,Col);
                                    if(depth ==1)
                                    {
                                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                                    }
                                }
                            }
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                            //MyPair.first = Row;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                    }
                }
                temp_go_up--;
                temp_go_down++;
            }
        }
        
        //row_right col
        if(temp_go_right < M+1)
        {
            int temp_go_up = First_Pos_Row -1, temp_go_down = First_Pos_Row;
            int temph =0;
            while(temph<SHeight)
            {
                //col up check
                temph++;
                if(temp_go_up>1)
                {
                    if(isHwallAllowed[temp_go_up][temp_go_right])
                    {
                        //place Hwall at right, up
                        //check value
                        //undo everything
                        
                        BoardChanges.resize(0);
                        
                        //temp =it;
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_right;
                        int Row = temp_go_up;
                        pair<int, int> MyPair = make_pair(Row, Col);
                        it = HWallsPosRem.find (MyPair);
                        temp = it;
                        //cout<<"H"<<Row<<" "<<Col<<endl;
                        //Change Curr Board
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = false;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-1)*M + Col-2));
                            
                        }
                        if(AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] )
                        {
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-1, (Row-1)*M + Col-1));
                        }
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = true;
                            //it = HWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        
                        if(DoEval)
                        {
                            NodesExplored++;
                            
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(1);
                            HWallsPosRemove[0] = MyPair;
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(0);
                            
                            
                            // 1)The wall itself
                            HWallsPosRem.erase(temp);
                            isHwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = VWallsPosRem.find (MyPair);
                            if(temp!=VWallsPosRem.end())
                            {
                                VWallsPosRem.erase(temp);
                                VWallsPosRemove.push_back(MyPair);
                                isVwallAllowed[Row][Col] = false;
                            }
                            
                            // 3)The wall to left
                            if(Col>2)
                            {
                                MyPair.second = Col -1;
                                temp = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col-1] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            //if(Col<9)
                            if(Col < M)
                            {
                                MyPair.second = Col + 1;
                                temp = HWallsPosRem.find(MyPair);
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col+1] =false;
                                }
                            }
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal>alpha)
                            {
                                alpha = ChildVal;
                                if(depth==1)
                                {
                                    bestAction = 1;
                                    bestMove = make_pair(Row,Col);
                                    if(depth ==1)
                                    {
                                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                                    }
                                }
                            }
                            
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                        
                        
                    }
                    if(isVwallAllowed[temp_go_up][temp_go_right])
                    {
                        //place Vwall at right, up
                        //check value
                        //undo everything
                        
                        
                        //temp = it;
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        
                        int Col = temp_go_right;
                        int Row = temp_go_up;
                        pair<int, int> MyPair = make_pair(Row, Col);
                        it = VWallsPosRem.find (MyPair);
                        temp = it;
                        //ChangeCurrBoard
                        //cout<<"V"<<Row<<" "<<Col<<endl;
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = false;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-2)*M + Col-1));
                            
                        }
                        if(AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1])
                        {
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-1)*M + Col-2,(Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = true;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(0);
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(1);
                            VWallsPosRemove[0]= MyPair;
                            
                            // 1)The wall itself
                            VWallsPosRem.erase(temp);
                            isVwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = HWallsPosRem.find (MyPair);
                            if(temp!= HWallsPosRem.end())
                            {
                                HWallsPosRem.erase(temp);
                                HWallsPosRemove.push_back(MyPair);
                                isHwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Row>2)
                            {
                                MyPair.first = Row -1;
                                temp = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row-1][Col] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            //if(Row<9)
                            if(Row< N)
                            {
                                MyPair.first = Row + 1;
                                temp = VWallsPosRem.find(MyPair);
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row+1][Col] = false;
                                }
                            }
                            
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal>alpha)
                            {
                                alpha = ChildVal;
                                if(depth==1)
                                {
                                    bestAction = 2;
                                    bestMove = make_pair(Row,Col);
                                    if(depth ==1)
                                    {
                                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                                    }
                                }
                            }
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                            //MyPair.first = Row;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                    }
                }
                
                //col down check
                if(temp_go_down<M+1)
                {
                    if(isHwallAllowed[temp_go_down][temp_go_right])
                    {
                        //place Hwall at right, down
                        //check value
                        //undo everything
                        
                        BoardChanges.resize(0);
                        
                        //temp =it;
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col= temp_go_right;
                        int Row = temp_go_down;
                        pair<int, int> MyPair = make_pair(Row, Col);
                        it = HWallsPosRem.find (MyPair);
                        temp= it;
                        //cout<<"H"<<Row<<" "<<Col<<endl;
                        //Change Curr Board
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = false;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-1)*M + Col-2));
                            
                        }
                        if(AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] )
                        {
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-1, (Row-1)*M + Col-1));
                        }
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = true;
                            //it = HWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            
                            NodesExplored++;
                            
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(1);
                            HWallsPosRemove[0] = MyPair;
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(0);
                            
                            
                            // 1)The wall itself
                            HWallsPosRem.erase(temp);
                            isHwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = VWallsPosRem.find (MyPair);
                            if(temp!=VWallsPosRem.end())
                            {
                                VWallsPosRem.erase(temp);
                                VWallsPosRemove.push_back(MyPair);
                                isVwallAllowed[Row][Col] = false;
                            }
                            
                            // 3)The wall to left
                            if(Col>2)
                            {
                                MyPair.second = Col -1;
                                temp = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col-1] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            //if(Col<9)
                            if(Col < M)
                            {
                                MyPair.second = Col + 1;
                                temp = HWallsPosRem.find(MyPair);
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col+1] =false;
                                }
                            }
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal>alpha)
                            {
                                alpha = ChildVal;
                                if(depth==1)
                                {
                                    bestAction = 1;
                                    bestMove = make_pair(Row,Col);
                                    if(depth ==1)
                                    {
                                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                                    }
                                }
                            }
                            
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                            
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                        
                        
                    }
                    if(isVwallAllowed[temp_go_down][temp_go_right])
                    {
                        //place Vwall at right, down
                        //check value
                        //undo everything
                        
                        
                        //temp = it;
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        
                        int Col = temp_go_right;
                        int Row = temp_go_down;
                        pair<int, int> MyPair = make_pair(Row, Col);
                        it = VWallsPosRem.find (MyPair);
                        temp = it;
                        //cout<<"V"<<Row<<" "<<Col<<endl;
                        //ChangeCurrBoard
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = false;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-2)*M + Col-1));
                            
                        }
                        if(AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1])
                        {
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-1)*M + Col-2,(Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = true;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(0);
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(1);
                            VWallsPosRemove[0]= MyPair;
                            
                            // 1)The wall itself
                            VWallsPosRem.erase(temp);
                            isVwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = HWallsPosRem.find (MyPair);
                            if(temp!= HWallsPosRem.end())
                            {
                                HWallsPosRem.erase(temp);
                                HWallsPosRemove.push_back(MyPair);
                                isHwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Row>2)
                            {
                                MyPair.first = Row -1;
                                temp = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row-1][Col] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            //if(Row<9)
                            if(Row< N)
                            {
                                MyPair.first = Row + 1;
                                temp = VWallsPosRem.find(MyPair);
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row+1][Col] = false;
                                }
                            }
                            
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MinVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal>alpha)
                            {
                                alpha = ChildVal;
                                if(depth==1)
                                {
                                    bestAction = 2;
                                    bestMove = make_pair(Row,Col);
                                    if(depth ==1)
                                    {
                                        cout<<"New Best move "<<alpha<<" "<<bestAction<<" "<<bestMove.first<<" "<<bestMove.second<<"\n";
                                    }
                                }
                            }
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                            //MyPair.first = Row;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                        }
                        else
                        {
                            DoEval = true;
                        }
                    }
                }
                temp_go_up--;
                temp_go_down++;
            }
        }
        
        temp_go_left--;
        temp_go_right++;
        
    }
    //ALL walls will be checked above
    //Scope to not check some walls???
    
    
    return alpha;               //best child
}




double MinVal(struct State* initState, double alpha, double beta, int depth, int MaxDepth)       //can make maxDepth global?
{
    if(initState->My_Row == My_Goal && !GameWon)
    {
        return evaluateState(initState, depth);
    }
    depth++;
    vector<pair<int, int> > BoardChanges;
    BoardChanges.resize(0);
    struct State* newState = (struct State*) malloc(sizeof(struct State));;
    newState->My_Row = initState->My_Row;
    newState->My_Col = initState->My_Col;                   //should I exchange my and opp row,col.If yes,needed for both Max & Min
    newState->Opp_Row = initState->Opp_Row;                 //change of ply
    newState->Opp_Col = initState->Opp_Col;
    newState->My_Walls_Rem = initState->My_Walls_Rem;
    newState->Opp_Walls_Rem = initState ->Opp_Walls_Rem;
    
    vector< pair<int, int > > PossMoves;
    PossMoves.resize(0);
    double ChildVal;
    
    //possible nodes: movement * 4
    int DiffRow = initState->Opp_Row - initState->My_Row, DiffCol = initState->Opp_Col - initState ->My_Col;
    if(initState->Opp_Row >0)
    {
        if(DiffRow==1 && DiffCol==0 && AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col][(initState->Opp_Row-1)*M + initState->Opp_Col])
        {
            if(initState->Opp_Row>1)
            {
                if(AdjMatrix[(initState->Opp_Row-1)*M + initState->Opp_Col ][(initState->Opp_Row-2)*M + initState->Opp_Col ])
                {
                    PossMoves.push_back(make_pair(initState->Opp_Row-2, initState->Opp_Col));
                }
            }
            if(initState->Opp_Row<=1 || !AdjMatrix[(initState->Opp_Row-1)*M + initState->Opp_Col ][(initState->Opp_Row-2)*M + initState->Opp_Col ])
            {
                if(initState->Opp_Col>0)
                {
                    if(AdjMatrix[(initState->Opp_Row-1)*M + initState->Opp_Col][(initState->Opp_Row -1)*M + initState->Opp_Col -1])
                    {
                        PossMoves.push_back(make_pair(initState->Opp_Row-1, initState->Opp_Col-1));
                    }
                }
                //if(initState->Opp_Col<8)
                if(initState->Opp_Col<M-1)
                {
                    if(AdjMatrix[(initState->Opp_Row-1)*M + initState->Opp_Col][(initState->Opp_Row -1)*M + initState->Opp_Col+1])
                    {
                        PossMoves.push_back(make_pair(initState->Opp_Row-1, initState->Opp_Col+1));
                    }
                }
            }
        }
        else
        {
            if(AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col ][(initState->Opp_Row-1)*M + initState->Opp_Col])
            {
                PossMoves.push_back(make_pair(initState->Opp_Row-1, initState->Opp_Col));
            }
        }
    }
    
    //if(initState->Opp_Row <8)
    if(initState->Opp_Row <N-1)
    {
        if(DiffRow==-1 && DiffCol==0 && AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col][(initState->Opp_Row+1)*M + initState->Opp_Col])
        {
            //if(initState->Opp_Row<7)
            if(initState->Opp_Row <N-2)
            {
                if(AdjMatrix[(initState->Opp_Row+1)*M + initState->Opp_Col][(initState->Opp_Row+2)*M + initState->Opp_Col])
                {
                    PossMoves.push_back(make_pair(initState->Opp_Row+2, initState->Opp_Col));
                }
            }
            //if(initState->Opp_Row>=7 || !AdjMatrix[(initState->Opp_Row+1)*M + initState->Opp_Col][(initState->Opp_Row+2)*M + initState->Opp_Col])
            if(initState->Opp_Row>=N-2 || !AdjMatrix[(initState->Opp_Row+1)*M + initState->Opp_Col][(initState->Opp_Row+2)*M + initState->Opp_Col])
            {
                if(initState->Opp_Col>0)
                {
                    if(AdjMatrix[(initState->Opp_Row+1)*M + initState->Opp_Col][(initState->Opp_Row +1)*M + initState->Opp_Col -1])
                    {
                        PossMoves.push_back(make_pair(initState->Opp_Row+1, initState->Opp_Col-1));
                    }
                }
                //if(initState->Opp_Col<8)
                if(initState->Opp_Col<M-1)
                {
                    if(AdjMatrix[(initState->Opp_Row+1)*M + initState->Opp_Col][(initState->Opp_Row +1)*M + initState->Opp_Col+1])
                    {
                        PossMoves.push_back(make_pair(initState->Opp_Row+1, initState->Opp_Col+1));
                    }
                }
            }
        }
        else
        {
            if(AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col][(initState->Opp_Row+1)*M + initState->Opp_Col])
            {
                PossMoves.push_back(make_pair(initState->Opp_Row+1, initState->Opp_Col));
            }
        }
    }
    
    //if(initState->Opp_Col<8)
    if(initState->Opp_Col<M-1)
    {
        if(DiffRow==0 && DiffCol==-1 && AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col ][(initState->Opp_Row)*M + initState->Opp_Col+1])
        {
            //if(initState->Opp_Col<7)
            if(initState->Opp_Col<M-2)
            {
                if(AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col +1][(initState->Opp_Row)*M + initState->Opp_Col+2])
                {
                    PossMoves.push_back(make_pair(initState->Opp_Row, initState->Opp_Col+2));
                }
            }
            //if (!AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col +1][(initState->Opp_Row)*M + initState->Opp_Col+2] || initState->Opp_Col>=7)
            if (!AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col +1][(initState->Opp_Row)*M + initState->Opp_Col+2] || initState->Opp_Col>=M-2)
            {
                if(initState->Opp_Row>0)
                {
                    if(AdjMatrix[(initState->Opp_Row-1)*M + initState->Opp_Col+1][(initState->Opp_Row)*M + initState->Opp_Col+1])
                    {
                        PossMoves.push_back(make_pair(initState->Opp_Row-1, initState->Opp_Col+1));
                    }
                }
                if(initState->Opp_Row<N-1)
                    //if(initState->Opp_Row<N-1)
                {
                    if(AdjMatrix[(initState->Opp_Row+1)*M + initState->Opp_Col+1][(initState->Opp_Row)*M + initState->Opp_Col+1])
                    {
                        PossMoves.push_back(make_pair(initState->Opp_Row+1, initState->Opp_Col+1));
                    }
                }
            }
        }
        else
        {
            if(AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col][(initState->Opp_Row)*M + initState->Opp_Col+1])
            {
                PossMoves.push_back(make_pair(initState->Opp_Row, initState->Opp_Col+1));
            }
        }
    }
    if(initState->Opp_Col>0)
    {
        if(DiffRow==0 && DiffCol==1 && AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col][(initState->Opp_Row)*M + initState->Opp_Col -1])
        {
            if(initState->Opp_Col>1)
            {
                if(AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col -2][(initState->Opp_Row)*M + initState->Opp_Col -1])
                {
                    PossMoves.push_back(make_pair(initState->Opp_Row, initState->Opp_Col-2));
                }
            }
            if(initState->Opp_Col <=1 || !AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col -2][(initState->Opp_Row)*M + initState->Opp_Col -1])
            {
                if(initState->Opp_Row>0)
                {
                    if(AdjMatrix[(initState->Opp_Row-1)*M + initState->Opp_Col -1][(initState->Opp_Row)*M + initState->Opp_Col -1])
                    {
                        PossMoves.push_back(make_pair(initState->Opp_Row-1, initState->Opp_Col-1));
                    }
                }
                //if(initState->Opp_Row<8)
                if(initState->Opp_Row<N-1)
                {
                    if(AdjMatrix[(initState->Opp_Row+1)*M + initState->Opp_Col -1][(initState->Opp_Row)*M + initState->Opp_Col -1])
                    {
                        PossMoves.push_back(make_pair(initState->Opp_Row+1, initState->Opp_Col-1));
                    }
                }
            }
        }
        else
        {
            if(AdjMatrix[(initState->Opp_Row)*M + initState->Opp_Col][(initState->Opp_Row)*M + initState->Opp_Col -1])
            {
                PossMoves.push_back(make_pair(initState->Opp_Row, initState->Opp_Col-1));
            }
        }
    }
    
    
    if(initState->Opp_Row != Opp_Goal)
    {
        NodesExplored++;
        for(int Vi=0;Vi<PossMoves.size();Vi++)
        {
            newState->Opp_Row = PossMoves[Vi].first;
            newState->Opp_Col = PossMoves[Vi].second;
            if (depth >= MaxDepth)
            {
                ChildVal = evaluateState(newState,depth);
            }
            else
            {
                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
            }
            if(ChildVal<beta)
            {
                beta = ChildVal;
            }
            
            
            if(alpha>=beta)
            {
                //keep track of best moves at each level??(Need move at depth 1)
                return ChildVal;
            }
            
        }
        
    }
    else
    {
        NodesExplored++;
        //pass
        if (depth >= MaxDepth)
        {
            ChildVal = evaluateState(initState,depth);
        }
        else
        {
            ChildVal = MaxVal(initState, alpha, beta, depth, MaxDepth);
        }
        if(ChildVal<beta)
        {
            beta = ChildVal;
        }
        if(alpha>=beta)
        {
            //keep track of best moves at each level??(Need move at depth 1)
            return ChildVal;
        }
        
    }
    
    newState->Opp_Row = initState->Opp_Row;
    newState->Opp_Col = initState->Opp_Col;
    //only change Opp_Row etc or Opp_row etc
    if(newState->Opp_Walls_Rem<=0)
    {
        return beta;
    }
    
    newState->Opp_Walls_Rem--;
    
    
    
    bool DoEval = true;
    int First_Pos_Row = initState->My_Row + 2;
    int First_Pos_Col = initState->My_Col + 2;
    if(First_Pos_Row> N)
    {
        First_Pos_Row --;
    }
    if(First_Pos_Col>M)
    {
        First_Pos_Col --;
    }
    int SWidth = 3 , SHeight = 4;
    int tempw = 0;
    int temp_go_left = First_Pos_Col-1 , temp_go_right  = First_Pos_Col ;
    while(tempw<SWidth)
    {
        tempw++;
        //row_left col
        if(temp_go_left>1)
        {
            int temp_go_up = First_Pos_Row -1, temp_go_down = First_Pos_Row;
            int temph=0;
            while(temph<SHeight)
            {
                temph ++;
                //col up check
                if(temp_go_up>1)
                {
                    if(isHwallAllowed[temp_go_up][temp_go_left])
                    {
                        //place Hwall at left, up
                        //check value
                        //undo everything
                        
                        set<pair<int, int> >::iterator it, temp;
                        
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_left;
                        int Row = temp_go_up;
                        pair<int, int > MyPair = make_pair(Row, Col);
                        it = HWallsPosRem.find (MyPair);
                        temp =it;
                        //ChangeCurrBoard
                        //cout<<"Min H"<<Row<<" "<<Col<<endl;
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = false;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-1)*M + Col-2));
                        }
                        if(AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] )
                        {
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-1, (Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = true;
                            //it = HWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(1);
                            HWallsPosRemove[0]=MyPair;
                            
                            vector< pair<int, int> > VWallsPosRemove ;
                            VWallsPosRemove.resize(0);
                            
                            // 1)The wall itself
                            HWallsPosRem.erase(temp);
                            isHwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = VWallsPosRem.find (MyPair);
                            if(temp !=VWallsPosRem.end())
                            {
                                VWallsPosRem.erase(temp);
                                VWallsPosRemove.push_back(MyPair);
                                isVwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Col>2)
                            {
                                MyPair.second = Col -1;
                                temp = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col-1]= false;
                                }
                            }
                            
                            // 4)The wall to the right
                            if(Col<M)
                            {
                                MyPair.second = Col + 1;
                                temp = HWallsPosRem.find(MyPair);
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col+1]=false;
                                }
                            }
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
                                if(ChildVal<beta)
                                {
                                    beta = ChildVal;
                                }
                            }
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                                
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return beta;                        //beta = childval
                            }
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                        
                    }
                    if(isVwallAllowed[temp_go_up][temp_go_left])
                    {
                        //place Vwall at left, up
                        //check value
                        //undo everything
                        
                        set<pair<int, int> >::iterator it, temp;
                        
                        //temp = it;
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_left;
                        int Row = temp_go_up;
                        pair<int, int> MyPair = make_pair(Row,Col);
                        it = VWallsPosRem.find(MyPair);
                        temp = it;
                        //cout<<"Min V"<<Row<<" "<<Col<<endl;
                        //ChangeCurrBoard
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = false;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-2)*M + Col-1));
                            
                        }
                        if(AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1])
                        {
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-1)*M + Col-2,(Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = true;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(0);
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(1);
                            VWallsPosRemove[0]= MyPair;
                            
                            // 1)The wall itself
                            VWallsPosRem.erase(temp);
                            isVwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = HWallsPosRem.find (MyPair);
                            if(temp!=HWallsPosRem.end())
                            {
                                HWallsPosRem.erase(temp);
                                HWallsPosRemove.push_back(MyPair);
                                isHwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Row>2)
                            {
                                MyPair.first = Row -1;
                                temp = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row-1][Col] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            if(Row<N)
                            {
                                MyPair.first = Row + 1;
                                temp = VWallsPosRem.find(MyPair);
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row+1][Col] = false;
                                }
                            }
                            
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal<beta)
                            {
                                beta = ChildVal;
                            }
                            
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                        
                    }
                }
                
                //col down check
                if(temp_go_down<N+1)
                {
                    if(isHwallAllowed[temp_go_down][temp_go_left])
                    {
                        //place Hwall at left, down
                        //check value
                        //undo everything
                        set<pair<int, int> >::iterator it, temp;
                        
                        
                        BoardChanges.resize(0);
                        
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_left;
                        int Row = temp_go_down;
                        pair<int, int > MyPair = make_pair(Row, Col);
                        it = HWallsPosRem.find (MyPair);
                        temp =it;
                        //cout<<"Min H"<<Row<<" "<<Col<<endl;
                        //ChangeCurrBoard
                        
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = false;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-1)*M + Col-2));
                        }
                        if(AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] )
                        {
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-1, (Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = true;
                            //it = HWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval =false;
                        }
                        if(DoEval)
                        {
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(1);
                            HWallsPosRemove[0]=MyPair;
                            
                            vector< pair<int, int> > VWallsPosRemove ;
                            VWallsPosRemove.resize(0);
                            
                            // 1)The wall itself
                            HWallsPosRem.erase(temp);
                            isHwallAllowed[Row][Col] =false;
                            
                            // 2)The corr. vert. wall
                            temp = VWallsPosRem.find (MyPair);
                            if(temp !=VWallsPosRem.end())
                            {
                                VWallsPosRem.erase(temp);
                                VWallsPosRemove.push_back(MyPair);
                                isVwallAllowed[Row][Col]= false;
                            }
                            // 3)The wall to left
                            if(Col>2)
                            {
                                MyPair.second = Col -1;
                                temp = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col-1] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            if(Col<M)
                            {
                                MyPair.second = Col + 1;
                                temp = HWallsPosRem.find(MyPair);
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col+1] = false;
                                }
                            }
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
                                if(ChildVal<beta)
                                {
                                    beta = ChildVal;
                                }
                            }
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                                
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return beta;                        //beta = childval
                            }
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                        
                    }
                    if(isVwallAllowed[temp_go_down][temp_go_left])
                    {
                        //place Vwall at left, down
                        //check value
                        //undo everything
                        set<pair<int, int> >::iterator it, temp;
                        
                        //temp = it;
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_left;
                        int Row = temp_go_down;
                        pair<int, int> MyPair = make_pair(Row,Col);
                        it = VWallsPosRem.find(MyPair);
                        temp = it;
                        //cout<<"Min V"<<Row<<" "<<Col<<endl;
                        //ChangeCurrBoard
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = false;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-2)*M + Col-1));
                            
                        }
                        if(AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1])
                        {
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-1)*M + Col-2,(Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = true;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(0);
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(1);
                            VWallsPosRemove[0]= MyPair;
                            
                            // 1)The wall itself
                            VWallsPosRem.erase(temp);
                            isVwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = HWallsPosRem.find (MyPair);
                            if(temp!=HWallsPosRem.end())
                            {
                                HWallsPosRem.erase(temp);
                                HWallsPosRemove.push_back(MyPair);
                                isHwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Row>2)
                            {
                                MyPair.first = Row -1;
                                temp = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row-1][Col] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            if(Row<N)
                            {
                                MyPair.first = Row + 1;
                                temp = VWallsPosRem.find(MyPair);
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row+1][Col] = false;
                                }
                            }
                            
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal<beta)
                            {
                                beta = ChildVal;
                            }
                            
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                            
                        }
                        else
                        {
                            DoEval = true;
                        }
                    }
                }
                temp_go_up--;
                temp_go_down++;
            }
        }
        
        //row_right col
        if(temp_go_right < M+1)
        {
            int temp_go_up = First_Pos_Row -1, temp_go_down = First_Pos_Row;
            int temph =0;
            while(temph<SHeight)
            {
                temph++;
                //col up check
                if(temp_go_up>1)
                {
                    if(isHwallAllowed[temp_go_up][temp_go_right])
                    {
                        //place Hwall at right, up
                        //check value
                        //undo everything
                        set<pair<int, int> >::iterator it, temp;
                        
                        
                        BoardChanges.resize(0);
                        
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_right;
                        int Row = temp_go_up;
                        pair<int, int > MyPair = make_pair(Row, Col);
                        it = HWallsPosRem.find (MyPair);
                        temp =it;
                        //ChangeCurrBoard
                        //cout<<"Min H"<<Row<<" "<<Col<<endl;
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = false;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-1)*M + Col-2));
                        }
                        if(AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] )
                        {
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-1, (Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = true;
                            //it = HWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(1);
                            HWallsPosRemove[0]=MyPair;
                            
                            vector< pair<int, int> > VWallsPosRemove ;
                            VWallsPosRemove.resize(0);
                            
                            // 1)The wall itself
                            HWallsPosRem.erase(temp);
                            isHwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = VWallsPosRem.find (MyPair);
                            if(temp !=VWallsPosRem.end())
                            {
                                VWallsPosRem.erase(temp);
                                VWallsPosRemove.push_back(MyPair);
                                isVwallAllowed[Row][Col]=false;
                            }
                            // 3)The wall to left
                            if(Col>2)
                            {
                                MyPair.second = Col -1;
                                temp = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col-1]=false;
                                }
                            }
                            
                            // 4)The wall to the right
                            if(Col<M)
                            {
                                MyPair.second = Col + 1;
                                temp = HWallsPosRem.find(MyPair);
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col+1] =false;
                                }
                            }
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
                                if(ChildVal<beta)
                                {
                                    beta = ChildVal;
                                }
                            }
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                                
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return beta;                        //beta = childval
                            }
                            
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                    }
                    if(isVwallAllowed[temp_go_up][temp_go_right])
                    {
                        //place Vwall at right, up
                        //check value
                        //undo everything
                        set<pair<int, int> >::iterator it, temp;
                        
                        //temp = it;
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_right;
                        int Row = temp_go_up;
                        pair<int, int> MyPair = make_pair(Row,Col);
                        it = VWallsPosRem.find(MyPair);
                        temp = it;
                        //cout<<"Min V"<<Row<<" "<<Col<<endl;
                        //ChangeCurrBoard
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = false;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-2)*M + Col-1));
                            
                        }
                        if(AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1])
                        {
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-1)*M + Col-2,(Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = true;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(0);
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(1);
                            VWallsPosRemove[0]= MyPair;
                            
                            // 1)The wall itself
                            VWallsPosRem.erase(temp);
                            isVwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = HWallsPosRem.find (MyPair);
                            if(temp!=HWallsPosRem.end())
                            {
                                HWallsPosRem.erase(temp);
                                HWallsPosRemove.push_back(MyPair);
                                isHwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Row>2)
                            {
                                MyPair.first = Row -1;
                                temp = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row-1][Col] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            if(Row<N)
                            {
                                MyPair.first = Row + 1;
                                temp = VWallsPosRem.find(MyPair);
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row+1][Col] = false;
                                }
                            }
                            
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal<beta)
                            {
                                beta = ChildVal;
                            }
                            
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                    }
                }
                
                //col down check
                if(temp_go_down<N+1)
                {
                    if(isHwallAllowed[temp_go_down][temp_go_right])
                    {
                        //place Hwall at right, down
                        //check value
                        //undo everything
                        set<pair<int, int> >::iterator it, temp;
                        
                        
                        BoardChanges.resize(0);
                        
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_right;
                        int Row = temp_go_down;
                        pair<int, int > MyPair = make_pair(Row, Col);
                        it = HWallsPosRem.find (MyPair);
                        temp =it;
                        //ChangeCurrBoard
                        //cout<<"Min H"<<Row<<" "<<Col<<endl;
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = false;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-1)*M + Col-2));
                        }
                        if(AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] )
                        {
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-1, (Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-1)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-2)*M + Col-1] = true;
                            //it = HWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(1);
                            HWallsPosRemove[0]=MyPair;
                            
                            vector< pair<int, int> > VWallsPosRemove ;
                            VWallsPosRemove.resize(0);
                            
                            // 1)The wall itself
                            HWallsPosRem.erase(temp);
                            isHwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = VWallsPosRem.find (MyPair);
                            if(temp !=VWallsPosRem.end())
                            {
                                VWallsPosRem.erase(temp);
                                VWallsPosRemove.push_back(MyPair);
                                isVwallAllowed[Row][Col]=false;
                            }
                            // 3)The wall to left
                            if(Col>2)
                            {
                                MyPair.second = Col -1;
                                temp = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col-1]=false;
                                }
                            }
                            
                            // 4)The wall to the right
                            if(Col<M)
                            {
                                MyPair.second = Col + 1;
                                temp = HWallsPosRem.find(MyPair);
                                if(temp!=HWallsPosRem.end())
                                {
                                    HWallsPosRem.erase(temp);
                                    HWallsPosRemove.push_back(MyPair);
                                    isHwallAllowed[Row][Col+1]= false;
                                }
                            }
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
                                if(ChildVal<beta)
                                {
                                    beta = ChildVal;
                                }
                            }
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                                
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return beta;                        //beta = childval
                            }
                            
                        }
                        else
                        {
                            DoEval = true;
                        }
                        
                        
                    }
                    if(isVwallAllowed[temp_go_down][temp_go_right])
                    {
                        //place Vwall at right, down
                        //check value
                        //undo everything
                        set<pair<int, int> >::iterator it, temp;
                        
                        //temp = it;
                        BoardChanges.resize(0);
                        //pair<int, int> MyPair = *temp;
                        //int Row = MyPair.first;
                        //int Col = MyPair.second;
                        int Col = temp_go_right;
                        int Row = temp_go_down;
                        pair<int, int> MyPair = make_pair(Row,Col);
                        it = VWallsPosRem.find(MyPair);
                        temp = it;
                        //cout<<"Min V"<<Row<<" "<<Col<<endl;
                        //ChangeCurrBoard
                        if(AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1])
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = false;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-2)*M + Col-2, (Row-2)*M + Col-1));
                            
                        }
                        if(AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1])
                        {
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = false;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = false;
                            BoardChanges.push_back(make_pair((Row-1)*M + Col-2,(Row-1)*M + Col-1));
                        }
                        
                        // Check if wall does not block path
                        bool Am_I_Conn = connected(newState->My_Row, newState->My_Col, My_Goal);
                        bool Is_Opp_Conn = connected(newState->Opp_Row, newState->Opp_Col, Opp_Goal);
                        
                        if(!Am_I_Conn || !Is_Opp_Conn)
                        {
                            AdjMatrix[(Row-2)*M + Col-2][(Row-2)*M + Col-1] = true;
                            AdjMatrix[(Row-2)*M + Col-1][(Row-2)*M + Col-2] = true;
                            AdjMatrix[(Row-1)*M + Col-2][(Row-1)*M + Col-1] = true;
                            AdjMatrix[(Row-1)*M + Col-1][(Row-1)*M + Col-2] = true;
                            //it = VWallsPosRem.find(MyPair);
                            //it++;
                            //continue;
                            DoEval = false;
                        }
                        if(DoEval)
                        {
                            
                            NodesExplored++;
                            //RemoveTheWallsFromGlobal
                            //Add_RemovedWallsTo newState's var
                            vector< pair<int, int> > HWallsPosRemove;
                            HWallsPosRemove.resize(0);
                            vector< pair<int, int> > VWallsPosRemove;
                            VWallsPosRemove.resize(1);
                            VWallsPosRemove[0]= MyPair;
                            
                            // 1)The wall itself
                            VWallsPosRem.erase(temp);
                            isVwallAllowed[Row][Col] = false;
                            
                            // 2)The corr. vert. wall
                            temp = HWallsPosRem.find (MyPair);
                            if(temp!=HWallsPosRem.end())
                            {
                                HWallsPosRem.erase(temp);
                                HWallsPosRemove.push_back(MyPair);
                                isHwallAllowed[Row][Col] = false;
                            }
                            // 3)The wall to left
                            if(Row>2)
                            {
                                MyPair.first = Row -1;
                                temp = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row-1][Col] = false;
                                }
                            }
                            
                            // 4)The wall to the right
                            if(Row<N)
                            {
                                MyPair.first = Row + 1;
                                temp = VWallsPosRem.find(MyPair);
                                if(temp!=VWallsPosRem.end())
                                {
                                    VWallsPosRem.erase(temp);
                                    VWallsPosRemove.push_back(MyPair);
                                    isVwallAllowed[Row+1][Col] = false;
                                }
                            }
                            
                            if (depth >= MaxDepth)
                            {
                                ChildVal = evaluateState(newState,depth);
                            }
                            else
                            {
                                ChildVal = MaxVal(newState, alpha, beta, depth, MaxDepth);
                            }
                            if(ChildVal<beta)
                            {
                                beta = ChildVal;
                            }
                            
                            //restore walls
                            for(int Hi=0;Hi<HWallsPosRemove.size();Hi++)
                            {
                                HWallsPosRem.insert(HWallsPosRemove[Hi]);
                                int temp1 = HWallsPosRemove[Hi].first;
                                int temp2 = HWallsPosRemove[Hi].second;
                                isHwallAllowed[temp1][temp2] = true;
                            }
                            for(int Vi=0;Vi<VWallsPosRemove.size();Vi++)
                            {
                                VWallsPosRem.insert(VWallsPosRemove[Vi]);
                                int temp1 = VWallsPosRemove[Vi].first;
                                int temp2 = VWallsPosRemove[Vi].second;
                                isVwallAllowed[temp1][temp2] = true;
                            }
                            for(int Bi =0; Bi<BoardChanges.size();Bi++)
                            {
                                AdjMatrix[BoardChanges[Bi].first][BoardChanges[Bi].second] = true;
                                AdjMatrix[BoardChanges[Bi].second][BoardChanges[Bi].first] = true;
                            }
                            //check for prune
                            if(alpha>=beta)
                            {
                                //keep track of best moves at each level??(Need move at depth 1)
                                return ChildVal;
                            }
                        }
                        else
                        {
                            DoEval = true;
                        }
                    }
                }
                temp_go_up--;
                temp_go_down++;
            }
        }
        temp_go_left--;
        temp_go_right++;
        
    }
    return beta;               //best child
    
}



void MiniMaxSearch(struct State* initState)
{
    bool DoMiniMax = true;
    int DepthToIterateTo = 4;
    if(TL<10)
    {
        DepthToIterateTo = 3;
    }
    
    if(initState->Opp_Walls_Rem ==K)
    {
        if(player==1)
        {
            if(initState->My_Row< (M/2 -1))
            {
                DoMiniMax = false;
                bestAction = 0;
                bestMove = make_pair(initState->My_Row + 1, initState->My_Col);
            }
            else if(initState->My_Row ==(M/2 -1))
            {
                if(initState->My_Walls_Rem == K)
                {
                    DoMiniMax = false;
                    bestAction = 1;
                    bestMove = make_pair (M/2,M/2 +1);
                }
            }
        }
        else
        {
            if(initState->My_Row> N/2)
            {
                DoMiniMax = false;
                bestAction = 0;
                bestMove = make_pair(initState->My_Row - 1, initState->My_Col);
                if(initState->My_Row -1 == initState->Opp_Row && initState->My_Col==initState->Opp_Col)
                {
                    bestMove.first --;
                }
            }
        }
    }
    
    if(GameWon && initState->Opp_Row == Opp_Goal)
    {
        bestAction = 0;
        bestMove = bfs(initState);
        return;
    }
    
    int MaxDepth = 1;
    while(MaxDepth< DepthToIterateTo && DoMiniMax)
    {
        NodesExplored=0;
        double val = MaxVal(initState, -1*numeric_limits<double>::max(), numeric_limits<double>::max(), 0, MaxDepth);
        cout<<"Nodes Explored at depth "<<MaxDepth<<" = "<<NodesExplored<<endl;
        if(MaxDepth ==2)
        {
            if( TL<20)
            {
                if(NodesExplored > 200)
                {
                    DepthToIterateTo = 3;
                }
            }
            else if(TL<30)
            {
                if(NodesExplored > 400)
                {
                    DepthToIterateTo = 3;
                }
            }
        }
        MaxDepth++;
    }
    
    if(GameWon && initState->My_Row == My_Goal)
    {
        cout<<"Best Action \n";
        if(bestAction ==0)
        {
            bestMove = make_pair(0,0);
        }
    }
    
}


int main(int argc, char *argv[])
{
    //...
    
    //
    //srand (time(NULL));
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1025];
    struct sockaddr_in serv_addr;
    
    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <port no> \n",argv[0]);
        return 1;
    }
    
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }
    
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
    
    //...
    
    cout<<"Quoridor will start..."<<endl;
    
    //...
    
    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d %d %d %d %d", &player, &N, &M, &K, &time_left);
    
    //...
    srand(time(0));
    //.
    /*
     N = 9;
     M = 9;
     K = 10;
     time_left = 10000;
     TL = 100;
     player = 2;
     //.
     */
    
    GameWon = false;
    cout<<"Player "<<player<<endl;
    cout<<"Time "<<time_left<<endl;
    cout<<"Board size "<<N<<"x"<<M<<" :"<<K<<endl;
    int om,oro,oc;
    int m,r,c;
	int d=3;
    char s[100];
	int x=1;
    int NM = N*M;
    //cin>>TL;
    //Complete H & V Wall Position Remaining sets
    pair<int, int> MyPair= make_pair(2,2);
    for(int i=2;i<=N;i++)
    {
        for(int j=2;j<=M;j++)
        {
            MyPair.first = i;
            MyPair.second = j;
            HWallsPosRem.insert(MyPair);
            VWallsPosRem.insert(MyPair);
        }
    }
    
    
    //initialise the adjacency matrix to be one if an edge exists currently between two nodes(move is poss)
    AdjMatrix.resize(NM,vector<bool> (NM,false));
    isHwallAllowed.resize(N+1,vector<bool> (M+1,true));
    isVwallAllowed.resize(N+1,vector<bool> (M+1,true));         //First 2 rows and cols and empty
    //Horizontal Movement
    for (int i=0;i<N;i++)
    {
        for(int j=0;j<M-1;j++)
        {
            AdjMatrix[i*M + j][ i * M + j+1] = true;
            AdjMatrix[i*M + j+1][ i * M + j] = true;
        }
    }
    //Vertical Movement
    for (int i=0;i<M;i++)
    {
        for(int j=0;j<N-1;j++)
        {
            AdjMatrix[i + j*M][i + j*M + M] =true;
            AdjMatrix[i + j*M + M][i + j*M] =true;
            
        }
    }
    //HERE. DEFINE HERE
    
    int opponent_row, opponent_col = M/2 +1;
    int my_row, my_col = M/2 +1;
    
    struct State *initState = (struct State*) malloc(sizeof(struct State));
    initState->My_Walls_Rem = K;
    initState->Opp_Walls_Rem = K;
    if(player == 1)
    {
        
        my_row = 1;
        opponent_row = N;
        My_Goal = N-1;
        Opp_Goal = 0;
        
        initState->My_Row = my_row-1;
        initState->My_Col = my_col-1;
        initState->Opp_Row = opponent_row-1;
        initState->Opp_Col = opponent_col-1;
        //.
        //cin>>m>>r>>c;
        //.
        //...
        
        MiniMaxSearch(initState);
        m = bestAction;
        r = bestMove.first ;
        c = bestMove.second ;
        //cout<<"My Move "<<m<<" "<<r<<" "<<c<<endl;
        //...
        
        if(m==0)
        {
            r++;
            c++;
        }
        //cout<<"given ";
        if(m == 0)
        {
            my_row = r ;
            my_col = c ;
            
            initState->My_Row = r - 1;
            initState->My_Col = c - 1;
        }
        else if(m == 1)
        {
            initState->My_Walls_Rem --;
            //ChangeCurrBoard
            AdjMatrix[(r-2)*M + c-2][(r-1)*M + c-2] = false;
            AdjMatrix[(r-1)*M + c-2][(r-2)*M + c-2] = false;
            AdjMatrix[(r-2)*M + c-1][(r-1)*M + c-1] = false;
            AdjMatrix[(r-1)*M + c-1][(r-2)*M + c-1] = false;
            
            //RemoveTheWallsFromGlobal
            //Add_RemovedWallsTo newState's var
            pair <int, int> MyPair = make_pair(r, c);
            set<pair<int, int> >::iterator it = HWallsPosRem.find (MyPair);
            
            // 1)The wall itself
            HWallsPosRem.erase(it);
            isHwallAllowed[MyPair.first][MyPair.second] = false;
            // 2)The corr. vert. wall
            it = VWallsPosRem.find (MyPair);
            if(it!=VWallsPosRem.end())
            {
                VWallsPosRem.erase(it);
                isVwallAllowed[MyPair.first][MyPair.second] = false;
            }
            // 3)The wall to left
            if(c>2)
            {
                MyPair.second = c -1;
                it = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                if(it!=HWallsPosRem.end())
                {
                    HWallsPosRem.erase(it);
                    isHwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
            // 4)The wall to the right
            if(c<M)
            {
                MyPair.second = c + 1;
                it = HWallsPosRem.find(MyPair);
                if(it!=HWallsPosRem.end())
                {
                    HWallsPosRem.erase(it);
                    isHwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
        }
        else if(m == 2)
        {
            initState->My_Walls_Rem --;
            //update board
            AdjMatrix[(r-2)*M + c-2][(r-2)*M + c-1] = false;
            AdjMatrix[(r-2)*M + c-1][(r-2)*M + c-2] = false;
            AdjMatrix[(r-1)*M + c-2][(r-1)*M + c-1] = false;
            AdjMatrix[(r-1)*M + c-1][(r-1)*M + c-2] = false;
            
            // 1)The wall itself
            pair <int, int> MyPair = make_pair(r, c);
            set<pair<int, int> >::iterator it = VWallsPosRem.find (MyPair);
            VWallsPosRem.erase(it);
            isVwallAllowed[MyPair.first][MyPair.second] = false;
            // 2)The corr. vert. wall
            it = HWallsPosRem.find (MyPair);
            if(it!=HWallsPosRem.end())
            {
                HWallsPosRem.erase(it);
                isHwallAllowed[MyPair.first][MyPair.second] = false;
            }
            // 3)The wall to left
            if(r>2)
            {
                MyPair.first = r -1;
                it = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                if(it!=VWallsPosRem.end())
                {
                    VWallsPosRem.erase(it);
                    isVwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
            // 4)The wall to the right
            if(r<N)
            {
                MyPair.first = r + 1;
                it = VWallsPosRem.find(MyPair);
                if(it!=VWallsPosRem.end())
                {
                    VWallsPosRem.erase(it);
                    isVwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
        }
        //
        if(My_Goal == initState->My_Row)
        {
            GameWon = true;
        }
        if(Opp_Goal == initState->Opp_Row)
        {
            GameWon = true;
        }
        
        
        //
        
        //...
        
        snprintf(sendBuff, sizeof(sendBuff), "%d %d %d", m, r , c);
        write(sockfd, sendBuff, strlen(sendBuff));
        
        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        recvBuff[n] = 0;
        sscanf(recvBuff, "%f %d", &TL, &d);
        cout<<TL<<" "<<d<<endl;
        if(d==1)
        {
            cout<<"You win!! Yayee!! :D ";
            x=0;
        }
        else if(d==2)
        {
            cout<<"Loser :P ";
            x=0;
        }
        
        
        //...
        
        
    }
    else
    {
        my_row = N;
        opponent_row = 1;
        Opp_Goal = N-1;
        My_Goal = 0;
        
        initState->My_Row = my_row-1;
        initState->My_Col = my_col-1;
        initState->Opp_Row = opponent_row-1;
        initState->Opp_Col = opponent_col-1;
        
        
    }
    
    
    
    while(x)
    {
        //Implement the game
        //.
        /*
         cout<<"\nEnter opponent m,r,o  \n";
         cin>>om>>oro>>oc;
         //.
         */
        //...
        
        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        recvBuff[n] = 0;
        sscanf(recvBuff, "%d %d %d %d", &om,&oro,&oc,&d);
        cout << om<<" "<<oro<<" "<<oc << " "<<d<<endl;
        if(d==1)
        {
            cout<<"You win!! Yayee!! :D ";
            break;
        }
        else if(d==2)
        {
            cout<<"Loser :P ";
            break;
        }
        //...
        
        if(GameWon)
        {
            cout<<"Game is Won by ";
            if(initState->My_Row == My_Goal)
            {
                cout<<"Me \n";
            }
            else
            {
                cout<<"Opp \n";
            }
        }
        //cout<<"taken ";
        if(om == 0 && initState->Opp_Row != Opp_Goal)
        {
            opponent_row = oro;
            opponent_col = oc;
            initState->Opp_Row = oro - 1;
            initState->Opp_Col = oc - 1;
            
            
        }
        else if(om == 1)
        {
            initState->Opp_Walls_Rem --;
            //ChangeCurrBoard
            AdjMatrix[(oro-2)*M + oc-2][(oro-1)*M + oc-2] = false;
            AdjMatrix[(oro-1)*M + oc-2][(oro-2)*M + oc-2] = false;
            AdjMatrix[(oro-2)*M + oc-1][(oro-1)*M + oc-1] = false;
            AdjMatrix[(oro-1)*M + oc-1][(oro-2)*M + oc-1] = false;
            
            //RemoveTheWallsFromGlobal
            //Add_RemovedWallsTo newState's var
            pair <int, int> MyPair = make_pair(oro, oc);
            set<pair<int, int> >::iterator it = HWallsPosRem.find (MyPair);
            
            // 1)The wall itself
            HWallsPosRem.erase(it);
            isHwallAllowed[MyPair.first][MyPair.second] = false;
            // 2)The corr. vert. wall
            it = VWallsPosRem.find (MyPair);
            if(it!=VWallsPosRem.end())
            {
                VWallsPosRem.erase(it);
                isVwallAllowed[MyPair.first][MyPair.second] = false;
            }
            // 3)The wall to left
            if(oc>2)
            {
                MyPair.second = oc -1;
                it = HWallsPosRem.find (MyPair);
                if(it!=HWallsPosRem.end())
                {
                    HWallsPosRem.erase(it);
                    isHwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
            // 4)The wall to the right
            if(oc<M)
            {
                MyPair.second = oc + 1;
                it = HWallsPosRem.find(MyPair);
                if(it!=HWallsPosRem.end())
                {
                    HWallsPosRem.erase(it);
                    isHwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
        }
        else if(om == 2)
        {
            initState->Opp_Walls_Rem --;
            //update board
            AdjMatrix[(oro-2)*M + oc-2][(oro-2)*M + oc-1] = false;
            AdjMatrix[(oro-2)*M + oc-1][(oro-2)*M + oc-2] = false;
            AdjMatrix[(oro-1)*M + oc-2][(oro-1)*M + oc-1] = false;
            AdjMatrix[(oro-1)*M + oc-1][(oro-1)*M + oc-2] = false;
            
            // 1)The wall itself
            pair <int, int> MyPair = make_pair(oro, oc);
            set<pair<int, int> >::iterator it = VWallsPosRem.find (MyPair);
            VWallsPosRem.erase(it);
            isVwallAllowed[MyPair.first][MyPair.second] = false;
            // 2)The corr. vert. wall
            it = HWallsPosRem.find (MyPair);
            if (it!=HWallsPosRem.end())
            {
                HWallsPosRem.erase(it);
                isHwallAllowed[MyPair.first][MyPair.second] = false;
            }
            
            // 3)The wall to left
            if(oro>2)
            {
                MyPair.first = oro -1;
                it = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                if(it!= VWallsPosRem.end())
                {
                    VWallsPosRem.erase(it);
                    isVwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
            // 4)The wall to the right
            if(oro<N)
            {
                MyPair.first = oro + 1;
                it = VWallsPosRem.find(MyPair);
                if(it!=VWallsPosRem.end())
                {
                    VWallsPosRem.erase(it);
                    isVwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
        }
        if(My_Goal == initState->My_Row)
        {
            GameWon = true;
        }
        if(Opp_Goal == initState->Opp_Row)
        {
            GameWon = true;
        }
        
        
        //update completed
        
        //cout<<"Your move! ";
        //cin>>m>>r>>c;
        MiniMaxSearch(initState);
        m = bestAction;
        r = bestMove.first ;
        c = bestMove.second ;
        cout<<"My Move "<<m<<" "<<r<<" "<<c<<endl;
        
        if(m==0)
        {
            r++;
            c++;
        }
        
        
        //...
        
        memset(sendBuff, '0', sizeof(sendBuff));
        string temp;
        //cin>>m>>r>>c;
        snprintf(sendBuff, sizeof(sendBuff), "%d %d %d", m, r , c);
        write(sockfd, sendBuff, strlen(sendBuff));
        
        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        recvBuff[n] = 0;
        sscanf(recvBuff, "%f %d", &TL, &d);//d=3 indicates game continues.. d=2 indicates lost game, d=1 means game won.
        cout<<TL<<" "<<d<<endl;
        if(d==1)
        {
            cout<<"You win!! Yayee!! :D ";
            break;
        }
        else if(d==2)
        {
            cout<<"Loser :P ";
            break;
        }
        
        //...
        
        //cout<<"given ";
        if(m == 0)
        {
            my_row = r ;
            my_col = c ;
            
            initState->My_Row = r - 1;
            initState->My_Col = c - 1;
        }
        else if(m == 1)
        {
            initState->My_Walls_Rem --;
            //ChangeCurrBoard
            AdjMatrix[(r-2)*M + c-2][(r-1)*M + c-2] = false;
            AdjMatrix[(r-1)*M + c-2][(r-2)*M + c-2] = false;
            AdjMatrix[(r-2)*M + c-1][(r-1)*M + c-1] = false;
            AdjMatrix[(r-1)*M + c-1][(r-2)*M + c-1] = false;
            
            //RemoveTheWallsFromGlobal
            //Add_RemovedWallsTo newState's var
            pair <int, int> MyPair = make_pair(r, c);
            set<pair<int, int> >::iterator it = HWallsPosRem.find (MyPair);
            
            // 1)The wall itself
            HWallsPosRem.erase(it);
            isHwallAllowed[MyPair.first][MyPair.second] = false;
            // 2)The corr. vert. wall
            it = VWallsPosRem.find (MyPair);
            if(it!=VWallsPosRem.end())
            {
                VWallsPosRem.erase(it);
                isVwallAllowed[MyPair.first][MyPair.second] = false;
            }
            // 3)The wall to left
            if(c>2)
            {
                MyPair.second = c -1;
                it = HWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                if(it!=HWallsPosRem.end())
                {
                    HWallsPosRem.erase(it);
                    isHwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
            // 4)The wall to the right
            if(c<M)
            {
                MyPair.second = c + 1;
                it = HWallsPosRem.find(MyPair);
                if(it!=HWallsPosRem.end())
                {
                    HWallsPosRem.erase(it);
                    isHwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
        }
        else if(m == 2)
        {
            initState->My_Walls_Rem --;
            //update board
            AdjMatrix[(r-2)*M + c-2][(r-2)*M + c-1] = false;
            AdjMatrix[(r-2)*M + c-1][(r-2)*M + c-2] = false;
            AdjMatrix[(r-1)*M + c-2][(r-1)*M + c-1] = false;
            AdjMatrix[(r-1)*M + c-1][(r-1)*M + c-2] = false;
            
            // 1)The wall itself
            pair <int, int> MyPair = make_pair(r, c);
            set<pair<int, int> >::iterator it = VWallsPosRem.find (MyPair);
            VWallsPosRem.erase(it);
            isVwallAllowed[MyPair.first][MyPair.second] = false;
            // 2)The corr. vert. wall
            it = HWallsPosRem.find (MyPair);
            if(it!=HWallsPosRem.end())
            {
                HWallsPosRem.erase(it);
                isHwallAllowed[MyPair.first][MyPair.second] = false;
            }
            // 3)The wall to left
            if(r>2)
            {
                MyPair.first = r -1;
                it = VWallsPosRem.find (MyPair);                //What if already deleted?? Check required?
                if(it!=VWallsPosRem.end())
                {
                    VWallsPosRem.erase(it);
                    isVwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
            // 4)The wall to the right
            if(r<N)
            {
                MyPair.first = r + 1;
                it = VWallsPosRem.find(MyPair);
                if(it!=VWallsPosRem.end())
                {
                    VWallsPosRem.erase(it);
                    isVwallAllowed[MyPair.first][MyPair.second] = false;
                }
            }
            
        }
        if(My_Goal == initState->My_Row)
        {
            GameWon = true;
        }
        if(Opp_Goal == initState->Opp_Row)
        {
            GameWon = true;
        }
        
        
    }
    return 0;
}



