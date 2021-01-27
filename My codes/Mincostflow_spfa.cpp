#include "bits/stdc++.h"
using namespace std;

//problem : https://cses.fi/problemset/task/2121/

#define all(x) begin(x),end(x)
template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }
template<typename T_container, typename T = typename enable_if<!is_same<T_container, string>::value, typename T_container::value_type>::type> ostream& operator<<(ostream &os, const T_container &v) { string sep; for (const T &x : v) os << sep << x, sep = " "; return os; }
#define debug(a) cerr << "(" << #a << ": " << a << ")\n";
typedef long long ll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> pi;
// const int mxN = 1e5+1, oo = 1e9;
const int mxN = 1e4+1;
const ll oo = 1e18;
const string letter = "AEFBDC";
struct mincostflow {
    // Source is Node 0;
    // Sink is node n-1
    // Uses capacity scaling.
    // Runtime: O(|E|^2 *(1+log2(C )))
    struct edge {
        ll f,c;
        int to, cost;
    };
    typedef vector<edge> ve;
    vector<vector<int>> adj;
    vector<edge> edges;
    int n;
    mincostflow(int _n) {
        n=_n;
        adj.resize(n);
        dist.resize(n);
        parent.resize(n);
        pflow.resize(n);
    }
    void addEdge(int a, int b, ll w, int cost, bool directed = true) {
        adj[a].push_back((int)edges.size());
        adj[b].push_back((int)edges.size()+1);
        edges.push_back({0,w,b,cost});
        edges.push_back({0,directed?0:w,a,-cost});
    }
    bitset<mxN> inqueue;
    vector<ll> dist, pflow;
    vi parent;
 
    void SPFA() {
        fill(all(dist),oo);
        queue<int> q; q.push(0);
        parent[0] = -1;
        dist[0] = 0;
        inqueue[0]= true;
        pflow[0] = oo;
        while(!q.empty()) {
            int at = q.front(); q.pop();
            inqueue[at] = false;
            for(int ei: adj[at]) {
                auto& e = edges[ei];
                if(e.c-e.f<1) continue;
                ll constraint = min(e.c-e.f,pflow[at]);
                if(dist[at]+e.cost < dist[e.to]) {
                    parent[e.to] = ei;
                    pflow[e.to] = constraint;
                    dist[e.to] = dist[at]+e.cost;
                    if(!inqueue[e.to]) {
                        inqueue[e.to] = true;
                        q.push(e.to);
                    }
                } else if(dist[at]+e.cost==dist[e.to]) {
                    if(pflow[e.to]< constraint) {
                        parent[e.to] = ei;
                        pflow[e.to] = constraint;
                    }
                }
            }
        }
 
    }
 
    // ll dfs(int at=0, ll mn=1e18) {
    //     if(at==n-1) return mn;
    //     visited[at] = true;
    //     for(int i: adj[at]) {
    //         auto& e = edges[i];
    //         if(!visited[e.to] and e.f+scale<=e.c) {
    //             ll tmp = min(mn,e.c-e.f);
    //             tmp =  min(tmp,dfs(e.to,tmp));
    //             if(tmp) {
    //                 e.f+=tmp;
    //                 auto& o = edges[i^1];
    //                 o.f-=tmp;
    //                 return tmp;
    //             }
    //         }
    //     }
    //     return 0;
    // }
    void findPath(ll extra) {
        int at = n-1;
        // string path = {letter[at]};
        while(at!=0) {
            int ei = parent[at];
            auto& e = edges[ei];
            e.f+=extra;
            auto& o = edges[ei^1];
            o.f-=extra;
            at = o.to;
            // path.push_back(letter[at]);
        }
        // reverse(all(path));
        // debug(path);
    }
    pair<ll,ll> solve(int k) {
        ll flow = 0, cost = 0;
        while(true) {
            SPFA();
            if(dist[n-1]==oo)  {
                break;
            }
            int extraflow = pflow[n-1];
            ll shortestpath = dist[n-1];
            // debug(shortestpath);
            // debug(extraflow);
            if(flow+extraflow>=k) {
                cost+=shortestpath*(k-flow);
                flow = k;
                break;
            }
            flow+=extraflow;
            cost+=shortestpath*extraflow;
            findPath(pflow[n-1]);
            
        }
        return {flow,cost};
    }
};
int main() {
    int n,m,k; cin >> n >> m >> k;
    mincostflow f(n);
    for(int i=0;i<m;++i) {
        int a,b,cost, c; cin >> a >> b >> c >> cost;
        --a,--b;
        f.addEdge(a,b,c,cost);
    }
    // cout << "Solving Now!\n";
    auto [flow,cost] = f.solve(k);
    if(flow<k) {
        cout << "-1\n";
    } else {
        cout << cost << endl;
    }
    
 
}
