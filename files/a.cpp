#include <iostream>
#include <string>
#include<iomanip>
#include<cstring>
#include<algorithm>
using namespace std;
int t=0;
int N,K,R;
//weapon类，有5种。
class Weapon{
	public:
	string name;
	int power;
	Weapon(string s,int i){
		name=s;
		id=i;
		used=0;
	}
	Weapon(){}
	int id;
	int used;
};
//warrior类，有5种，每个warrior有name,life(生命)，attack（攻击力），id(编号)的特征，place标志他所在城市编号，color为其所属阵营。
class Warrior{
	public:
	string name;
	int life;
	int attack;
	int id;
	Warrior(string n,int color=0,int l=0,int a=1,int i=1){
		name=n;
		life=l;
		attack=a;
		id=i;
	}
	Warrior(){}
	void Setname(string n,string col,int lif,int att,int i){
		name=n;
		color=col;
		life=lif;
		attack=att;
		id=i;
		if(color=="red"){
			place=0;
		}
		else{
			place=N+1;
		}
	}
	int place;
	string color;
	int loyalty;
	double morale;
	//march_cnt用于计算武士前进次数，详见march_integrated（）
	int march_cnt=0;
	//每个武士每种武器最多1个
	Weapon *sword=NULL;
	Weapon *bomb=NULL;
	Weapon *arrow=NULL;
	bool hassword=0;
	bool hasarrow=0;
	bool hasbomb=0;
	//武士所有可能的行为
	void lion_run(){
		cout<<fixed<<setw(3)<<setfill('0')<<t<<":05 "<<color<<" lion "<<id<<" ran away"<<endl;

	}
	//抵达敌军司令部
	void takehead(string color1,string color2){
		cout<<fixed<<setw(3)<<setfill('0')<<t<<":10 "<<color1<<" "<<name<<" "<<id<<" reached "<<color2<<" headquarter with "<<life<<" elements and force "<<attack<<endl;
	}
	bool dead(){
		if(life<=0)return true;
		return false;
	}
	//报告
	void reportself(){
		cout<<fixed<<setw(3)<<setfill('0')<<t<<":55 "<<color<<" "<<name<<" "<<id<<" has ";
		if(hasarrow){
			cout<<"arrow("<<(3-arrow->used)<<")";
			if(hasbomb||hassword){
				cout<<",";
			}//报告结束就不需要输出“，”了
		}
		if(hasbomb){
			cout<<"bomb";
			if(hassword){
				cout<<",";
			}
		}
		if(hassword){
			cout<<"sword("<<sword->power<<")";
		}
		if(!hasarrow&&!hassword&&!hasbomb){
			cout<<"no weapon";
		}
		cout<<endl;
	}

	~Warrior(){
		delete sword;
		delete arrow;
		delete bomb;
	}
};
//用于在生成武士时配发武器。
void initweapon(Weapon w,Warrior* a){
	if(w.name=="sword"){
		a->sword=new Weapon("sword",0);
		a->sword->power=a->attack/5;
		a->hassword=1;
		if(a->sword->power==0)a->hassword=0;
	}
	if(w.name=="bomb"){
		a->bomb=new Weapon("bomb",1);
		a->hasbomb=1;
	}
	if(w.name=="arrow"){
		a->arrow=new Weapon("arrow",2);
		a->hasarrow=1;
	}
}
//用于wolf获胜后获取武器。
void getweapon(Warrior *a,Warrior *b){
		if(b->hasarrow&&!a->hasarrow){
			a->hasarrow=1;
			a->arrow=b->arrow;
		}
		if(b->hasbomb&&!a->hasbomb){
			a->hasbomb=1;
			a->bomb=b->bomb;
		}
		if(b->hassword&&!a->hassword){
			a->hassword=1;
			a->sword=b->sword;
		}
	}

	//a在第i个城市主动攻击b
