/*input
7 7 3 5
14 7 5 13 12 15 8 
8 18 1 9 10 3 6
*/

// Push-Relabel implementation of the cost-scaling algorithm
// Runs in O( <max_flow> * log(V * max_edge_cost)) = O( V^3 * log(V * C))
// Really fast in practice, 3e4 edges are fine.
// Operates on integers, costs are multiplied by N!!

#include <bits/stdc++.h>

using namespace std;

template<typename flow_t = int, typename cost_t = int>
struct mcSFlow{
    struct Edge{
        cost_t c;
        flow_t f;
        int to, rev;
        Edge(int _to, cost_t _c, flow_t _f, int _rev):c(_c), f(_f), to(_to), rev(_rev){}
    };
    static constexpr cost_t INFCOST = numeric_limits<cost_t>::max()/2;
    cost_t eps;
    int N, S, T;
    vector<vector<Edge> > G;
    vector<unsigned int> isq, cur;
    vector<flow_t> ex;
    vector<cost_t> h;
    mcSFlow(int _N, int _S, int _T):eps(0), N(_N), S(_S), T(_T), G(_N){}
    void add_edge(int a, int b, cost_t cost, flow_t cap){
    assert(cap>=0);
        assert(a>=0&&a<N&&b>=0&&b<N);
        if(a==b){assert(cost>=0); return;}
        cost*=N;
        eps = max(eps, abs(cost));
        G[a].emplace_back(b, cost, cap, G[b].size());
        G[b].emplace_back(a, -cost, 0, G[a].size()-1);
    }
    void add_flow(Edge& e, flow_t f) {
        Edge &back = G[e.to][e.rev];
        if (!ex[e.to] && f)
            hs[h[e.to]].push_back(e.to);
        e.f -= f; ex[e.to] += f;
        back.f += f; ex[back.to] -= f;
    }
    vector<vector<int> > hs;
    vector<int> co;
    flow_t max_flow() {
        ex.assign(N, 0);
        h.assign(N, 0); hs.resize(2*N);
        co.assign(2*N, 0); cur.assign(N, 0);
        h[S] = N;
        ex[T] = 1;
        co[0] = N-1;
        for(auto &e:G[S]) add_flow(e, e.f);
        if(hs[0].size())
        for (int hi = 0;hi>=0;) {
            int u = hs[hi].back();
            hs[hi].pop_back();
            while (ex[u] > 0) { // discharge u
                if (cur[u] == G[u].size()) {
                    h[u] = 1e9;
                    for(unsigned int i=0;i<G[u].size();++i){
                        auto &e = G[u][i];
                        if (e.f && h[u] > h[e.to]+1){
                            h[u] = h[e.to]+1, cur[u] = i;
                        }
                    }
                    if (++co[h[u]], !--co[hi] && hi < N)
                        for(int i=0;i<N;++i)
                            if (hi < h[i] && h[i] < N){
                                --co[h[i]];
                                h[i] = N + 1;
                            }
                    hi = h[u];
                } else if (G[u][cur[u]].f && h[u] == h[G[u][cur[u]].to]+1)
                    add_flow(G[u][cur[u]], min(ex[u], G[u][cur[u]].f));
                else ++cur[u];
      }
            while (hi>=0 && hs[hi].empty()) --hi;
        }
        return -ex[S];
    }
    void push(Edge &e, flow_t amt){
        if(e.f < amt) amt=e.f;
        e.f-=amt; ex[e.to]+=amt;
        G[e.to][e.rev].f+=amt; ex[G[e.to][e.rev].to]-=amt;
    }
    void relabel(int vertex){
        cost_t newHeight = -INFCOST;
        for(unsigned int i=0;i<G[vertex].size();++i){
            Edge const&e = G[vertex][i];
            if(e.f && newHeight < h[e.to]-e.c){
                newHeight = h[e.to] - e.c;
                cur[vertex] = i;
            }
        }
        h[vertex] = newHeight - eps;
    }
    static constexpr int scale=2;
    pair<flow_t, cost_t> minCostMaxFlow(){
        cost_t retCost = 0;
        for(int i=0;i<N;++i)
            for(Edge &e:G[i])
                retCost += e.c*(e.f);
        //find max-flow
        flow_t retFlow = max_flow();
        h.assign(N, 0); ex.assign(N, 0);
        isq.assign(N, 0); cur.assign(N,0);
        queue<int> q;
        for(;eps;eps>>=scale){
            //refine
            fill(cur.begin(), cur.end(), 0);
            for(int i=0;i<N;++i)
                for(auto &e:G[i])
                    if(h[i] + e.c - h[e.to] < 0 && e.f) push(e, e.f);
            for(int i=0;i<N;++i){
                if(ex[i]>0){
                    q.push(i);
                    isq[i]=1;
                }
            }
            // make flow feasible
            while(!q.empty()){
                int u=q.front();q.pop();
                isq[u]=0;
                while(ex[u]>0){
                    if(cur[u] == G[u].size())
                        relabel(u);
                    for(unsigned int &i=cur[u], max_i = G[u].size();i<max_i;++i){
                        Edge &e=G[u][i];
                        if(h[u] + e.c - h[e.to] < 0){
                            push(e, ex[u]);
                            if(ex[e.to]>0 && isq[e.to]==0){
                                q.push(e.to);
                                isq[e.to]=1;
                            }
                            if(ex[u]==0) break;
                        }
                    }
                }
            }
            if(eps>1 && eps>>scale==0){
                eps = 1<<scale;
            }
        }
        for(int i=0;i<N;++i){
            for(Edge &e:G[i]){
                retCost -= e.c*(e.f);
            }
        }
        return make_pair(retFlow, retCost/2/N);
    }
    flow_t getFlow(Edge const &e){
        return G[e.to][e.rev].f;
    }
};

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long L;
typedef vector<L> VL;
typedef vector<VL> VVL;
typedef pair<int, int> PII;
typedef vector<PII> VPII;

