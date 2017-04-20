#11/19/2013

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <queue>
using namespace std;

struct flight
{
    string flightid;
    string depart_c;
    string arrival_c;
    int depart_t;
    int arrival_t;
    
};

int calc_minutes(flight a)
{
    int t;
    t=a.arrival_t-a.depart_t;
    t=t-40*((a.arrival_t/100) - (a.depart_t/100));
    return t;
    
}



map<string,int> dist;
map<string,list<flight*>> adjlist;
map<string,bool> visited;
map<string,flight*> parent;

priority_queue<pair<int,string>,vector<pair<int,string>>,greater<pair<int,string>>> dista;

void initialise_maps()
{
    map<string,bool>::iterator v_it=visited.begin();
    map<string,int>::iterator  d_it=dist.begin();
    map<string,flight*>::iterator p_it=parent.begin();
    while(v_it!=visited.end())
    {
        visited[v_it->first]=false;
        v_it++;
    }
    while(d_it!=dist.end())
    {
        dist[d_it->first]=10000;
        d_it++;
    }
    while(p_it!=parent.end())
    {
        parent[p_it->first]=NULL;
        p_it++;
    }

}

void shortest_path(string s,string dest)
{
    initialise_maps();
    visited[s]=true;
    dist[s]=0;
    pair<int,string> mypair=make_pair(0, s);
    dista.push(mypair);
    
    string curr_city;
    while(!dista.empty())
    {
        mypair=dista.top();
        curr_city=mypair.second;
        dista.pop();
        visited[curr_city]=true;
        list<flight*>::iterator iter;
        iter=adjlist[curr_city].begin();
        int alt;
        flight* v;
        while(iter!=adjlist[curr_city].end())
        {
            v=*iter;
            
            if((visited[v->arrival_c]==true) ||((parent[v->depart_c]!=NULL) && v->depart_t < parent[v->depart_c]->arrival_t))
            {
                iter++;
                continue;
            }
            alt=dist[v->depart_c]+calc_minutes(*v);
            if(alt<dist[v->arrival_c])
            {
                dist[v->arrival_c]=alt;
                parent[v->arrival_c]=v;
                dista.push(make_pair(alt, v->arrival_c));
            }
            iter++;
        }
    }
    if(dist[dest]==10000)
    {
        cout<<"No path"<<endl;
        return;
    }
    cout<<"You have reached your destination in "<<dist[dest]<<" minutes. The path is:"<<endl;
    flight* path=parent[dest];
    while(path!=NULL)
    {
        cout<<path->flightid<<" "<<path->depart_c<<" to "<<path->arrival_c<<endl;
        path=parent[path->depart_c];
    }
}

void earliest_path(string s,string dest)
{
    initialise_maps();
    visited[s]=true;
    dist[s]=0;
    pair<int,string> mypair=make_pair(0, s);
    dista.push(mypair);
    
    string curr_city;
    while(!dista.empty())
    {
        mypair=dista.top();
        curr_city=mypair.second;
        dista.pop();
        visited[curr_city]=true;
        list<flight*>::iterator iter;
        iter=adjlist[curr_city].begin();
        int alt;
        flight* v;
        while(iter!=adjlist[curr_city].end())
        {
            v=*iter;
            
            if((visited[v->arrival_c]==true) ||((parent[v->depart_c]!=NULL) && v->depart_t < parent[v->depart_c]->arrival_t))
            {
                iter++;
                continue;
            }
            alt=v->arrival_t;
            if(alt<dist[v->arrival_c])
            {
                dist[v->arrival_c]=alt;
                parent[v->arrival_c]=v;
                dista.push(make_pair(alt, v->arrival_c));
            }
            iter++;
        }
    }
    
    if(dist[dest]==10000)
    {
        cout<<"No path"<<endl;
        return;
    }
    cout<<"You have reached your destination at "<<dist[dest]<<" hours. The path is"<<endl;
    flight* path=parent[dest];
    while(path!=NULL)
    {
        cout<<path->flightid<<" "<<path->depart_c<<" to "<<path->arrival_c<<endl;
        path=parent[path->depart_c];
    }
}


int main(int argc, const char * argv[])
{
    fstream File;
    File.open("/Users/PranitArora/Desktop/IF.txt",ios::in|ios::out);
    flight *F1;
    list<flight*>::iterator it;
    while(!File.eof())
    {
        F1=new flight;
        File>>F1->flightid>>F1->depart_c>>F1->depart_t>>F1->arrival_c>>F1->arrival_t;
        it=adjlist[F1->depart_c].begin();
        adjlist[F1->depart_c].push_back(F1);
        visited[F1->depart_c]=false;
        visited[F1->arrival_c]=false;
        dist[F1->depart_c]=100000;
        dist[F1->arrival_c]=10000;
        parent[F1->depart_c]=NULL;
        parent[F1->arrival_c]=NULL;
        
        
    }
    int option;
    string from,to;
    do
    {
        cout<<"Select option: 1. Shortest 2. Earliest 3. exit ";
        cin>>option;
        if(option==1)
        {
            cout<<"Enter departure city ";
            cin>>from;
            cout<<"Enter arrival city ";
            cin>>to;
            shortest_path(from, to);
        }
        if(option==2)
        {
            cout<<"Enter departure city ";
            cin>>from;
            cout<<"Enter arrival city ";
            cin>>to;
            earliest_path(from, to);
        }
    }while(option!=3);
    
  }