void attacking(Warrior *a,Warrior *b,int i){
		if(a->hassword){
			b->life-=a->attack+a->sword->power;
			a->sword->power=a->sword->power*4/5;
			if(a->sword->power==0){
				a->hassword=0;
			}
		}
		else{
			b->life-=a->attack;
		}
		cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 "<<a->color<<" "<<a->name<<" "<<a->id<<" attacked "<<b->color<<" "<<b->name<<" "<<b->id<<" in city "<<i<<" with "<<a->life<<" elements and force "<<a->attack<<endl;
	}


	//a在第i个城市反击b
void fightback(Warrior *a,Warrior*b,int i){
	if(a->hassword){
		b->life-=a->attack/2+a->sword->power;
		a->sword->power=a->sword->power*4/5;
		if(a->sword->power==0){
				a->hassword=0;
			}
	}
	else{
		b->life-=a->attack/2;
	}
	cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 "<<a->color<<" "<<a->name<<" "<<a->id<<" fought back against "<<b->color<<" "<<b->name<<" "<<b->id<<" in city "<<i<<endl;
}

//a向b射箭
void arrow_attack(Warrior*a,Warrior*b){
	b->life-=R;
	a->arrow->used++;
	if(a->arrow->used==3){
		a->hasarrow=0;
	}
	cout<<fixed<<setw(3)<<setfill('0')<<t<<":35 "<<a->color<<" "<<a->name<<" "<<a->id<<" shot";
	if(b->life<=0){
		cout<<" and killed "<<b->color<<" "<<b->name<<" "<<b->id<<endl;
	}
	else{
		cout<<endl;
	}
}

//获取生命元的输出
void element_earn(int num,Warrior*p){
	cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 "<<p->color<<" "<<p->name<<" "<<p->id<<" earned "<<num<<" elements for his headquarter"<<endl;
	
}

//城市类
class City{
	public:
	int index;
	int element;
	string color="NULL";//旗子的颜色，最初没有插旗，为NULL
	//用于记录城市中红蓝累计胜利次数，未发生战斗数值不变，用于旗帜升起
	int red_win=0;
	int blue_win=0;
	//用于记录当前战斗胜利方，为1表示胜出，未发生战斗均为0，用于获取城市生命元和奖励发放
	int blue_wingame=0;
	int red_wingame=0;

	//用指针记录城市中的红蓝武士，没有为NULL
	Warrior *red_warrior_in_city;
	Warrior *blue_warrior_in_city;
	//用于武士前进时临时记录，详见march_integrated()
	Warrior *red_march_temp;
	Warrior *blue_march_temp;

	City(){
		red_warrior_in_city=NULL;
		blue_warrior_in_city=NULL;
		red_march_temp=NULL;
		blue_march_temp=NULL;
		element=0;
	}

