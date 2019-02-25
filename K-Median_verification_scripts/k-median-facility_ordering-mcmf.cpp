/*input
7 7 3 7
14 7 5 13 12 15 8 
8 18 1 9 10 3 6
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
int n, m, r, k;
int alpha, x1, x2, number_vert;
int start1;

int getSource(){
	return 0;
}

int getFacility(int layer, int facility, int position){
	return (layer-1)*2*n + (facility-1)*2 + position;
}

int getAlphaSink(){
	return start1-2;
}

int getPrevAlphaSink(){
	return start1-1;
}

int getGlobalSink(){
	return start1;
}

int BalancingSource(int facility){
	return start1+facility;
}

int BalancingDest(int facility){
	return start1+n+facility;
}

int main() {
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
  alpha = (k + r - 1) / r;
  x1 = (1 - alpha) * r + k;
  x2 = alpha * r - k;
  start1 = 2*alpha*n + 3;
  number_vert = start1+2*n+1;
  MinCostMaxFlow mcmf(number_vert);
  int X = 1e6;
  
  for(int i=1;i<=n;i++){
  	mcmf.AddEdge(getSource(), getFacility(1, i, 1), 1, calc_cost(-1,i-1));
  	mcmf.AddEdge(getFacility(alpha, i, 2), getAlphaSink(), 1, calc_cost(i-1,-1));
  	mcmf.AddEdge(getFacility(alpha-1, i, 2), getPrevAlphaSink(), 1, calc_cost(i-1,-1));
  }
  mcmf.AddEdge(getAlphaSink(), getGlobalSink(), x1, 0);			// Modify cost
  mcmf.AddEdge(getPrevAlphaSink(), getGlobalSink(), x2, 0);		// Modify cost

  for(int i=1;i<=alpha;i++){
  	for(int j=1;j<=n;j++){
  		if(i<alpha){
	  		for(int l=j+1;l<=n;l++){
	  			mcmf.AddEdge(getFacility(i,j,2), getFacility(i+1,l,1), 1, calc_cost(j-1,l-1));
	  		}
	  	}
  		mcmf.AddEdge(getFacility(i,j,1), getFacility(i,j,2), 1, 0);		// Modify cost
  	}
  }
  pair<L, L> res = mcmf.GetMaxFlow(getSource(), getGlobalSink());
  cout << "Max flow: "<< res.first << " Min Cost: " <<res.second << endl;
  return 0;
}

// END CUT