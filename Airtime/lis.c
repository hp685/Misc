
// Returns the length of the longest increasing subsequence.
// Note that this is looking for the longest strictly increasing subsequence.
//    This can be easily modified for other situations.
#include<iostream>
#include<vector>
#include<stdio.h>
#include <limits.h>
#include<cstdlib>
#include<fstream>
using namespace std;
int lis( vector<int>& a, int N ) {
	int *best, i, j, max = INT_MIN;
	best = (int*) malloc ( sizeof( int ) * N );

	for ( i = 0; i < N; i++ ) best[i] = 1;

	for ( i = 1; i < N; i++ )
		for ( j = 0; j < i; j++ )
			if ( a[i] > a[j] && best[i] < best[j] + 1 ){
				best[i] = best[j] + 1;
				if(max < best[i])
					max = best[i];
			}

	free( best );
	return max;
}
// Sample usage.
int main(){
	freopen("sequence","r",stdin);
	vector<int> b;
	int n;
	while(cin>>n){
		b.push_back(n);
	}
	// the longest increasing subsequence = 13456?
	// the length would be 5, as well lcs(b,8) will return.
	printf("%d\n", lis( b, b.size() ) );
}