	bool havewar(){
		return red_warrior_in_city!=NULL&&blue_warrior_in_city!=NULL;
	}
	//战斗，p1为red，p2为blue
	void war(Warrior *p1,Warrior* p2){
		//临时记录初始生命，用于lion死后生命元被获取
		int flifp1=p1->life;
		int flifp2=p2->life;
		bool red_first;//为1表示红武士先进攻，为0表示蓝武士先进攻
		if(color=="NULL"&&index%2==1||color=="red"){
			red_first=1;
			if(!p1->dead()&&!p2->dead()){
				attacking(p1,p2,index);
				if(p2->dead()){
					cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 "<<p2->color<<" "<<p2->name<<" "<<p2->id<<" was killed in city "<<index<<endl;
				}
				else{
					if(p2->name!="ninja")
					fightback(p2,p1,index);
					if(p1->dead()){
						cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 "<<p1->color<<" "<<p1->name<<" "<<p1->id<<" was killed in city "<<index<<endl;
						
					}
				}
			}
		}

		else{
			red_first=0;
			if(!p1->dead()&&!p2->dead()){
				attacking(p2,p1,index);
				if(p1->dead()){
					cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 "<<p1->color<<" "<<p1->name<<" "<<p1->id<<" was killed in city "<<index<<endl;
					
				}
				else{
					if(p1->name!="ninja")
					fightback(p1,p2,index);
					if(p2->dead()){
						cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 "<<p2->color<<" "<<p2->name<<" "<<p2->id<<" was killed in city "<<index<<endl;
					}
				}
			}
		}

		//战斗的副产物，士气增减
		if(!p1->dead()&&p1->name=="dragon"){
			if(p2->dead()){
				p1->morale+=0.2;
			}
			else{
				p1->morale-=0.2;
			}
		}
		if(!p2->dead()&&p2->name=="dragon"){
			if(p1->dead()){
				p2->morale+=0.2;
			}
			else{
				p2->morale-=0.2;
			}
		}
		if(p1->name=="lion"){
			if(p1->dead()&&flifp1>0){//lion被箭射死，可能战斗开始时生命元为负，不能加
				p2->life+=flifp1;
			}
			else if(!p2->dead()){
				p1->loyalty-=K;
			}
		}
		//lion生命元被对方获取
		if(p2->name=="lion"){
			if(p2->dead()&&flifp2>0){
				p1->life+=flifp2;
			}
			else if(!p1->dead()){
				p2->loyalty-=K;
			}
		}

		//wolf获取敌人武器
		if(!p1->dead()&&p1->name=="wolf"){
			if(p2->dead()){
				getweapon(p1,p2);
			}
		}
		if(!p2->dead()&&p2->name=="wolf"){
			if(p1->dead()){
				getweapon(p2,p1);
			}
		}

		//dragon欢呼
		if(red_first&&!p1->dead()&&p1->name=="dragon"&&p1->morale>0.8){
			cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 red dragon "<<p1->id<<" yelled in city "<<index<<endl;
		}
		if(!red_first&&!p2->dead()&&p2->name=="dragon"&&p2->morale>0.8){
			cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 blue dragon "<<p2->id<<" yelled in city "<<index<<endl;
		}

		//生命元获取
		if(p1->dead()&&!p2->dead()){
			blue_wingame=1;
			red_warrior_in_city=NULL;
			blue_win++;
			red_win=0;
			element_earn(element,p2);
		}
		else if(p2->dead()&&!p1->dead()){
			red_wingame=1;
			blue_warrior_in_city=NULL;
			red_win++;
			blue_win=0;
			element_earn(element,p1);
		}
		else if(!p1->dead()&&!p2->dead()){
			blue_win=0;
			red_win=0;
		}

		//旗帜升起
		if(blue_win==2){
			if(color!="blue"){
				cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 blue flag raised in city "<<index<<endl;
			}
			color="blue";
			blue_win=0;
		}
		if(red_win==2){
			if(color!="red"){
				cout<<fixed<<setw(3)<<setfill('0')<<t<<":40 red flag raised in city "<<index<<endl;
			}
			color="red";
			red_win=0;
		}
	}

	~City(){
		delete red_march_temp;
		delete blue_march_temp;
		delete red_warrior_in_city;
		delete blue_warrior_in_city;
	}
};
City city[30];
Weapon weapon[3]={Weapon("sword",0),Weapon("bomb",1),Weapon("arrow",2)};
Warrior warrior[5]={Warrior("dragon"),Warrior("ninja"),Warrior("iceman"),Warrior("lion"),Warrior("wolf")};

//司令部类，有红蓝两方
class RedHead:public City{
	public:
	int totallife;
	int numofwarrior[5];
	int sequence[5]={2,3,4,1,0};
	int temp=0;//记录此时应该生成的武士种类
	int occupied_times=0;//记录当前司令部中蓝武士数，为2则游戏结束
	Warrior*put_blue=NULL;//用于记录第一个抵达红司令部的蓝武士
	int redcnt=0;//记录编号数

