/*input
7 7 5
14 7 5 13 12 15 8 
8 18 1 9 10 3 6
1 2 3 1 2 3 1
*/

// Nihesh Anderson - knandy

#include<bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 

using namespace __gnu_pbds;
using namespace std;

typedef long long ll;
typedef tree<ll,null_type,less<ll>,rb_tree_tag,tree_order_statistics_node_update> ordered_set; // find_by_order, order_of_key
ll MOD=1000000007;

#ifndef ONLINE_JUDGE
#define trace(...) __f(#__VA_ARGS__, __VA_ARGS__)
template <typename Arg1> void __f(const char *name, Arg1 &&arg1) {
    cerr << name << " : " << arg1 << std::endl;
}
template <typename Arg1, typename... Args>
void __f(const char *names, Arg1 &&arg1, Args &&... args) {
    const char *comma = strchr(names + 1, ',');
    cerr.write(names, comma - names) << " : " << arg1 << " | ";
    __f(comma + 1, args...);
}

#else
#define trace(...)
#endif // ifndef ONLINE_JUDGE

#define endl '\n'
#define fast_cin() ios_base::sync_with_stdio(false)
#define pb push_back
#define gcd(a,b) __gcd((a),(b))
#define lcm(a,b) ((a)*(b))/gcd((a),(b))
#define debug1(x) cerr << x << endl;
#define debug2(x,y) cerr << x << " " << y << endl;
#define debug3(x,y,z) cerr << x << " " << y << " " << z << endl;
#define FOR(i, a, b) for (ll i=a; i<(b); i++)
#define FORR(i, a, b) for (ll i=a; i>=(b); i--)
ll INF = 2000000000000000001;
ll nextll(){ll inp; cin>>inp; return inp;}
string nextStr(){string inp; cin>>inp; return inp;}
double nextDouble(){double inp; cin>>inp; return inp;}
ll power(ll a, ll n, ll md){if(n==0){return 1;}else{ll res=power(a,n/2,md);res=(res*res)%md;if(n%2!=0){res=(res*a)%md;}return res;}}
random_device rndm;
mt19937 grndm(rndm());
void mix(ll* a, ll* b){shuffle(a,b,grndm);}

// Two-phase simplex algorithm for solving linear programs of the form
//
//     maximize     c^T x
//     subject to   Ax <= b
//                  x >= 0
//
// INPUT: A -- an m x n matrix
//        b -- an m-dimensional vector
//        c -- an n-dimensional vector
//        x -- a vector where the optimal solution will be stored
//
// OUTPUT: value of the optimal solution (infinity if unbounded
//         above, nan if infeasible)
//
// To use this code, create an LPSolver object with A, b, and c as
// arguments.  Then, call Solve(x).

typedef long double DOUBLE;
typedef vector<DOUBLE> VD;
typedef vector<VD> VVD;
typedef vector<ll> VI;

const DOUBLE EPS = 1e-9;

struct LPSolver {
  ll m, n;
  VI B, N;
  VVD D;

  LPSolver(const VVD &A, const VD &b, const VD &c) :
    m(b.size()), n(c.size()), N(n+1), B(m), D(m+2, VD(n+2)) {
    for (ll i = 0; i < m; i++) for (ll j = 0; j < n; j++) D[i][j] = A[i][j];
    for (ll i = 0; i < m; i++) { B[i] = n+i; D[i][n] = -1; D[i][n+1] = b[i]; }
    for (ll j = 0; j < n; j++) { N[j] = j; D[m][j] = -c[j]; }
    N[n] = -1; D[m+1][n] = 1;
  }

  void Pivot(ll r, ll s) {
    DOUBLE inv = 1.0 / D[r][s];
    for (ll i = 0; i < m+2; i++) if (i != r)
      for (ll j = 0; j < n+2; j++) if (j != s)
        D[i][j] -= D[r][j] * D[i][s] * inv;
    for (ll j = 0; j < n+2; j++) if (j != s) D[r][j] *= inv;
    for (ll i = 0; i < m+2; i++) if (i != r) D[i][s] *= -inv;
    D[r][s] = inv;
    swap(B[r], N[s]);
  }

