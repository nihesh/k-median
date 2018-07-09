#include<bits/stdc++.h>

using namespace std;

int power(int base, int exponent){
	if(exponent == 0){
		return 1;
	}
	else{
		int temp = power(base, exponent/2);
		return temp*temp*((exponent%2!=0)?base:1);
	}
}

// first argument is the dimension of the euclidean space n, and the second argument is the tolerance parameter k.
int n, k;
stack<int> data;

void generateClients(int num, int base_length, int zero_set){
	if(num==zero_set){
		data.push(0);
		generateClients(num+1, base_length, zero_set);
		data.pop();
		return;
	}
	if(num==n){
		stack<int> temp;
		while(data.size()){
			int val = data.top();
			data.pop();
			temp.push(val);
		}
		while(temp.size()){
			int val = temp.top();
			cout<<val*base_length<<" ";
			temp.pop();
			data.push(val);
		}
		cout<<endl;
		return;
	}
	data.push(-1);
	generateClients(num+1, base_length, zero_set);
	data.pop();
	data.push(1);
	generateClients(num+1, base_length, zero_set);
	data.pop();
}

int main(int argc, char* argv[]){
	if(argc!=3){
		cout<<"Incorrect usage of the test generation script. Try again"<<endl;
		return 0;
	}
	n=stoi(argv[1]);
	k=stoi(argv[2]);
	cout<<power(2,n)<<endl;
	generateClients(0,20,-1);
	cout<<power(2,n)+n*power(2,n-1)<<endl;
	generateClients(0,10,-1);
	for(int i=0;i<n;i++){
		generateClients(0,20,i);
	}
	cout<<n*power(2,n-1)<<endl;
	cout<<k<<endl;
	return 0;
}