	//生成武士，并配发武器
	void produce(){
		int type=sequence[temp];
		if(totallife>=warrior[type].life){
			totallife-=warrior[type].life;
				city[0].red_warrior_in_city=new Warrior;
				city[0].red_warrior_in_city->Setname(warrior[type].name,"red",warrior[type].life,warrior[type].attack,redcnt+1);
				redcnt++;

			cout<<fixed<<setw(3)<<setfill('0')<<t<<":00 "<<"red "<<warrior[type].name<<" "<<redcnt<<" born"<<endl;// with strength "<<warrior[type].life<<","<<numofwarrior[type]<<" "<<warrior[type].name<<" in "<<color<<" headquarter"<<endl;
			if(type!=4&&type!=3){
				initweapon(weapon[(redcnt)%3],city[0].red_warrior_in_city);
				if(type==1){
					initweapon(weapon[(redcnt+1)%3],city[0].red_warrior_in_city);
				}
			}
			if(type==3){
				city[0].red_warrior_in_city->loyalty=totallife;
				cout<<"Its loyalty is "<<totallife<<endl;
			}
			if(type==0){
				city[0].red_warrior_in_city->morale=(double)totallife/city[0].red_warrior_in_city->life;
				cout<<"Its morale is "<<fixed<<setprecision(2)<<city[0].red_warrior_in_city->morale<<endl;
			}
			temp++;
			temp=temp%5;
		}
	}
	//司令部报告生命元
	void report(){
		cout<<fixed<<setw(3)<<setfill('0')<<t<<":50 "<<totallife<<" elements in red headquarter"<<endl;
	}
	//司令部被占领
	void occupied(){
		cout<<fixed<<setw(3)<<setfill('0')<<t<<":10 red headquarter was taken"<<endl;
	}
	//司令部派发奖励，由近及远派发
	void reward(){
		for(int i=1;i<=N;i++){
			if(city[i].red_wingame>0){
				if(totallife>=8){
					//if(city[i].red_warrior_in_city==NULL)cout<<"error!"<<endl;
					city[i].red_warrior_in_city->life+=8;
					totallife-=8;
				}
				else{
					break;
				}
				red_wingame=0;
			}
		}
	}

	~RedHead(){
		delete put_blue;
	}
};
//蓝司令部，与红司令部基本相同
class BlueHead:public City{
	public:
	int totallife;
	int sequence[5]={3,0,1,2,4};
	int temp=0;
	int occupied_times=0;
	Warrior*put_red=NULL;
	int bluecnt=0;
	void produce(){
		int type=sequence[temp];
		if(totallife>=warrior[type].life){
			totallife-=warrior[type].life;
				city[N+1].blue_warrior_in_city=new Warrior;
				city[N+1].blue_warrior_in_city->Setname(warrior[type].name,"blue",warrior[type].life,warrior[type].attack,bluecnt+1);
				bluecnt++;


			cout<<fixed<<setw(3)<<setfill('0')<<t<<":00 "<<"blue "<<warrior[type].name<<" "<<bluecnt<<" born"<<endl;
			if(type!=4&&type!=3){
				initweapon(weapon[(bluecnt)%3],city[N+1].blue_warrior_in_city);
				if(type==1){
					initweapon(weapon[(bluecnt+1)%3],city[N+1].blue_warrior_in_city);
				}
			}
			if(type==3){
				city[N+1].blue_warrior_in_city->loyalty=totallife;
				cout<<"Its loyalty is "<<totallife<<endl;
			}
			if(type==0){
				city[N+1].blue_warrior_in_city->morale=(double)totallife/city[N+1].blue_warrior_in_city->life;
				cout<<"Its morale is "<<fixed<<setprecision(2)<<city[N+1].blue_warrior_in_city->morale<<endl;
			}
			temp++;
			temp=temp%5;
		}
	}
	void report(){
		cout<<fixed<<setw(3)<<setfill('0')<<t<<":50 "<<totallife<<" elements in blue headquarter"<<endl;
	}
	void occupied(){
		cout<<fixed<<setw(3)<<setfill('0')<<t<<":10 blue headquarter was taken"<<endl;
	}
	void reward(){
		for(int i=N;i>=1;i--){
			if(city[i].blue_wingame>0){
				if(totallife>=8){
					//if(city[i].blue_warrior_in_city==NULL)cout<<"blueerror"<<i<<endl;
					city[i].blue_warrior_in_city->life+=8;
					totallife-=8;
				}
				else{
					break;
				}
				blue_wingame=0;
			}
		}
	}