  bool Simplex(ll phase) {
    ll x = phase == 1 ? m+1 : m;
    while (true) {
      ll s = -1;
      for (ll j = 0; j <= n; j++) {
        if (phase == 2 && N[j] == -1) continue;
        if (s == -1 || D[x][j] < D[x][s] || D[x][j] == D[x][s] && N[j] < N[s]) s = j;
      }
      if (s < 0 || D[x][s] > -EPS) return true;
      ll r = -1;
      for (ll i = 0; i < m; i++) {
        if (D[i][s] < EPS) continue;
        if (r == -1 || D[i][n+1] / D[i][s] < D[r][n+1] / D[r][s] ||
            D[i][n+1] / D[i][s] == D[r][n+1] / D[r][s] && B[i] < B[r]) r = i;
      }
      if (r == -1) return false;
      Pivot(r, s);
    }
  }

  DOUBLE Solve(VD &x) {
    ll r = 0;
    for (ll i = 1; i < m; i++) if (D[i][n+1] < D[r][n+1]) r = i;
    if (D[r][n+1] <= -EPS) {
      Pivot(r, n);
      if (!Simplex(1) || D[m+1][n+1] < -EPS) return -numeric_limits<DOUBLE>::infinity();
      for (ll i = 0; i < m; i++) if (B[i] == -1) {
        ll s = -1;
        for (ll j = 0; j <= n; j++)
          if (s == -1 || D[i][j] < D[i][s] || D[i][j] == D[i][s] && N[j] < N[s]) s = j;
        Pivot(i, s);
      }
    }
    if (!Simplex(2)) return numeric_limits<DOUBLE>::infinity();
    x = VD(n);
    for (ll i = 0; i < m; i++) if (B[i] < n) x[B[i]] = D[i][n+1];
    return D[m][n+1];
  }
};

ll n,m,k;
vector<ll> clients, facilities;
vector<ll> r;

int getFacility(int j){return j;}
int getClientFacility(int i, int j){
	return m+i*m+j;
}


int main(){
	fast_cin();
	// freopen("input.in","r",stdin);
	// freopen("output.out","w",stdout);
	cin>>n>>m>>k;
	FOR(i,0,n) clients.pb(nextll());
	FOR(i,0,m) facilities.pb(nextll());
	FOR(i,0,n) r.pb(nextll());
  VVD eqn;
	VD intercept;
	VD target;
	int num_eqn = n*m+n+m+1;
	int num_var = n*m + m;
	eqn.resize(num_eqn);
	target.resize(num_var);
	intercept.resize(num_eqn);
	FOR(i,0,n){
		FOR(j,0,m){
			target[getClientFacility(i,j)] = -abs(clients[i] - facilities[j]);
		}
	}
	int cur=0;
	FOR(i,0,n){
		FOR(j,0,m){
			eqn[cur].resize(num_var);
			eqn[cur][getClientFacility(i,j)] = 1;
			eqn[cur][getFacility(j)] = -1;
			intercept[cur] = 0;
			cur++;
		}
	}
	FOR(j,0,m){
		eqn[cur].resize(num_var);
		eqn[cur][getFacility(j)] = 1;
		intercept[cur] = k;
	}
	cur++;
	FOR(i,0,n){
		eqn[cur].resize(num_var);
		FOR(j,0,m){
			eqn[cur][getClientFacility(i,j)] = -1;
			intercept[cur] = -r[i];
		}
		cur++;
	}
	FOR(j,0,m){
		eqn[cur].resize(num_var);
		eqn[cur][getFacility(j)] = 1;
		intercept[cur] = 1;
		cur++;
	}
	LPSolver prob(eqn, intercept, target);
	VD solution;
	cout<<-prob.Solve(solution)<<endl;
	return 0;
}