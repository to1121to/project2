#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <queue>
#include <stack>
#include <vector>
using namespace std;
typedef vector< int > route;
class graph{
public:
    graph(int, int, int);
    void insertnode(int, int, int);
    void renew(int i){dirtyfloor=i;}
    void BFS();
    void DFS();
    route BFS_route(int, int);
    int pos(int y, int x){return y*width+x;}
    pair<int, int> reversepos(int);
    void print_origin(string);
    void print(string);
    int checkspace(int, int);
    void generateroute();
private:
    vector< int > mapping;
    vector< int > dist;
    vector< int > predecessor;
    route originroute;
    route finalroute;
    int xofr,yofr;
    int width,height;
    int dirtyfloor;
    int battery;
    int cleared;
};
int main(int argc, char *argv[])
{
    fstream file,file2;
    string buffer=argv[1];
    buffer = "./"+buffer+"/floor.data";
    file.open(buffer,ios::in);
    int m,n,battery;
    file>>m>>n>>battery;
    graph ma(m,n,battery);
    int needtoclear=0;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            char c;
            file>>c;
            switch(c){
            case '1':
                ma.insertnode(i,j,1);
                break;
            case '0':
                ma.insertnode(i,j,0);
                needtoclear++;
                break;
            case 'R':
                ma.insertnode(i,j,9999);
                break;
            }
        }
    }
    ma.renew(needtoclear);
    ma.BFS();
    cout<<"test"<<endl;
    ma.DFS();
    cout<<"test"<<endl;
    ma.generateroute();
    cout<<"test"<<endl;
    buffer=argv[1];
    ma.print_origin(buffer);
    buffer = "./"+buffer+"/final.path";
    ma.print(buffer);
    return 0;
}
graph::graph(int m,int n,int bat){
    height=m;
    width=n;
    battery=bat;
    cleared=0;
    mapping.resize(m*n);
    dist.resize(m*n);
    predecessor.resize(m*n);
}
void graph::insertnode(int i,int j,int value){
    mapping[pos(i, j)]=value;
    if(value==9999){
        xofr=j;
        yofr=i;
        mapping[pos(i, j)] = true;
        dist[pos(i, j)] = 0;
    }else if(value==0) mapping[pos(i, j)] = false;
    else{
        mapping[pos(i, j)] = true;
        dist[pos(i, j)] = -1;
    }
}
void graph::BFS(){
    queue< int > q;
    q.push(pos(yofr, xofr));
    while(!q.empty()){
        int i,j;
        i=reversepos(q.front()).first;
        j=reversepos(q.front()).second;
        q.pop();
        if(i!=0){
            if(mapping[pos(i-1, j)]==0){
                q.push(pos(i-1, j));
                dist[pos(i-1, j)]=dist[pos(i, j)]+1;
                mapping[pos(i-1, j)]=2;
            }
        }
        if(i!=height-1){
            if(mapping[pos(i+1, j)]==0){
                q.push(pos(i+1, j));
                dist[pos(i+1, j)]=dist[pos(i, j)]+1;
                mapping[pos(i+1, j)]=2;
            }
        }
        if(j!=0){
            if(mapping[pos(i, j-1)]==0){
                q.push(pos(i, j-1));
                dist[pos(i, j-1)]=dist[pos(i, j)]+1;
                mapping[pos(i, j-1)]=2;
            }
        }
        if(j!=width-1){
            if(mapping[pos(i, j+1)]==0){
                q.push(pos(i, j+1));
                dist[pos(i, j+1)]=dist[pos(i, j)]+1;
                mapping[pos(i, j+1)]=2;
            }
        }
    }
}
void graph::DFS(){
    stack< int > s;
    s.push(pos(yofr, xofr));
    while(!s.empty()){
        int i=s.top();
        s.pop();
        if(mapping[i]==3) continue;
        if(mapping[i]==2){
            mapping[i]=3;
            cleared++;
        }
        int x=reversepos(i).second;
        int y=reversepos(i).first;
        if(y!=yofr || x!=xofr) originroute.push_back(i);
        if(checkspace(y, x)==0){
            if(cleared==dirtyfloor) return;
            int previous=predecessor[i];
            while(checkspace(reversepos(previous).first, reversepos(previous).second)==0){
                previous=predecessor[previous];
            }
            route newroute=BFS_route(i, previous);
            for(int j=1;j<newroute.size();j++){
                originroute.push_back(newroute[j]);
                if(mapping[newroute[j]]==2){
                    mapping[newroute[j]]=3;
                    cleared++;
                }
            }
        }
        else{
            if(x!=width-1){
                if(mapping[i+1]==2){
                    s.push(i+1);
                    predecessor[i+1]=i;
                }
            }
            if(x!=0){
                if(mapping[i-1]==2){
                    s.push(i-1);
                    predecessor[i-1]=i;
                }
            }
            if(y!=height-1){
                if(mapping[i+width]==2){
                    s.push(i+width);
                    predecessor[i+width]=i;
                }
            }
            if(y!=0){
                if(mapping[i-width]==2){
                    s.push(i-width);
                    predecessor[i-width]=i;
                }
            }
        }
    }
}
void graph::print_origin(string buffer){
    string a=buffer = "./"+buffer+"/origin.path";
    fstream file;
    file.open(a,ios::out);
    file<<originroute.size()<<endl;
    for(int i=0;i<originroute.size();i++){
        file<<originroute[i]/width<<' '<<originroute[i]%width<<endl;
    }
}
void graph::print(string buffer){
    fstream file;
    file.open(buffer,ios::out);
    file<<finalroute.size()<<endl;
    for(int i=0;i<finalroute.size();i++){
        file<<finalroute[i]/width<<' '<<finalroute[i]%width<<endl;
    }
}
int graph::checkspace(int rooty,int rootx){
    int space=0;
    if(rooty!=height-1){
        if(mapping[pos(rooty+1,rootx)]==2) space++;
    }
    if(rooty!=0){
        if(mapping[pos(rooty-1,rootx)]==2) space++;
    }
    if(rootx!=width-1){
        if(mapping[pos(rooty,rootx+1)]==2) space++;
    }
    if(rootx!=0){
        if(mapping[pos(rooty,rootx-1)]==2) space++;
    }
    return space;
}
void graph::generateroute(){
    int batterynow=0;
    for(int i=0;i<originroute.size();i++){
        finalroute.push_back(originroute[i]);
        batterynow++;
        if(i!=originroute.size()-1){
            if(batterynow+1+dist[originroute[i+1]]>battery){
                route newroute=BFS_route(originroute[i], pos(yofr, xofr));
                for(int j=1;j<newroute.size();j++){
                    finalroute.push_back(newroute[j]);
                }
                batterynow=0;
                for(int j=newroute.size()-2;j>=0;j--){
                    finalroute.push_back(newroute[j]);
                    batterynow++;
                }
            }
        }

    }
    route last=BFS_route(originroute[originroute.size()-1], pos(yofr, xofr));
    for(int j=1;j<last.size();j++){
        finalroute.push_back(last[j]);
    }
}
pair<int, int> graph::reversepos(int i){
    pair<int, int> re;
    re.first=i/width;
    re.second=i%width;
    return re;
}
route graph::BFS_route(int start, int goal){
    queue< int > q;//queue for node
    vector< bool > visited;
    vector< int > predecessor_BFS;
    visited.resize(width*height);
    visited.assign(width*height, false);
    predecessor_BFS.resize(width*height);
    q.push(goal);
    while(!q.empty()){
        int i=q.front();
        q.pop();
        if(i/width!=0){
            if((mapping[i-width]==2||mapping[i-width]==3||i-width==start)&&!visited[i-width]){
                if(i-width==start){
                    predecessor_BFS[start]=i;
                    break;
                }
                else{
                    visited[i-width]=true;
                    q.push(i-width);
                    predecessor_BFS[i-width]=i;
                }
            }
        }
        if(i/width!=height-1){
            if((mapping[i+width]==2||mapping[i+width]==3||i+width==start)&&!visited[i+width]){
                if(i+width==start){
                    predecessor_BFS[start]=i;
                    break;
                }
                else{
                    visited[i+width]=true;
                    q.push(i+width);
                    predecessor_BFS[i+width]=i;
                }
            }
        }
        if(i%width!=0){
            if((mapping[i-1]==2||mapping[i-1]==3||i-1==start)&&!visited[i-1]){
                if(i-1==start){
                    predecessor_BFS[start]=i;
                    break;
                }
                else{
                    visited[i-1]=true;
                    q.push(i-1);
                    predecessor_BFS[i-1]=i;
                }
            }
        }
        if(i%width!=width-1){
            if((mapping[i+1]==2||mapping[i+1]==3||i+1==start)&&!visited[i+1]){
                if(i+1==start){
                    predecessor_BFS[start]=i;
                    break;
                }
                else{
                    visited[i+1]=true;
                    q.push(i+1);
                    predecessor_BFS[i+1]=i;
                }
            }
        }

    }
    route r;
    int previous=predecessor_BFS[start];
    r.push_back(start);
    while(previous!=goal){
        r.push_back(previous);
        previous=predecessor_BFS[previous];
    }
    r.push_back(goal);
    visited.clear();
    return r;
}