	~BlueHead(){
		delete put_red;
	}
};
//武士前进一格，所有武士前进逻辑详见march_integrated()
void march(Warrior *w){
		if(w->color=="red"){
			city[w->place+1].red_march_temp=w;
			w->place++;
		}
		else{
			city[w->place-1].blue_march_temp=w;
			w->place--;
		}
		if(w->name=="iceman"){
			if(w->march_cnt==1){
				w->life-=9;
				w->attack+=20;
				w->march_cnt=0;
				if(w->life<=0){
					w->life=1;
				}
			}
			else{
				w->march_cnt++;
			}
		}
	}

RedHead head1;
BlueHead head2;
int occupied_flag=0;//标志游戏结束

//a判断是否使用bomb
bool bombjudge(Warrior*a,Warrior*b,int i){
	if(a->color=="red"&&(city[i].color=="blue"||city[i].color=="NULL"&&city[i].index%2==0)||a->color=="blue"&&(city[i].color=="red"||city[i].color=="NULL"&&city[i].index%2==1)){
		if(b->hassword){
			if(a->life<=b->attack+b->sword->power){
				return true;
			}
		}
		else{
			if(a->life<=b->attack)
			return true;
		}
		return false;
	}
	else{
		if(a->hassword){
			if(b->life<=a->attack+a->sword->power)
			return false;
		}
		else{
			if(b->life<=a->attack)
			return false;
		}
		if(b->name=="ninja"){
			return false;
		}
		if(b->hassword){
			if(a->life<=b->attack/2+b->sword->power){
				return true;
			}
		}
		else{
			if(a->life<=b->attack/2){
				return true;
			}
		}
		return false;
	}
}

//所有武士的前进逻辑，每个城市的march-temp记录应该到达的武士指针
void march_integrated(){
	for(int j=0;j<=N+1;j++){
		if(city[j].red_warrior_in_city!=NULL)
			march(city[j].red_warrior_in_city);
		if(city[j].blue_warrior_in_city!=NULL)
			march(city[j].blue_warrior_in_city);
	}
	for(int j=0;j<=N+1;j++){
		city[j].red_warrior_in_city=city[j].red_march_temp;
		city[j].blue_warrior_in_city=city[j].blue_march_temp;
		city[j].red_march_temp=NULL;
		city[j].blue_march_temp=NULL;
	}

	//输出，并判断是否抵达司令部
	for(int j=0;j<=N+1;j++){
		if(city[j].red_warrior_in_city!=NULL){
			if(j!=N+1)
			cout<<fixed<<setw(3)<<setfill('0')<<t<<":10 red "<<city[j].red_warrior_in_city->name<<" "
			<<city[j].red_warrior_in_city->id<<" marched to city "<<j<<" with "<<city[j].red_warrior_in_city->life<<" elements and force "<<city[j].red_warrior_in_city->attack<<endl;
			else{
				city[N+1].red_warrior_in_city->takehead("red","blue");
				head2.occupied_times++;
				head2.put_red=city[N+1].red_warrior_in_city;
				city[N+1].red_warrior_in_city=NULL;
				if(head2.occupied_times==2){
					head2.occupied();
					occupied_flag=1;
				}
			}
		}
		if(city[j].blue_warrior_in_city!=NULL){
			if(j!=0)
			cout<<fixed<<setw(3)<<setfill('0')<<t<<":10 blue "<<city[j].blue_warrior_in_city->name<<" "
			<<city[j].blue_warrior_in_city->id<<" marched to city "<<j<<" with "<<city[j].blue_warrior_in_city->life<<" elements and force "<<city[j].blue_warrior_in_city->attack<<endl;
			else{
				city[0].blue_warrior_in_city->takehead("blue","red");
				head1.occupied_times++;
				head1.put_blue=city[0].blue_warrior_in_city;
				city[0].blue_warrior_in_city=NULL;
				if(head1.occupied_times==2){
					head1.occupied();
					occupied_flag=1;
				}
			}
		}
	}

}

