/*CPP 中的前向声明只用于*/
#include<iostream>
#include<cstdio>
#include<cstring>
#include<iomanip>
using namespace std;
const int MAXN = 10000;

int initLife[5];
const char warType[5][10]={"dragon","ninja","iceman","lion","wolf"};
const int redList[5]={2,3,4,1,0};
const int blueList[5]={3,0,1,2,4};
const char weaponList[3][10]={"sword","bomb","arrow"};
/*
Warrior:
id, lifepoint,headQ

dragon: weapon,morale,
ninja: weapon_1, weapon_2
iceman:weapon
lion:loyalty
wolf:
*/


class HeadQ;

class Warrior{
protected:
    int id,lifePoint;
    const HeadQ* headQ;
public:
    Warrior(int _id,int _lP,const HeadQ * _hq):
        id(_id),lifePoint(_lP),headQ(_hq){}
};

class dragon:public Warrior{
    char weapon[10];
    double morality;
    public:
    dragon(int _id,int _lP,const HeadQ * hq,int hqLifePoint):Warrior(_id,_lP,hq){
        strcpy(weapon,weaponList[id%3]);
        morality = (double)hqLifePoint / lifePoint;
        cout<<"It has a "<<weapon<<",and it's morale is ";
        cout<<fixed<<setprecision(2)<<morality<<endl;
    }
};
class ninja:public Warrior{
    char weapon_1[10];
    char weapon_2[10];
    public:
    ninja(int _id,int _lP,const HeadQ * hq):Warrior(_id,_lP,hq){
        strcpy(weapon_1,weaponList[id%3]);
        strcpy(weapon_2,weaponList[(id+1)%3]);
        cout<<"It has a "<<weapon_1<<" and a "<<weapon_2<<endl;
    }
};
class iceman:public Warrior{
    char weapon[10];
    public:
    iceman(int _id,int _lP,const HeadQ * hq):Warrior(_id,_lP,hq){
        strcpy(weapon,weaponList[id%3]);
        cout<<"It has a "<<weapon<<endl;
    }
};
class lion:public Warrior{
    int loyalty;
    public:
    lion(int _id,int _lP,const HeadQ * hq,int hqLifePoint):Warrior(_id,_lP,hq){
        loyalty = hqLifePoint;
        cout<<"It's loyalty is "<<loyalty<<endl;
    }
};
class wolf:public Warrior{
    public:
    wolf(int _id,int _lP,const HeadQ * hq):Warrior(_id,_lP,hq){}
};

class HeadQ{
    char type[10];
    int lifePoint;
    int totalWar;
    int warCnt[5];     //warrior count
    bool canMake;   //make whether can make warrior
    int makeList[5];//sequence of making warrior
    int pCurWar;    //current warrior index
    Warrior * warList[100];//warList[i] points to warrior with id i
public:
    HeadQ(int M,const char * tt){
        strcpy(type,tt);lifePoint = M;
        memset(warCnt, 0,sizeof(warCnt));
        totalWar = 0; 
        canMake = true;
        pCurWar = 0;
        if(tt[0] == 'r'){
            memcpy(makeList,redList,sizeof(redList));
        }else if(tt[0]=='b'){
            memcpy(makeList,blueList,sizeof(blueList));
        }
    }
    bool makeWarrior(){
        bool flag = false;
        for(int i = 0 ; i < sizeof(makeList)/sizeof(int);++i){
            if(lifePoint>=initLife[makeList[(pCurWar+i)%5]]){
                
                lifePoint -= initLife[makeList[(pCurWar+i)%5]];
                totalWar ++;
                warCnt[makeList[(pCurWar+i)%5]] ++;

                //print
                cout<<type<<' '<<warType[makeList[(pCurWar+i)%5]]<<' '<<totalWar;
                cout<<" born with strength "<<initLife[makeList[(pCurWar+i)%5]];
                cout<<","<<warCnt[makeList[(pCurWar+i)%5]]<<" "<<warType[makeList[(pCurWar+i)%5]];
                cout<<" in "<<type<<" headquarter"<<endl;
                switch(warType[makeList[(pCurWar+i)%5]][0]){
                    case 'd': {
                        this->warList[totalWar] = new dragon(totalWar,initLife[makeList[(pCurWar+i)%5]],this,lifePoint);
                        break;
                    }
                    case 'n':{
                        this->warList[totalWar] = new ninja(totalWar,initLife[makeList[(pCurWar+i)%5]],this);
                        break;
                    }
                    case 'i':{
                        this->warList[totalWar] = new iceman(totalWar,initLife[makeList[(pCurWar+i)%5]],this);
                        break;
                    }
                    case 'l':{
                        this->warList[totalWar] = new lion(totalWar,initLife[makeList[(pCurWar+i)%5]],this,lifePoint);
                        break;
                    }
                    case 'w':{
                        this->warList[totalWar] = new wolf(totalWar,initLife[makeList[(pCurWar+i)%5]],this);
                        break;
                    }
                    default: break;
                }

                pCurWar = (pCurWar+i+1)%5;
                flag = true;
                break;
            }
        }
        if(!flag){//can't make anymore
            canMake = false;
            //print info
            cout<<type<< " headquarter stops making warriors"<<endl;
        } 
        return flag;
    }
    bool hasStopped()const{
        return !canMake;
    }
    int getLifePoint() const{
        return lifePoint;
    }
};

int main(){
    freopen("wow_phase2.in","r",stdin);
   // freopen("wow_phase1.out","w",stdout);
    int T,M;
    scanf("%d",&T);
    for(int casenum = 1 ; casenum <= T; casenum++){
        scanf("%d",&M);
        HeadQ redHQ(M,"red"),blueHQ(M,"blue");
        for(int i = 0 ;i  < 5; ++i){
            cin>>initLife[i];
        }
        cout<<"Case:"<<casenum<<endl;
        for(int tt = 0 ; !(redHQ.hasStopped()&&blueHQ.hasStopped() );tt++){
            if(!redHQ.hasStopped()){
                cout<<setfill('0')<<setw(3)<<tt<<' ';
                redHQ.makeWarrior();
            }
            if(!blueHQ.hasStopped()){
                cout<<setfill('0')<<setw(3)<<tt<<' ';
                blueHQ.makeWarrior();
            }
        }
    }

}
