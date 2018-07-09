#include<bits/stdc++.h>

using namespace std;

struct point{
	int x,y;
};

int number_of_clients;
int number_of_facilities;
vector<point> facilities;
vector<point> clients;
int facilities_to_open;
int tolerance;
bool bit_set[100];
vector<double> results;

double distance_btw_points(point a, point b){
	return sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));
}

double compute_cost(vector<point> data){
	double cost = 0;
	for(int i=0;i<clients.size();i++){
		vector<double> d;
		for(int j=0;j<data.size();j++){
			d.push_back(distance_btw_points(data[j], clients[i]));
		}
		sort(d.begin(), d.end());
		for(int j=0;j<min(tolerance, facilities_to_open);j++){
			cost+=d[j];
		}
	}
	return cost;
}

vector<point> local_search(vector<point> data, vector<point> rest, double cur_cost){
	for(int i=0;i<data.size();i++){
		for(int j=0;j<rest.size();j++){
			point temp = data[i];
			data[i] = rest[j];
			rest[j] = temp;
			double cost = compute_cost(data);
			if(cost < cur_cost){
				return local_search(data, rest, cost);
			}
			temp = data[i];
			data[i] = rest[j];
			rest[j] = temp;
		}
	}
	return data;
}

bool flag=false;

void SubsetsOfSizeK(int cur_index, int facilities_left_to_open){
	if(cur_index == number_of_facilities){
		if(facilities_left_to_open==0){
			vector<point> data;
			vector<point> rest;
			for(int i=0;i<number_of_facilities;i++){
				if(bit_set[i]){
					data.push_back(facilities[i]);
				}
				else{
					rest.push_back(facilities[i]);
				}
			}
			if(!flag && compute_cost(local_search(data, rest, 1000000000))>53){
				for(int i=0;i<data.size();i++){
					cout<<data[i].x<<" "<<data[i].y<<endl;
				}
				flag = true;
			}
			results.push_back(compute_cost(local_search(data, rest, 1000000000)));
		}
	}
	else{
		bit_set[cur_index] = true;
		SubsetsOfSizeK(cur_index+1, facilities_left_to_open-1);
		bit_set[cur_index] = false;
		SubsetsOfSizeK(cur_index+1, facilities_left_to_open);
	}
}

int main(){

	cerr<<"Number of clients: ";
	cin>>number_of_clients;	

	cerr<<"Client locations (n entries): x y"<<endl;
	for(int i=0;i<number_of_clients;i++){
		point temp;
		cin>>temp.x>>temp.y;
		clients.push_back(temp);
	}

	cerr<<"Number of facilities: ";
	cin>>number_of_facilities;

	cerr<<"Facility locations (m entries): x y"<<endl;
	for(int i=0;i<number_of_facilities;i++){
		point temp;
		cin>>temp.x>>temp.y;
		facilities.push_back(temp);
	}

	cerr<<"Number of facilities to be opened: ";
	cin>>facilities_to_open;

	cerr<<"Tolerance for each client: ";
	cin>>tolerance;

	memset(bit_set, false, sizeof(bit_set));

	SubsetsOfSizeK(0, facilities_to_open);

	sort(results.begin(), results.end());
	map<int,int> count;
	for(int i=0;i<results.size();i++){
		count[results[i]]=1;
	}
	cout<<"Optimal answer: "<<results[0]<<endl;
	cout<<"Max answer: "<<results[results.size()-1]<<endl;
	cout<<"Approximation ratio: "<<(double)results[results.size()-1]/(double)results[0]<<endl;
	cout<<"Number of Minimas: "<<count.size()<<endl;
}