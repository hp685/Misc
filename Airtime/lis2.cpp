#include<iostream>
#include <vector>
#include<fstream>
#include<cstdio>
using namespace std;


void find_lis(vector<int> &a, vector<int> &b)
{
	vector<int> p(a.size());
	int u, v;

	if (a.empty()) return;

	b.push_back(0);

	for (size_t i = 1; i < a.size(); i++){

		if (a[b.back()] < a[i]){
			p[i] = b.back();
			b.push_back(i);
			continue;
		}
		//Binary search
		for (u = 0, v = b.size()-1; u < v;){
			int c = (u + v) / 2;
			if (a[b[c]] < a[i]) u=c+1; else v=c;
		}


		if (a[i] < a[b[u]]){
			if (u > 0) p[i] = b[u-1];
			b[u] = i;
		}
	}

	for (u = b.size(), v = b.back(); u--;v=p[v]){
		b[u] = v;
	}
}



int main()
{
	freopen("sequence","r",stdin);
	int n;
	vector<int> a;
	while(cin>>n){
		a.push_back(n);
	}
	vector<int> seq(a);
	vector<int> lis;
        find_lis(seq, lis);

	//Output
	for (size_t i = 0; i < lis.size(); i++)
		cout<<seq[lis[i]]<<" ";
	cout<<endl;

	return 0;
}
