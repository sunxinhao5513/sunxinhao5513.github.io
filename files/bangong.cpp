#include<iostream>
using namespace std;
int main(){
	int t,m;
	cin>>t>>m;
	int a[101],b[101];
	for(int i=1;i<=t;i++){
		cin>>a[i]>>b[i];
	}
	int dp[101][2];
	dp[1][0]=a[1];
	dp[1][1]=b[1];
	for(int i=2;i<=t;i++){
		dp[i][0]=max(dp[i-1][0]+a[i],dp[i-1][1]-m+a[i]);
		dp[i][1]=max(dp[i-1][1]+b[i],dp[i-1][0]-m+b[i]);
	}
	cout<<max(dp[t][0],dp[t][1])<<endl;
	return 0;
}
