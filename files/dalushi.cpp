#include<iostream>
#include<queue>
using namespace std;
struct node{
    int a[16];
    int step;
    node(int* p,int n,int s){
        for(int i=1;i<=n;i++){
            a[i]=*(p+i);
        }
        step=s;
    }
};
bool check(node s,int n){
    for(int i=1;i<n;i++){
        if(s.a[i]>s.a[i+1]){
            return false;
        }
    }
    return true;
}

int main(){
    int t;
    cin>>t;
    for(int i=1;i<=t;i++){
        int n;
        cin>>n;
        queue<node> q;
        int init[16];
        for(int j=1;j<=n;j++){
            cin>>init[j];
        }
        node start(init,n,0);
        q.push(start);
        int ans=0;
        while(!q.empty()){
            node temp=q.front();
            if(check(temp,n)){
            	ans=temp.step;
                break;
            }
            //cout<<temp.step<<endl;
            if(temp.step>5)break;
            q.pop();
            if(getrevenum(temp,n)>5-temp.step){
            	cout<<getrevenum(temp,n)<<endl;
            	continue;
			}
            for(int j=1;j<n;j++){
                for(int k=j;k<n;k++){
                    for(int s=1;s<=n-k;s++){
                        int child[16];
                        for(int m=1;m<j;m++){
                            child[m]=temp.a[m];
                        }
                        for(int m=1;m<=s;m++){
                            child[j-1+m]=temp.a[k+m];
                        }
                        for(int m=1;m<=k-j+1;m++){
                            child[j+s+m-1]=temp.a[j+m-1];
                        }
                        for(int m=1;m<=n-k-s;m++){
                            child[k+s+m]=temp.a[k+s+m];
                        }
                        //for(int r=1;r<=n;r++)cout<<child[r]<<" ";
                    	//cout<<endl;
                        node temp1(child,n,temp.step+1);
                        //cout<<getnum(temp1,n)<<" "<<getnum(temp,n)<<endl;
                        if(getrevenum(temp1,n)<=getrevenum(temp,n)){
                        	//cout<<"%";
	                        q.push(temp1);
	                    }
                    }
                }
            }
        }
        if(ans!=0)
        cout<<"Case #"<<i<<": "<<ans<<endl;
        else{
            cout<<"Case #"<<i<<": Can not finish in 5 steps"<<endl;
        }
    }
    return 0;
}
