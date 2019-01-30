/*input
5 5 4 3
2 10 4 1 6 
2 0 6 10 5
*/

// Nihesh Anderson - knandy

#include<bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 

using namespace __gnu_pbds;
using namespace std;

typedef long long ll;
typedef tree<ll,null_type,less<ll>,rb_tree_tag,tree_order_statistics_node_update> ordered_set; // find_by_order, order_of_key
ll MOD=100000000000000007;

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
double EPS = 1e-9;
ll nextll(){ll inp; cin>>inp; return inp;}
string nextStr(){string inp; cin>>inp; return inp;}
double nextDouble(){double inp; cin>>inp; return inp;}
ll power(ll a, ll n, ll md){if(n==0){return 1;}else{ll res=power(a,n/2,md);res=(res*res)%md;if(n%2!=0){res=(res*a)%md;}return res;}}
random_device rndm;
mt19937 grndm(rndm());
void mix(ll* a, ll* b){shuffle(a,b,grndm);}

vector<ll> clients;
vector<ll> facilities;
ll n,m;
vector<ll> choose;
 
ll getCost(ll r){
	ll cost = 0;
	FOR(i,0,n){
		vector<ll> data;
		FOR(j,0,choose.size()){
			data.pb(abs(clients[i]-choose[j]));
		}
		sort(data.begin(),data.end());
		FOR(j,0,r) cost+=data[j];
	}
	return cost;
} 

ll kmedian(ll k, ll r, bool toRemove){
	ll SZ = power(2,m,MOD);
	ll mincost = INF;
	FOR(i,0,SZ){
		bitset<50> b(i);	
		if(b.count() != k) continue;
		ll now = 0;
		FOR(j,0,n){
			vector<ll> cost;
			FOR(l,0,50){
				if(b[l]){
					cost.pb(abs(clients[j]-facilities[l]));
				}
			}
			sort(cost.begin(),cost.end());
			FOR(l,0,r) now+=cost[l];
		}
		mincost = min(now,mincost);
	}
	if(toRemove){
		FOR(i,0,SZ){
			bitset<50> b(i);
			if(b.count() != k) continue;
			ll now = 0;
			FOR(j,0,n){
				vector<ll> cost;
				FOR(l,0,50){
					if(b[l]){
						cost.pb(abs(clients[j]-facilities[l]));
					}
				}
				sort(cost.begin(),cost.end());
				FOR(l,0,r) now+=cost[l];
			}
			if(now == mincost){
				cout<<"Requirements ";
				FORR(j,49,0){
					if(b[j]){
						cout<<facilities[j]<<" ";
						choose.pb(facilities[j]);
						facilities.erase(facilities.begin()+j);
					}
				}
				cout<<endl;
				m = facilities.size();
				return mincost;
			}
		}	
	}
	return mincost;
}

int main(){
	fast_cin();
	// freopen("input.in","r",stdin);
	// freopen("output.out","w",stdout);
	ll k,r;
	cin>>n>>m>>k>>r;
	vector<ll> ff;
	FOR(i,0,n) clients.pb(nextll());
	FOR(i,0,m) facilities.pb(nextll());
	ff = facilities;
	ll mincost = kmedian(k,r,false);
	ll alpha = (k+r-1)/r;
	ll cnt = k-(r*(alpha-1));
	kmedian(alpha-1,1,true);
	kmedian(alpha-1,1,true);
	kmedian(alpha,1,true);
	if(mincost == getCost(r)){
		cout<<"Theorem holds"<<endl;
	}
	else{
		cout<<"Theorem failed"<<endl;
	}
	return 0;
}