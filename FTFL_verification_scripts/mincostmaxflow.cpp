/*input
7 7 3
14 7 5 13 12 15 8 
8 18 1 9 10 3 6 
19 7 10 5 16 11 15
*/

// Implementation of min cost max flow algorithm using adjacency
// matrix (Edmonds and Karp 1972).  This implementation keeps track of
// forward and reverse edges separately (so you can set cap[i][j] !=
// cap[j][i]).  For a regular max flow, set all edge costs to 0.
//
// Running time, O(|V|^2) cost per augmentation
//     max flow:           O(|V|^3) augmentations
//     min cost max flow:  O(|V|^4 * MAX_EDGE_COST) augmentations
//     
// INPUT: 
//     - graph, constructed using AddEdge()
//     - source
//     - sink
//
// OUTPUT:
//     - (maximum flow value, minimum cost value)
//     - To obtain the actual flow, look at positive values only.

#include <bits/stdc++.h>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long L;
typedef vector<L> VL;
typedef vector<VL> VVL;
typedef pair<int, int> PII;
typedef vector<PII> VPII;

const L INF = numeric_limits<L>::max() / 4;

VI clients, facilities, opening;

struct MinCostMaxFlow {
  int N;
  VVL cap, flow, cost;
  VI found;
  VL dist, pi, width;
  VPII dad;

  MinCostMaxFlow(int N) : 
    N(N), cap(N, VL(N)), flow(N, VL(N)), cost(N, VL(N)), 
    found(N), dist(N), pi(N), width(N), dad(N) {}
  
  void AddEdge(int from, int to, L cap, L cost) {
    this->cap[from][to] = cap;
    this->cost[from][to] = cost;
  }
  
  void Relax(int s, int k, L cap, L cost, int dir) {
    L val = dist[s] + pi[s] - pi[k] + cost;
    if (cap && val < dist[k]) {
      dist[k] = val;
      dad[k] = make_pair(s, dir);
      width[k] = min(cap, width[s]);
    }
  }

  L Dijkstra(int s, int t) {
    fill(found.begin(), found.end(), false);
    fill(dist.begin(), dist.end(), INF);
    fill(width.begin(), width.end(), 0);
    dist[s] = 0;
    width[s] = INF;
    
    while (s != -1) {
      int best = -1;
      found[s] = true;
      for (int k = 0; k < N; k++) {
        if (found[k]) continue;
        Relax(s, k, cap[s][k] - flow[s][k], cost[s][k], 1);
        Relax(s, k, flow[k][s], -cost[k][s], -1);
        if (best == -1 || dist[k] < dist[best]) best = k;
      }
      s = best;
    }

    for (int k = 0; k < N; k++)
      pi[k] = min(pi[k] + dist[k], INF);
    return width[t];
  }

  pair<L, L> GetMaxFlow(int s, int t) {
    L totflow = 0, totcost = 0;
    while (L amt = Dijkstra(s, t)) {
      totflow += amt;
      for (int x = t; x != s; x = dad[x].first) {
        if (dad[x].second == 1) {
          flow[dad[x].first][x] += amt;
          totcost += amt * cost[dad[x].first][x];
        } else {
          flow[x][dad[x].first] -= amt;
          totcost -= amt * cost[x][dad[x].first];
        }
      }
    }
    return make_pair(totflow, totcost);
  }
};

// BEGIN CUT
// The following code solves UVA problem #10594: Data Flow

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

vector<pair<int, int> > fac;

int main() {
  int n, m, r;
  cin >> m >> n >> r;
  clients.resize(m);
  facilities.resize(n);
  opening.resize(n);
  for(int i = 0; i < m; i++)
  {
    cin >> clients[i];
  }
  for(int i = 0; i < n; i++)
  {
    cin >> facilities[i];
  }
  for(int i = 0; i < n; i++)
  {
    cin >> opening[i];
    fac.push_back({facilities[i], opening[i]});
  }
  sort(fac.begin(), fac.end());
  for(int i = 0; i < fac.size(); i++)
  {
  	facilities[i] = fac[i].first;
  	opening[i] = fac[i].second;
  }
  int number_vert = 2 * n + 3;
  MinCostMaxFlow mcmf(number_vert);
  mcmf.AddEdge(0, 1, r, 0);
  for(int i = 0; i < n; i++)
  {
    int vert = 2*i + 2;
    int temp_vert = 2*i + 3;
    mcmf.AddEdge(1, vert, 1, calc_cost(-1, i));
    mcmf.AddEdge(vert, temp_vert, 1, opening[i]);
    mcmf.AddEdge(temp_vert, 2 * n + 2, 1, calc_cost(i, -1));
  }
  for(int i = 0; i < n; i++)
  {
    int u = 2*i + 3;
    for(int j = i + 1; j < n; j++)
    {
      int v = 2*j + 2;
      mcmf.AddEdge(u, v, 1, calc_cost(i, j));
    }
  }
  pair<L, L> res = mcmf.GetMaxFlow(0, 2*n + 2);
  if(res.first != r)
  {
    return 0;
  }
  else
  {
    cout << res.second << endl;
  }
  return 0;
}

// END CUT