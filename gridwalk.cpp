#include<iostream>
#include<vector>
#include<cmath>
#include<cstring>

using namespace std;

const int MAX = 1000;

int T[2*MAX][2*MAX];

int compute(int x){
	int d = 0, sum = 0;
while(x > 0){
		d = x % 10;
		x /= 10;
		sum += d;
	}
 return sum;

}

bool visit(int x, int y){
	if(T[MAX + x][MAX + y] == 1) return false;
	T[MAX + x][MAX + y] += 1;
	x = abs(x);
	y = abs(y);



	if(compute(x) + compute(y) <= 19)
		return true;
	return false;
}

int count(int x, int y ){

	static int cnt = 0;
	cnt++;
	if(visit(x+1,y)) count(x+1,y);
	if(visit(x-1,y)) count(x-1,y);
	if(visit(x,y+1)) count(x,y+1);
	if(visit(x,y-1)) count(x,y-1);


	return cnt;
}

int main(){

	memset(T,0,sizeof(T));

	cout<<count(0,0)-1<<endl;

	return 0;
}
