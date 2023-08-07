#include<iostream>
#include<iomanip>
#include<string.h>
using namespace std;
bool prot[501][501]={0};
char map[501][501];
void protect(int i,int j,int r,int c){
	prot[i][j]=1;
	if(i-1>=1&&map[i-1][j]=='O'&&prot[i-1][j]==0){
		protect(i-1,j,r,c);
	}
	if(i+1<=r&&map[i+1][j]=='O'&&prot[i-1][j]==0){
		protect(i+1,j,r,c);
	}
	if(j-1>=1&&map[i][j-1]=='O'&&prot[i-1][j]==0){
		protect(i,j-1,r,c);
	}
	if(j+1<=c&&map[i][j+1]=='O'&&prot[i-1][j]==0){
		protect(i,j+1,r,c);
	}
}
int main(){
	int t;
	cin>>t;
	for(int i=1;i<=t;i++){
		int r,c;
		cin>>r>>c;
		memset(prot,0,sizeof(prot));
		for(int j=1;j<=r;j++){
			for(int k=1;k<=c;k++){
				cin>>map[j][k];
			}
		}
		for(int j=1;j<=c;j++){
			if(map[1][j]=='O'){
				protect(1,j,r,c);
			}
			if(map[r][j]=='O'){
				protect(r,j,r,c);
			}
		}
		for(int j=1;j<=r;j++){
			if(map[j][1]=='O'){
				protect(j,1,r,c);
			}
			if(map[j][c]=='O'){
				protect(j,c,r,c);
			}
		}
		for(int j=1;j<=r;j++){
			for(int k=1;k<=c;k++){
				if(prot[j][k]){
					cout<<"O";
				}
				else{
					cout<<"X";
				}
			}
			cout<<endl;
		}
	}
	return 0;
} 