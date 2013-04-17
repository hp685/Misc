#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
using namespace std;


int lis(vector<long long>& a){

	vector<long long> q(a.size());
	long long mx;
	for(int k = 0; k < a.size(); k++){
		mx = 0;
		for(int j = 0; j < k; j++){
			if(a[k] > a[j]){
				mx = max(mx,q[j]);

			}
		}
		q[k] = mx + 1;
	}
	mx = 0;
	for(int i = 0; i < a.size(); i++){
		mx = max(mx,q[i]);
	}
	return mx;

}

int main(){

	freopen("sequence","r",stdin);

	vector<long long> a;
	int n,N;

	//for(int i = 0; i < N; i++){
	while(cin>>n){
		a.push_back(n);
	}


	//cout<<"Size of LIS: "<< lis(a)<<endl;
	cout<<lis(a)<<endl;
	return 0;
}
