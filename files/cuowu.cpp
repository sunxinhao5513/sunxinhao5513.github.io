#include<iostream>
using namespace std;
bool a[101][101];
bool check(int n){
	for(int i=1;i<=n;i++){
		int cnt1=0,cnt2=0;
		for(int j=1;j<=n;j++){
			if(a[i][j]==1){
				cnt1++;
			}
			if(a[j][i]==1){
				cnt2++;
			}
		}
		if(cnt1%2!=0||cnt2%2!=0){
			return false;
		}
	}
	return true;
}
int main(){
	int n;
	cin>>n;
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			cin>>a[i][j];
		}
	}
	if(check(n)){
		cout<<"OK"<<endl;
	}
	else{
		int flag=1;
		for(int i=1;i<=n;i++){
			for(int j=1;j<=n;j++){
				a[i][j]=!a[i][j];
				if(check(n)){
					cout<<i<<" "<<j<<endl;
					flag=0;
					break;
				}
				a[i][j]=!a[i][j];
			}
			if(flag==0){
				break;
			}
		}
		if(flag==1){
			cout<<"Corrupt"<<endl;
		}
	}
	return 0;
} 