VI clients, facilities, opening;

L calc_cost(int start, int end)
{
  L ans = 0;
  if(start == -1)
  {
    for(int i = 0; i < clients.size(); i++)
    {
      ans += max(0, facilities[end] - clients[i]);
    }
  }
  else if(end == -1)
  {
    for(int i = 0; i < clients.size(); i++)
    {
      ans += max(0, -1 * (facilities[start] - clients[i]));
    }
  }
  else
  {
    for(int i = 0; i < clients.size(); i++)
    {
      ans += max(0, min(clients[i] - facilities[start], facilities[end] - clients[i]));
    }
  }
  return ans;
}

// int main(){

//   mcSFlow<> mcmf(5,0,3);
//   mcmf.add_edge(0,1,1,0);
//   mcmf.add_edge(1,4,1,1);
//   mcmf.add_edge(1,2,-1,1);
//   mcmf.add_edge(2,3,-1,1);
//   mcmf.add_edge(3,1,-1,1);
//   pair<int,int> ans = mcmf.minCostMaxFlow();
//   cout<<"Flow = "<<ans.first<<" "<<"Cost = "<<ans.second<<endl;
// }

int main() {
  int n, m, r, k;
  cin >> m >> n >> r >> k;
  clients.resize(m);
  facilities.resize(n);
  // opening.resize(n);
  for(int i = 0; i < m; i++)
  {
    cin >> clients[i];
  }
  for(int i = 0; i < n; i++)
  {
    cin >> facilities[i];
  }
  sort(facilities.begin(), facilities.end());
  int alpha = (k + r - 1) / r;
  cout<<"Alpha: "<<alpha<<endl;
  int x1 = (1 - alpha) * r + k;
  int x2 = alpha * r - k;
  cout<<"#Alpha layers: "<<x1<<endl;
  cout<<"#Alpha-1 layers: "<<x2<<endl;
  int number_vert = alpha * n + 4;
  mcSFlow<> mcmf(number_vert, 0, alpha*n + 3);
  int X = 1e6;
  // int X = 0;
  for(int i = 1; i <= n; i++)
  {
  	// cout<<0<<" to "<<i<<", capacity: "<<1<<", cost: "<<calc_cost(-1, i - 1)<<endl;
    mcmf.add_edge(0, i, calc_cost(-1, i - 1), 1);
  	// cout<<(alpha - 1) * n + i<<" to "<<(alpha) * n + 1<<", capacity: "<<1<<", cost: "<<calc_cost(i - 1, -1)<<endl;
    mcmf.add_edge((alpha - 1) * n + i, alpha * n + 1, calc_cost(i - 1, -1) + alpha*X, 1);
  	// cout<<(alpha - 2) * n + i<<" to "<<(alpha) * n + 2<<", capacity: "<<1<<", cost: "<<calc_cost(i - 1, -1)<<endl;
    if(x2!=0)
    	mcmf.add_edge((alpha - 2) * n + i, alpha * n + 2, calc_cost(i - 1, -1) + alpha*X, 1);
    // cout<<endl;
  }

  for(int counter = 1; counter < alpha; counter++)
  {
    for(int i = 1; i <= n; i++)
    {
      for(int j = i + 1; j <= n; j++)
      {
        int curr_vertex = (counter - 1) * n + i;
        int next_vertex = counter * n + j;
		// cout<<curr_vertex<<" to "<<next_vertex<<", capacity: "<<1<<", cost: "<<calc_cost(i - 1, j - 1)<<endl;
        // productive edges
        mcmf.add_edge(curr_vertex, next_vertex, calc_cost(i - 1, j - 1) + alpha*X, 1);
      }

      // LOOPS (balancing edges)
      // cout<<(counter - 1) * n + i<<" to "<<counter * n + i<<", capacity: "<<1<<", cost: "<<-X<<endl;
      mcmf.add_edge((counter - 1) * n + i, counter * n + i, -X, 1);
    }

  }

  for(int i = 1; i <= n; i++)
  {
    // cout<<(alpha - 1) * n + i<<" to "<<i<<", capacity: "<<1<<", cost: "<<-X<<endl;
    mcmf.add_edge((alpha - 1) * n + i, i, -X, 1);
  }

  mcmf.add_edge(alpha*n + 1, alpha*n + 3, 0, x1);
  if(x2!=0)
  mcmf.add_edge(alpha*n + 2, alpha*n + 3, 0, x2);

  pair<L, L> res = mcmf.minCostMaxFlow();
  if(res.first != r)
  {
    cout << "MAX FLOW wrong" << endl;
    return 0;
  }
  else
  {
    cout << "Max flow: "<< res.first <<" Min cost: "<< res.second << endl;
  }
  return 0;
}

