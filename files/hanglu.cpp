#include<iostream>
using namespace std;
int dis[18][18];
int visited[18]={0};
int cursum=1000000000;
int state[1<<14][16]; 
int cur=0;
int n;
int sum;
void rev(int s,int cnt){
	//visited[s]=1;
	if(cnt==n-2){
		if(cursum>dis[s][n]+sum){
			cursum=dis[s][n]+sum;
		}
		return;
	}
	int minimum=10000;
	for(int i=1;i<n;i++){
		if(visited[i]==1||i==s){
			continue;
		}
		else{
			cout<<"*";
			if(state[cur][i]<sum+dis[s][i])continue;
			if(sum+dis[s][i]>cursum)continue;
			visited[s]=1;
			cur=cur|(1<<(i-2));
			sum+=dis[s][i];
			state[cur][i]=sum;
			rev(i,cnt+1);
			visited[s]=0;
			cur=cur^(1<<(i-2));
			sum-=dis[s][i];
			//cout<<minimum<<endl;
		}
	}
	//visited[s]=0;
	return;
}
int main(){
	scanf("%d",&n);
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			scanf("%d",&dis[i][j]);
			//cin>>dis[i][j];
		}
	}
	for(int i=0;i<=1<<14;i++){
		for(int j=0;j<=16;j++){
			state[i][j]=1<<30;
		}
	}
	rev(1,0);
	printf("%d",cursum);
	//cout<<rev(n,1,0)<<endl;
	return 0;
}
