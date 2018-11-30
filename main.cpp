#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <queue>
#include <vector>
using namespace std;
typedef vector< int > route;
class graph{
public:
    graph(int, int, int);
    //~graph();
    void insertnode(int, int, int);
    void renew(int i){dirtyfloor=i;}
    void BFS();
    void DFS();
    void DFS_search(int, int);
    route BFS_route(int, int);
    int pos(int y, int x){return y*width+x;}
    pair<int, int> reversepos(int);
    void print_origin();
    void print(string);
    //void print_bestroute(int, int);
    int checkspace(int, int);
    void generateroute();
private:
    vector< int > mapping;
    vector< int > dist;
    //vector< route > bestroute;
    //vector< bool > mapping;
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
    //cin>>m>>n>>battery;
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
    cout<<"test"<<endl;
    ma.BFS();
    cout<<"test2"<<endl;
    ma.DFS();
    cout<<"test3"<<endl;
    ma.generateroute();
    cout<<"test4"<<endl;
    ma.print_origin();
    cout<<"test5"<<endl;
    buffer=argv[1];
    buffer = "./"+buffer+"/final.path";
    ma.print(buffer);
    cout<<"test6"<<endl;
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
    //bestroute.resize(m*n);
    //mapping.resize(m*n);
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
    //queue< route > qq;
    //route r,copyr,router;
    //r.push_back(make_pair(yofr, xofr));
    //bestroute[pos(yofr, xofr)]=r;
    //qq.push(r);
    while(!q.empty()){
        int i,j;
        i=reversepos(q.front()).first;
        j=reversepos(q.front()).second;
        q.pop();
        //copyr=qq.front();
        //qq.pop();
        if(i!=0){
            if(mapping[pos(i-1, j)]==0){
                q.push(pos(i-1, j));
                dist[pos(i-1, j)]=dist[pos(i, j)]+1;
                mapping[pos(i-1, j)]=2;
                //router=copyr;
                //router.push_back(int(i-1,j));
                //bestroute[pos(i-1, j)]=router;
                //qq.push(router);
            }
        }
        if(i!=height-1){
            if(mapping[pos(i+1, j)]==0){
                q.push(pos(i+1, j));
                dist[pos(i+1, j)]=dist[pos(i, j)]+1;
                mapping[pos(i+1, j)]=2;
                //router=copyr;
                //router.push_back(make_pair(i+1,j));
                //bestroute[pos(i+1, j)]=router;
                //qq.push(router);
            }
        }
        if(j!=0){
            if(mapping[pos(i, j-1)]==0){
                q.push(pos(i, j-1));
                dist[pos(i, j-1)]=dist[pos(i, j)]+1;
                mapping[pos(i, j-1)]=2;
                //router=copyr;
                //router.push_back(make_pair(i,j-1));
                //bestroute[pos(i, j-1)]=router;
                //qq.push(router);
            }
        }
        if(j!=width-1){
            if(mapping[pos(i, j+1)]==0){
                q.push(pos(i, j+1));
                dist[pos(i, j+1)]=dist[pos(i, j)]+1;
                mapping[pos(i, j+1)]=2;
                //router=copyr;
                //router.push_back(make_pair(i,j+1));
                //bestroute[pos(i, j+1)]=router;
                //qq.push(router);
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
        int previous=predecessor[pos(rooty, rootx)];
        while(checkspace(reversepos(previous).first, reversepos(previous).second)==0){
            //originroute.push_back(previous);
            previous=predecessor[previous];
        }
        route newroute=BFS_route(pos(rooty,rootx),previous);
        for(int i=1;i<newroute.size();i++){
            originroute.push_back(newroute[i]);
        }
        return;
    }
    else{
        if(rootx!=width-1){
            if(mapping[pos(rooty, rootx+1)]==2){
                //mapping[pos(rooty, rootx+1)]=true;
                originroute.push_back(pos(rooty, rootx+1));
                predecessor[pos(rooty, rootx+1)]=pos(rooty, rootx);
                mapping[pos(rooty, rootx+1)]=3;
                cleared++;
                DFS_search(rooty, rootx+1);
            }
        }
        if(rootx!=0){
            if(mapping[pos(rooty, rootx-1)]==2){
                //mapping[pos(rooty, rootx-1)]=true;
                originroute.push_back(pos(rooty, rootx-1));
                predecessor[pos(rooty, rootx-1)]=pos(rooty, rootx);
                mapping[pos(rooty, rootx-1)]=3;
                cleared++;
                DFS_search(rooty, rootx-1);
            }
        }
        if(rooty!=height-1){
            if(mapping[pos(rooty+1, rootx)]==2){
                //mapping[pos(rooty+1, rootx)]=true;
                originroute.push_back(pos(rooty+1, rootx));
                predecessor[pos(rooty+1, rootx)]=pos(rooty+1, rootx);
                mapping[pos(rooty+1, rootx)]=3;
                cleared++;
                DFS_search(rooty+1, rootx);
            }
        }
        if(rooty!=0){
            if(mapping[pos(rooty-1, rootx)]==2){
                //mapping[pos(rooty-1, rootx)]=true;
                originroute.push_back(pos(rooty-1, rootx));
                predecessor[pos(rooty-1, rootx)]=pos(rooty, rootx);
                mapping[pos(rooty-1, rootx)]=3;
                cleared++;
                DFS_search(rooty-1, rootx);
            }
        }
    }
}
void graph::print_origin(){
    cout<<originroute.size()<<endl;
    for(int i=0;i<originroute.size();i++){
        cout<<originroute[i]/width<<' '<<originroute[i]%width<<endl;
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
/*void graph::print_bestroute(int m, int n){
    cout<<endl<<bestroute[pos(m,n)].size()<<endl;
    for(int i=0;i<bestroute[pos(m,n)].size();i++){
        cout<<bestroute[pos(m,n)][i].first<<' '<<bestroute[pos(m,n)][i].second<<endl;
    }
    cout<<endl;
}*/
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
                cout<<"check"<<endl;
                route newroute=BFS_route(originroute[i+1], pos(yofr, xofr));
                for(int j=1;j<newroute.size();j++){
                    finalroute.push_back(newroute[j]);
                }
                batterynow=0;
                for(int j=newroute.size()-2;j>0;j--){
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
    cout<<"test7"<<endl;
    queue< route > qq;//queue for route
    queue< int > q;//queue for node
    vector< bool > visited;
    visited.resize(width*height);
    visited.assign(width*height, false);
    q.push(start);
    route r,rr;
    r.push_back(start);
    qq.push(r);
    while(!q.empty()){
        int i=q.front();
        q.pop();
        r=qq.front();
        qq.pop();
        if(i/width!=0){
            if((mapping[i-width]==2||mapping[i-width]==3||i-width==goal)&&!visited[i-width]){
                if(i-width==goal){
                    cout<<"check2"<<endl;
                    r.push_back(i-width);
                    break;
                }
                else{
                    visited[i-width]=true;
                    q.push(i-width);
                    rr=r;
                    rr.push_back(i-width);
                    qq.push(rr);
                }
            }
        }
        if(i/width!=height-1){
            if((mapping[i+width]==2||mapping[i+width]==3||i+width==goal)&&!visited[i+width]){
                if(i+width==goal){
                    cout<<"check3"<<endl;
                    r.push_back(i+width);
                    break;
                }
                else{
                    visited[i+width]=true;
                    q.push(i+width);
                    rr=r;
                    rr.push_back(i+width);
                    qq.push(rr);
                }
            }
        }
        if(i%width!=0){
            if((mapping[i-1]==2||mapping[i-1]==3||i-1==goal)&&!visited[i-1]){
                if(i-1==goal){
                    cout<<"check4"<<endl;
                    r.push_back(i-1);
                    break;
                }
                else{
                    visited[i-1]=true;
                    q.push(i-1);
                    rr=r;
                    rr.push_back(i-1);
                    qq.push(rr);
                }
            }
        }
        if(i%width!=width-1){
            if((mapping[i+1]==2||mapping[i+1]==3||i+1==goal)&&!visited[i+1]){
                if(i+1==goal){
                    cout<<"check5"<<endl;
                    r.push_back(i+1);
                    break;
                }
                else{
                    visited[i+1]=true;
                    q.push(i+1);
                    rr=r;
                    rr.push_back(i+1);
                    qq.push(rr);
                }
            }
        }
    }
    rr.clear();
    return r;
}
