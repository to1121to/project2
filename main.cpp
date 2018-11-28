#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <queue>
#include <vector>
using namespace std;
typedef vector< pair< int, int > > route;
class graph{
public:
    graph(int, int, int);
    //~graph();
    void insertnode(int, int, int);
    void renew(int i){dirtyfloor=i;}
    void BFS();
    void DFS();
    void DFS_search(int, int);
    int pos(int y, int x){return y*width+x;}
    void print_origin();
    void print(string);
    void print_bestroute(int, int);
    int checkspace(int, int);
    void generateroute();
private:
    vector< int > mapping;
    vector< int > dist;
    vector< route > bestroute;
    vector< bool > visited;
    route predecessor;
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
    cin>>m>>n>>battery;
    graph ma(m,n,battery);
    int needtoclear=0;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            char c;
            file>>c;
            //cin>>c;
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
    ma.DFS();
    ma.generateroute();
    buffer=argv[1];
    buffer = "./"+buffer+"/final.path";
    ma.print(buffer);
    //ma.print("test");
    return 0;
}
graph::graph(int m,int n,int bat){
    height=m;
    width=n;
    battery=bat;
    cleared=0;
    mapping.resize(m*n);
    dist.resize(m*n);
    bestroute.resize(m*n);
    visited.resize(m*n);
    predecessor.resize(m*n);
}
void graph::insertnode(int i,int j,int value){
    mapping[pos(i, j)]=value;
    if(value==9999){
        xofr=j;
        yofr=i;
        visited[pos(i, j)] = true;
        dist[pos(i, j)] = 0;
    }else if(value==0) visited[pos(i, j)] = false;
    else{
        visited[pos(i, j)] = true;
        dist[pos(i, j)] = -1;
    }
}
void graph::BFS(){
    queue< pair< int, int > > q;
    q.push(make_pair(yofr, xofr));
    queue< route > qq;
    route r,copyr,router;
    r.push_back(make_pair(yofr, xofr));
    bestroute[pos(yofr, xofr)]=r;
    qq.push(r);
    while(!q.empty()){
        int i,j;
        i=q.front().first;
        j=q.front().second;
        q.pop();
        copyr=qq.front();
        qq.pop();
        if(i!=0){
            if(!visited[pos(i-1, j)]){
                q.push(make_pair(i-1,j));
                dist[pos(i-1, j)]=dist[pos(i, j)]+1;
                visited[pos(i-1, j)]=true;
                router=copyr;
                router.push_back(make_pair(i-1,j));
                bestroute[pos(i-1, j)]=router;
                qq.push(router);
            }
        }
        if(i!=height-1){
            if(!visited[pos(i+1, j)]){
                q.push(make_pair(i+1,j));
                dist[pos(i+1, j)]=dist[pos(i, j)]+1;
                visited[pos(i+1, j)]=true;
                router=copyr;
                router.push_back(make_pair(i+1,j));
                bestroute[pos(i+1, j)]=router;
                qq.push(router);
            }
        }
        if(j!=0){
            if(!visited[pos(i, j-1)]){
                q.push(make_pair(i,j-1));
                dist[pos(i, j-1)]=dist[pos(i, j)]+1;
                visited[pos(i, j-1)]=true;
                router=copyr;
                router.push_back(make_pair(i,j-1));
                bestroute[pos(i, j-1)]=router;
                qq.push(router);
            }
        }
        if(j!=width-1){
            if(!visited[pos(i, j+1)]){
                q.push(make_pair(i,j+1));
                dist[pos(i, j+1)]=dist[pos(i, j)]+1;
                visited[pos(i, j+1)]=true;
                router=copyr;
                router.push_back(make_pair(i,j+1));
                bestroute[pos(i, j+1)]=router;
                qq.push(router);
            }
        }
    }
}
void graph::DFS(){
    DFS_search(yofr, xofr);
}
void graph::DFS_search(int rooty, int rootx){
    if(checkspace(rooty, rootx)==0){
        if(cleared==dirtyfloor) return;
        pair<int, int> previous=predecessor[pos(rooty, rootx)];
        while(checkspace(previous.first, previous.second)==0){
            originroute.push_back(previous);
            previous=predecessor[pos(previous.first, previous.second)];
        }
        originroute.push_back(previous);
        return;
    }
    else{
        if(rootx!=width-1){
            if(mapping[pos(rooty, rootx+1)]==0){
                //visited[pos(rooty, rootx+1)]=true;
                originroute.push_back(make_pair(rooty, rootx+1));
                predecessor[pos(rooty, rootx+1)]=make_pair(rooty, rootx);
                mapping[pos(rooty, rootx+1)]=2;
                cleared++;
                DFS_search(rooty, rootx+1);
            }
        }
        if(rootx!=0){
            if(mapping[pos(rooty, rootx-1)]==0){
                //visited[pos(rooty, rootx-1)]=true;
                originroute.push_back(make_pair(rooty, rootx-1));
                predecessor[pos(rooty, rootx-1)]=make_pair(rooty, rootx);
                mapping[pos(rooty, rootx-1)]=2;
                cleared++;
                DFS_search(rooty, rootx-1);
            }
        }
        if(rooty!=height-1){
            if(mapping[pos(rooty+1, rootx)]==0){
                //visited[pos(rooty+1, rootx)]=true;
                originroute.push_back(make_pair(rooty+1, rootx));
                predecessor[pos(rooty+1, rootx)]=make_pair(rooty+1, rootx);
                mapping[pos(rooty+1, rootx)]=2;
                cleared++;
                DFS_search(rooty+1, rootx);
            }
        }
        if(rooty!=0){
            if(mapping[pos(rooty-1, rootx)]==0){
                //visited[pos(rooty-1, rootx)]=true;
                originroute.push_back(make_pair(rooty-1, rootx));
                predecessor[pos(rooty-1, rootx)]=make_pair(rooty, rootx);
                mapping[pos(rooty-1, rootx)]=2;
                cleared++;
                DFS_search(rooty-1, rootx);
            }
        }
    }
}
void graph::print_origin(){
    cout<<originroute.size()<<endl;
    for(int i=0;i<originroute.size();i++){
        cout<<originroute[i].first<<' '<<originroute[i].second<<endl;
    }
}
void graph::print(string buffer){
    fstream file;
    file.open(buffer,ios::out);
    file<<finalroute.size()<<endl;
    for(int i=0;i<finalroute.size();i++){
        file<<finalroute[i].first<<' '<<finalroute[i].second<<endl;
    }
}
void graph::print_bestroute(int m, int n){
    cout<<endl<<bestroute[pos(m,n)].size()<<endl;
    for(int i=0;i<bestroute[pos(m,n)].size();i++){
        cout<<bestroute[pos(m,n)][i].first<<' '<<bestroute[pos(m,n)][i].second<<endl;
    }
    cout<<endl;
}
int graph::checkspace(int rooty,int rootx){
    int space=0;
    if(rooty!=height-1){
        if(mapping[pos(rooty+1,rootx)]==0) space++;
    }
    if(rooty!=0){
        if(mapping[pos(rooty-1,rootx)]==0) space++;
    }
    if(rootx!=width-1){
        if(mapping[pos(rooty,rootx+1)]==0) space++;
    }
    if(rootx!=0){
        if(mapping[pos(rooty,rootx-1)]==0) space++;
    }
    return space;
}
void graph::generateroute(){
    int batterynow=0;
    for(int i=0;i<originroute.size();i++){
        finalroute.push_back(originroute[i]);
        batterynow++;
        if(i!=originroute.size()-1){
            if(batterynow+1+dist[pos(originroute[i+1].first, originroute[i+1].second)]>battery){
                for(int j=dist[pos(originroute[i].first, originroute[i].second)]-1;j>0;j--){
                    finalroute.push_back(bestroute[pos(originroute[i].first, originroute[i].second)][j]);
                }
                batterynow=0;
                for(int j=0;j<dist[pos(originroute[i+1].first, originroute[i+1].second)];j++){
                    finalroute.push_back(bestroute[pos(originroute[i+1].first, originroute[i+1].second)][j]);
                    batterynow++;
                }
            }
        }

    }
    for(int j=dist[pos(originroute[originroute.size()-1].first, originroute[originroute.size()-1].second)]-1;j>=0;j--){
        finalroute.push_back(bestroute[pos(originroute[originroute.size()-1].first, originroute[originroute.size()-1].second)][j]);
    }
}