//武士放箭
void arrow_integrated(){
	for(int i=1;i<=N;i++){
		if(i!=N&&city[i].red_warrior_in_city!=NULL&&city[i+1].blue_warrior_in_city!=NULL&&city[i].red_warrior_in_city->hasarrow){
			arrow_attack(city[i].red_warrior_in_city,city[i+1].blue_warrior_in_city);
		}
		if(i!=1&&city[i].blue_warrior_in_city!=NULL&&city[i-1].red_warrior_in_city!=NULL&&city[i].blue_warrior_in_city->hasarrow){
			arrow_attack(city[i].blue_warrior_in_city,city[i-1].red_warrior_in_city);
		}
	}
}
//武士使用bomb
void bomb_integrated(){
	for(int i=1;i<=N;i++){
		if(city[i].havewar()&&city[i].red_warrior_in_city->life>0&&city[i].blue_warrior_in_city->life>0){
			if(city[i].red_warrior_in_city->hasbomb){
				if(bombjudge(city[i].red_warrior_in_city,city[i].blue_warrior_in_city,i)){
					cout<<fixed<<setw(3)<<setfill('0')<<t<<":38 red "<<city[i].red_warrior_in_city->name<<" "<<city[i].red_warrior_in_city->id<<" used a bomb and killed blue "<<city[i].blue_warrior_in_city->name<<" "<<city[i].blue_warrior_in_city->id<<endl;
					city[i].red_warrior_in_city=NULL;
					city[i].blue_warrior_in_city=NULL;
					continue;
				}
			}
			if(city[i].blue_warrior_in_city->hasbomb){
				if(bombjudge(city[i].blue_warrior_in_city,city[i].red_warrior_in_city,i)){
					cout<<fixed<<setw(3)<<setfill('0')<<t<<":38 blue "<<city[i].blue_warrior_in_city->name<<" "<<city[i].blue_warrior_in_city->id<<" used a bomb and killed red "<<city[i].red_warrior_in_city->name<<" "<<city[i].red_warrior_in_city->id<<endl;
					city[i].red_warrior_in_city=NULL;
					city[i].blue_warrior_in_city=NULL;
				}
			}
		}
	}
}
//城市产生生命元
void elements_produce(){
	for(int i=1;i<=N;i++){
		city[i].element+=10;
	}
}

//武士获取生命元并转到司令部，f用于标志是否需要输出
void elements_get(bool f){
	for(int i=1;i<=N;i++){
		if(city[i].red_warrior_in_city!=NULL&&city[i].blue_warrior_in_city==NULL){
			head1.totallife+=city[i].element;
			if(f==0){
				cout<<fixed<<setw(3)<<setfill('0')<<t<<":30 red "<<city[i].red_warrior_in_city->name<<" "<<city[i].red_warrior_in_city->id<<" earned "<<city[i].element<<" elements for his headquarter"<<endl;
			}
			city[i].element=0;
		}
		else if(city[i].blue_warrior_in_city!=NULL&&city[i].red_warrior_in_city==NULL){
			head2.totallife+=city[i].element;
			if(f==0){
				cout<<fixed<<setw(3)<<setfill('0')<<t<<":30 blue "<<city[i].blue_warrior_in_city->name<<" "<<city[i].blue_warrior_in_city->id<<" earned "<<city[i].element<<" elements for his headquarter"<<endl;
			}
			city[i].element=0;
		}
	}
}

