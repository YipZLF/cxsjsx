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

class HeadQ{
    char type[10];
    int lifePoint;
    int totalWar;
    int warCnt[5];     //warrior count
    bool canMake;   //make whether can make warrior
    int makeList[5];//sequence of making warrior
    int pCurWar;    //current warrior index
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
    bool hasStopped(){
        return !canMake;
    }
};

int main(){
    freopen("wow_phase1.in","r",stdin);
    freopen("wow_phase1.out","w",stdout);
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