//--MAIN--
int main(){
	int s;
	cin>>s;
	for(int i=1;i<=s;i++){
		cout<<"Case "<<i<<":"<<endl;
		int M,T;
		cin>>M>>N>>R>>K>>T;
		head1.totallife=head2.totallife=M;
		for(int j=0;j<5;j++){
			int x;
			cin>>x;
			warrior[j].life=x;
		}
		for(int j=0;j<5;j++){
			int x;
			cin>>x;
			warrior[j].attack=x;
		}
		for(int j=0;j<=N+1;j++){
			city[j].index=j;
		}
		while(t*60<=T){
			//武士制造
			head1.produce();
			head2.produce();
			if(t*60+5>T)break;//处处判断是否截止
			//lion逃跑
			for(int j=0;j<=N+1;j++){
				if(city[j].red_warrior_in_city!=NULL){
					if(city[j].red_warrior_in_city->name=="lion"&&city[j].red_warrior_in_city->loyalty<=0){
						city[j].red_warrior_in_city->lion_run();
						city[j].red_warrior_in_city=NULL;
					}
				}
				if(city[j].blue_warrior_in_city!=NULL){
					if(city[j].blue_warrior_in_city->name=="lion"&&city[j].blue_warrior_in_city->loyalty<=0){
						city[j].blue_warrior_in_city->lion_run();
						city[j].blue_warrior_in_city=NULL;
					}
				}
			}

			if(t*60+10>T)break;
			//武士前进与生命元产生
			march_integrated();
			elements_produce();
			if(occupied_flag||t*60+30>T)break;//武士前进可能导致司令部被占领，此时停止输出
			elements_get(0);//只有一个武士的城市中的武士获取生命元
			if(t*60+35>T)break;
			arrow_integrated();
			if(t*60+38>T)break;
			bomb_integrated();

			if(t*60+40>T)break;
			for(int j=1;j<=N;j++){
				city[j].red_wingame=0;
				city[j].blue_wingame=0;//每次初始化，仅判断当前战斗
				if(city[j].havewar()){
					city[j].war(city[j].red_warrior_in_city,city[j].blue_warrior_in_city);
				}
				if(city[j].red_warrior_in_city!=NULL&&city[j].red_warrior_in_city->life<=0){
					city[j].red_warrior_in_city=NULL;
				}
				if(city[j].blue_warrior_in_city!=NULL&&city[j].blue_warrior_in_city->life<=0){
					city[j].blue_warrior_in_city=NULL;
				}
				
			}
			head1.reward();
			head2.reward();
			elements_get(1);//获胜武士获取生命元

			if(t*60+50>T)break;
			head1.report();
			head2.report();

			if(t*60+55>T)break;
			for(int j=1;j<=N;j++){
				if(city[j].red_warrior_in_city!=NULL){
					city[j].red_warrior_in_city->reportself();
				}
			}
			if(head2.put_red!=NULL){
				head2.put_red->reportself();
			}
			if(head1.put_blue!=NULL){
				head1.put_blue->reportself();
			}//抵达敌军司令部的武士也要汇报

			for(int j=1;j<=N;j++){
				if(city[j].blue_warrior_in_city!=NULL){
					city[j].blue_warrior_in_city->reportself();
				}
			}//汇报顺序先红后蓝，自西向东
			t++;
		}
		//每个case的初始化操作
		for(int j=0;j<=N+1;j++){
			city[j].blue_warrior_in_city=NULL;
			city[j].red_warrior_in_city=NULL;
			city[j].element=0;
			city[j].color="NULL";
			city[j].red_win=0;
			city[j].blue_win=0;
			city[j].red_wingame=0;
			city[j].blue_wingame=0;
		}
		t=0;
		head1.temp=0;
		head2.temp=0;
		head1.put_blue=NULL;
		head2.put_red=NULL;
		occupied_flag=0;
		head1.occupied_times=0;
		head2.occupied_times=0;
		head1.redcnt=0;
		head2.bluecnt=0;
	}
	return 0;
}
