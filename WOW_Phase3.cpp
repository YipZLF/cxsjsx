#include<iostream>
#include<cstdio>
#include<cstring>
#include<iomanip>
#include<algorithm>
#define DEBUG
using namespace std;
const int MAXN = 10000;
int M,N,K,T;
int initLife[5],initAtt[5];
const char warType[5][10]={"dragon","ninja","iceman","lion","wolf"};
const int redList[5]={2,3,4,1,0};
const int blueList[5]={3,0,1,2,4};
const char weaponList[3][10]={"sword","bomb","arrow"};

class HeadQ;
class Warrior;
class City;

class Weapon{
protected:
    Warrior * owner;
public:
    char type[10];
    int id,statusCode;
    // statusCode = avail*100+id*10+isused*1
    // available: avail = 0
    // used: isused = 0

    Weapon(Warrior * ow):owner(ow){statusCode = 1;}
    int getID(){return id;}
    bool isUsed(){return !(statusCode % 2);}
    bool isAvail(){return !(statusCode / 100);}
    void invalidate(){statusCode = 100+id;}
    virtual void usedOnce(){}
};

class sword:public Weapon{
public:
    sword(Warrior * ow):Weapon(ow){
        strcpy(type,"sword");
        id = 0;
        statusCode += id*10;
    }
};
class bomb:public Weapon{
public:
    bomb(Warrior * ow):Weapon(ow){
        strcpy(type,"bomb");
        id = 1;
        statusCode += id*10;
    }
    void usedOnce(){
        statusCode += 100;// not available and is used
        statusCode = (statusCode / 10)*10;
    }
};
class arrow:public Weapon{
    int arrowCnt;
public:
    arrow(Warrior * ow):Weapon(ow){
        strcpy(type,"arrow");
        id = 2;
        statusCode += id*10;
        arrowCnt = 2;
    }
    void usedOnce(){
        arrowCnt--;
        if(!this->isUsed()){
            statusCode= (statusCode / 10)*10;
        }
        if(arrowCnt == 0){
            statusCode+=100;
        }
    }
};
 bool weaponCompare(Weapon *a, Weapon * b){  
        return a->statusCode < b->statusCode;
    }
class Warrior{
protected:
    char group[10];
    int id,lifePoint,Att;
    int locateCity;
    char type[10];
    
public:

    Weapon * weapon[10];
    int weaponListCnt[3];
    int weaponCnt;
    int curWeapon;
    
    Warrior(int _id,int _lP,int _Att,const char * gr):id(_id),lifePoint(_lP),Att(_Att){
        strcpy(group,gr);
        if(gr[0]=='r') locateCity = 0;
        else if(gr[0]=='b') locateCity = N+1;
        weaponCnt = 0;
        memset(weaponListCnt,0,sizeof(weaponListCnt));
        curWeapon = 0;
    }
    ~Warrior(){
        for(int i = 0 ;i < weaponCnt;++i){
            delete weapon[i];
        }
    }
    
    virtual void marchOn(int idx){
        locateCity = idx;
    }
    friend bool weaponCompare(Weapon * a, Weapon * b);
    void sortWeapon(){

        int cnt = 0;
        for(int i =0 ; i < weaponCnt ;i++){
            if(weapon[i]->isAvail())
                cnt++;
        }
        sort(weapon,weapon+weaponCnt,&weaponCompare);
        for(int i = cnt;i<weaponCnt;i++){
            delete weapon[i];
        }
        weaponCnt = cnt;
        countWeapon();
    }
    
    virtual bool attackEnemy(Warrior & enemy){// return whether weapon status changed
        bool weaponStatusChanged = false;
        for(int i = 0 ; i < weaponCnt;++i){
            if(weapon[(i+curWeapon)%weaponCnt]->isAvail()){
                //attack
                Weapon * wpPtr = weapon[(i+curWeapon)%weaponCnt];
                int weaponID = wpPtr->getID();
                switch(weaponID){
                    case 0:{//swords
                        enemy.getHurt((int)(Att * 0.2));
                        bool moreWeaponToUsed = false;
                        
                        for(int j = 1;j<weaponCnt;j++){
                            if(weapon[(j+i+curWeapon)%weaponCnt]->type[0]!='s' &&
                                weapon[(j+i+curWeapon)%weaponCnt]->isAvail())
                                moreWeaponToUsed = true;
                        }
                        if(moreWeaponToUsed)
                            weaponStatusChanged=true;

                        break;
                    }
                    case 1:{//bomb
                        this->getHurt((int)((int)(Att * 0.4)*0.5));
                        enemy.getHurt((int)(Att * 0.4));
                        weaponStatusChanged = true;
                        wpPtr->usedOnce();
                        break;
                    }
                    case 2:{//arrow
                        enemy.getHurt((int)(Att * 0.3));
                        weaponStatusChanged = true;
                        wpPtr->usedOnce();
                        break;
                    }
                    default: break;
                }
                curWeapon = (curWeapon+i+1)%weaponCnt;
                break;
            }
        }
        return weaponStatusChanged;
    }
    void getHurt(int injury){
        lifePoint-=injury;
    }
    bool isDead(){
        return lifePoint<=0;
    }
    void takeWeapon(Warrior & enemy){
        this->sortWeapon();// take new arrows first
        enemy.sortWeapon();
        int i;
        for(i = 0 ; i < enemy.weaponCnt && enemy.weapon[i]->id != 2;++i){
            if(weaponCnt >= 10) break;
            weapon[weaponCnt++] = enemy.weapon[i];
            enemy.weapon[i]=new Weapon(&enemy);
            enemy.weapon[i]->invalidate();
        }
        for(int j = enemy.weaponCnt-1; j >= i && enemy.weapon[j]->id == 2;j--){
            if(weaponCnt >= 10) break;
            weapon[weaponCnt++] = enemy.weapon[j];
            enemy.weapon[j]=new Weapon(&enemy);
            enemy.weapon[j]->invalidate();
        }
        countWeapon();
    }
    char *getType(){
        return type;
    }
    char * getGroup(){
        return group;
    }
    int getID(){
        return id;
    }
    int getElements(){
        return lifePoint;
    }
    int getForce(){
        return Att;
    }
    int getCity(){
        return locateCity;
    }
    void countWeapon(){
        memset(weaponListCnt,0,sizeof(weaponListCnt));
        for(int i = 0 ;i < weaponCnt;i++){
            if(weapon[i]->isAvail())
                weaponListCnt[weapon[i]->id] ++;
        }
    }
    void report(int hour,int min){
        countWeapon();
        cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
        cout<<group<<' '<<type<<' '<<id<<" has ";
        cout<<weaponListCnt[0]<<" sword ";
        cout<<weaponListCnt[1]<<" bomb ";
        cout<<weaponListCnt[2]<<" arrow ";
        cout<<"and "<<lifePoint<< " elements"<<endl;
    }
    virtual int getLoyalty(){}
    virtual bool runAway(){}
    virtual bool robWeapon(Warrior & enemy,int hour,int min){}
    virtual void cheers(int hour, int min){}
};

class dragon:public Warrior{
public:
    dragon(int _id,int _lP,int _Att,const char * gr):Warrior(_id,_lP,_Att,gr){
        strcpy(type,"dragon");
        switch(weaponList[id%3][0]){
            case 's': {
                weapon[0] = new sword(this);
                weaponListCnt[0]++;break;
                }
            case 'b':{
                weapon[0] = new bomb(this);
                weaponListCnt[1]++;
                break;
            } 
            case 'a': {
                weapon[0] = new arrow(this);
                weaponListCnt[2]++;
                break;
            }
            default: break;
        }
        weaponCnt = 1;
    }
    void cheers(int hour,int min){
        //print
        cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
        cout<<group<<' '<<type<<' '<<id<<" yelled in city "<<locateCity<<endl;
    }
};
class ninja:public Warrior{
    public:
    ninja(int _id,int _lP,int _Att,const char * gr):Warrior(_id,_lP,_Att,gr){
        strcpy(type,"ninja");
        switch(weaponList[id%3][0]){
            case 's': {
                weapon[0] = new sword(this);
                weapon[1] = new bomb(this);
                weaponListCnt[0]++;
                weaponListCnt[1]++;
                break;
            }
            case 'b': {
                weapon[0] = new bomb(this);
                weapon[1] = new arrow(this);
                weaponListCnt[2]++;
                weaponListCnt[1]++;
                break;
            }
            case 'a': {
                weapon[0] = new arrow(this);
                weapon[1] = new sword(this);
                weaponListCnt[0]++;
                weaponListCnt[2]++;
                break;
            }
            default: break;
        }
        weaponCnt = 2;
    }
    /*escape from bomb*/
    bool attackEnemy(Warrior & enemy){// return whether weapon status changed
        bool weaponStatusChanged = false;
        for(int i = 0 ; i < weaponCnt;++i){
            if(weapon[(i+curWeapon)%weaponCnt]->isAvail()){
                //attack
                Weapon * wpPtr = weapon[(i+curWeapon)%weaponCnt];
                int weaponID = wpPtr->getID();
                switch(weaponID){
                    case 0:{//swords
                        enemy.getHurt((int)(Att * 0.2));
                        bool moreWeaponToUsed = false;

                        for(int j = 1;j<weaponCnt;j++){
                            if(weapon[(j+i+curWeapon)%weaponCnt]->type[0]!='s' &&
                                weapon[(j+i+curWeapon)%weaponCnt]->isAvail())
                                moreWeaponToUsed = true;
                        }
                        if(moreWeaponToUsed)
                            weaponStatusChanged=true;

                        break;
                    }
                    case 1:{//bomb
                        enemy.getHurt((int)(Att * 0.4));
                        weaponStatusChanged = true;
                        wpPtr->usedOnce();
                        break;
                    }
                    case 2:{//arrow
                        enemy.getHurt((int)(Att * 0.3));
                        weaponStatusChanged = true;
                        wpPtr->usedOnce();
                        break;
                    }
                    default: break;
                }
                curWeapon = (curWeapon+i+1)%weaponCnt;
                break;
            }
        }
        return weaponStatusChanged;
    }
};
class iceman:public Warrior{
    public:
    iceman(int _id,int _lP,int _Att,const char * gr):Warrior(_id,_lP,_Att,gr){
        strcpy(type,"iceman");
        switch(weaponList[id%3][0]){
            case 's': {
                weapon[0] = new sword(this);
                weaponListCnt[0]++;
                break;
                }
            case 'b':{
                weapon[0] = new bomb(this);
                weaponListCnt[1]++;
                break;
            } 
            case 'a': {
                weapon[0] = new arrow(this);
                weaponListCnt[2]++;
                break;
            }
            default: break;
        }
        weaponCnt = 1;
    }
    void marchOn(int idx){
        int dec = 0.1 * lifePoint;
        lifePoint-= dec;
        locateCity = idx;
    }
    /* once march on , lifepoint -= -.1*lifepoint*/
};
class lion:public Warrior{
    int loyalty;
    public:
    lion(int _id,int _lP,int _Att,const char * gr,int hqLifePoint):Warrior(_id,_lP,_Att,gr){
        strcpy(type,"lion");
        loyalty = hqLifePoint;
        switch(weaponList[id%3][0]){
            case 's': {
                weapon[0] = new sword(this);
                weaponListCnt[0]++;break;
                }
            case 'b':{
                weapon[0] = new bomb(this);
                weaponListCnt[1]++;
                break;
            } 
            case 'a': {
                weapon[0] = new arrow(this);
                weaponListCnt[2]++;
                break;
            }
            default: break;
        }
        weaponCnt = 1;
        //print
        cout<<"Its loyalty is "<<loyalty<<endl;
    }
    void marchOn(int idx){
        locateCity = idx;
        loyalty-=K;
    }
    int getLoyalty(){
        return loyalty;
    }
    bool runAway(){
        //print
        cout<<group<<" lion "<<id<<" ran away"<<endl;
    }
};
class wolf:public Warrior{
    public:
    wolf(int _id,int _lP,int _Att,const char * gr):Warrior(_id,_lP,_Att,gr){
        strcpy(type,"wolf");
        weaponCnt = 0;
    }
   /*rob others*/
    virtual bool robWeapon(Warrior & enemy,int hour,int min){
        if(enemy.getType()[0]=='w'){//if enemy is wolf too
            return false;
        }

        int robCnt = 0;
        enemy.sortWeapon();


        int minID = enemy.weapon[0]->id;
        if(minID !=2){

            for(int i = 0; i < enemy.weaponListCnt[minID] && this->weaponCnt<10 ;++i){
                this->weapon[weaponCnt++]=enemy.weapon[i];

                enemy.weapon[i] = new Weapon(&enemy);
                enemy.weapon[i]->invalidate();
                
                robCnt++;
            }
        }else{//take new arrow first
            for(int i = enemy.weaponCnt - 1;
                    i >= enemy.weaponListCnt[0]+enemy.weaponListCnt[1]&& this->weaponCnt<10;  --i){
                this->weapon[weaponCnt++]=enemy.weapon[i];

                enemy.weapon[i] = new Weapon(&enemy);
                enemy.weapon[i]->invalidate();
                
                robCnt++;
            }
        }
        countWeapon();
        //print
        if(robCnt ==0){
            return false;
        }
        cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
        cout<<group<<' '<<type<<' '<<id<<" took "<<robCnt<<' '<<weaponList[minID];
        cout<<" from "<<enemy.getGroup()<<' '<<enemy.getType()<<' '<<enemy.getID()<<" in city "<<locateCity<<endl;
        return true;
    }
};

#define NOT_OVER -1
#define DEAD 0
#define RED 1
#define BLUE 2
#define ALIVE 3
class City{
public:
    Warrior * redWarrior, * blueWarrior;
    int idx;
    int color;
    City(){
        redWarrior = blueWarrior = NULL;
        idx = -1;
    }
    bool ready4Battle(){
        return (redWarrior!=NULL)&&(blueWarrior!=NULL);
    }
    void Battle(int hour,int min){
        
        redWarrior->sortWeapon();
        blueWarrior->sortWeapon();    
        redWarrior->curWeapon = 0;
        blueWarrior->curWeapon = 0;
        int result= NOT_OVER;// -1: not over; 0: both dead; 1:red win; 2:blue win;3:both alive
        if(redWarrior->weaponCnt ==0 && blueWarrior->weaponCnt==0)
                result = ALIVE;
        while(result == NOT_OVER){
            bool hasChanged = false;
            bool elementsChanged = false;
            int _re = redWarrior->getElements();//red elements old
            int _be = blueWarrior->getElements();//blue elements old
            if(idx % 2 ==0){//blue first
                if(!blueWarrior->isDead() ){
                    hasChanged |= blueWarrior->attackEnemy(*redWarrior);
                }
                if(!redWarrior->isDead() ){
                    hasChanged |= redWarrior->attackEnemy(*blueWarrior);
                }
            }
            else{//red first
                if(!redWarrior->isDead() ){
                    hasChanged |= redWarrior->attackEnemy(*blueWarrior);
                }
                if(!blueWarrior->isDead() ){
                    hasChanged |= blueWarrior->attackEnemy(*redWarrior);
                }
            }

            if(redWarrior->getElements()!=_re || 
                blueWarrior->getElements()!=_be){
                    elementsChanged = true;
            }

            if( hasChanged == false &&
                elementsChanged== false && 
                !redWarrior->isDead() && 
                !blueWarrior->isDead()){// nothing will ever change, battle comes to a draw
                result = ALIVE;
                break;
            }
            if(redWarrior->isDead()&& blueWarrior->isDead()){
                result = DEAD;
                break;
            }
            if(redWarrior->isDead()){
                result = BLUE;
                break;
            }
            if(blueWarrior->isDead()){
                result = RED;
                break;
            }
        }
        if (result == BLUE) color= BLUE;
        else if(result == RED) color= RED;
        else color = 0;

        //print
        cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
        switch(result){
            case DEAD:{
                cout<<"both red "<<redWarrior->getType()<<' '<<redWarrior->getID()<<" and blue ";
                cout<<blueWarrior->getType()<<' '<<blueWarrior->getID()<<" died in city "<<idx<<endl;
                redWarrior = NULL;
                blueWarrior = NULL;
                break;
            }
            case RED:{
                cout<<"red "<<redWarrior->getType()<<' '<<redWarrior->getID()<<" killed blue ";
                cout<<blueWarrior->getType()<<' '<<blueWarrior->getID()<<" in city "<<idx;
                cout<<' '<<"remaining "<< redWarrior->getElements() <<" elements"<<endl;
                
                if(redWarrior->getType()[0]=='d'){//dragon yell
                    redWarrior->cheers(hour,min);
                }
                redWarrior->takeWeapon(*blueWarrior);
                blueWarrior = NULL;
                break;
            }
            case BLUE:{
                cout<<"blue "<<blueWarrior->getType()<<' '<<blueWarrior->getID()<<" killed red ";
                cout<<redWarrior->getType()<<' '<<redWarrior->getID()<<" in city "<<idx;
                cout<<' '<<"remaining "<<blueWarrior->getElements()<<" elements"<<endl;
                
                if(blueWarrior->getType()[0]=='d'){//dragon yell
                    blueWarrior->cheers(hour,min);
                }
                blueWarrior->takeWeapon(*redWarrior);
                redWarrior = NULL;
                break;
            }
            case ALIVE:{
                cout<<"both red "<<redWarrior->getType()<<' '<<redWarrior->getID()<<" and blue ";
                cout<<blueWarrior->getType()<<' '<<blueWarrior->getID()<<" were alive in city "<<idx<<endl;
                
                if(redWarrior->getType()[0]=='d'){//dragon yell
                    redWarrior->cheers(hour,min);
                }
                if(blueWarrior->getType()[0]=='d'){//dragon yell
                    blueWarrior->cheers(hour,min);
                }
                break;
            }
            default:break;
        }
    }
};

City cityList[22];


class HeadQ{
    char type[10];
    int lifePoint;
    int totalWar;
    bool canMake;   //make whether can make warrior
    int makeList[5];//sequence of making warrior
    int pCurWar;    //current warrior index
public:
    int warCnt[5];     //warrior count
    int curWarListIdx;
    Warrior * warList[100];//warList[i] points to warrior with id i

    HeadQ(int M,const char * tt){
        strcpy(type,tt);lifePoint = M;
        memset(warCnt, 0,sizeof(warCnt));
        memset(warList, 0,sizeof(warList));
        totalWar = 0; 
        curWarListIdx = 0;
        canMake = true;
        pCurWar = 0;
        if(tt[0] == 'r'){
            memcpy(makeList,redList,sizeof(redList));
        }else if(tt[0]=='b'){
            memcpy(makeList,blueList,sizeof(blueList));
        }
    }
    Warrior * makeWarrior(int hour,int min){
        bool flag = false;
        for(int i = 0 ; i < 1;++i){
            if(lifePoint>=initLife[makeList[(pCurWar+i)%5]]){
                flag = true;
                lifePoint -= initLife[makeList[(pCurWar+i)%5]];
                totalWar ++;
                curWarListIdx ++;
                warCnt[makeList[(pCurWar+i)%5]] ++;

                //print
                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                cout<<type<<' '<<warType[makeList[(pCurWar+i)%5]]<<' '<<totalWar<<" born"<<endl;
                switch(warType[makeList[(pCurWar+i)%5]][0]){
                    case 'd': {
                        this->warList[curWarListIdx] = new dragon(curWarListIdx,initLife[makeList[(pCurWar+i)%5]],initAtt[0],this->type);
                        break;
                    }
                    case 'n':{
                        this->warList[curWarListIdx] = new ninja(curWarListIdx,initLife[makeList[(pCurWar+i)%5]],initAtt[1],this->type);
                        break;
                    }
                    case 'i':{
                        this->warList[curWarListIdx] = new iceman(curWarListIdx,initLife[makeList[(pCurWar+i)%5]],initAtt[2],this->type);
                        break;
                    }
                    case 'l':{
                        this->warList[curWarListIdx] = new lion(curWarListIdx,initLife[makeList[(pCurWar+i)%5]],initAtt[3],this->type,lifePoint);
                        break;
                    }
                    case 'w':{
                        this->warList[curWarListIdx] = new wolf(curWarListIdx,initLife[makeList[(pCurWar+i)%5]],initAtt[4],this->type);
                        break;
                    }
                    default: break;
                }

                pCurWar = (pCurWar+i+1)%5;
                break;
            }
        }
        if(flag)
            return warList[curWarListIdx];
        else
            return NULL;
    }
    bool hasStopped()const{
        return !canMake;
    }
    int getLifePoint() const{
        return lifePoint;
    }
    void lionRunaway(int id){
        warCnt[3]--;
        totalWar--;
        delete warList[id];
        warList[id] = NULL;
    }
    void reportElements(int hour,int min){
        cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
        cout<<lifePoint<<" elements"<<" in "<<type<<" headquarter"<<endl;
    }
};

int main(){
    //freopen("WOW_Phase3.in","r",stdin);
    //freopen("wow_phase3.out","w",stdout);
    int casenum,caseCnt = 0;
    cin>>casenum;
    for(int caseCnt = 1 ; caseCnt <= casenum; caseCnt++){
        memset(cityList,0,sizeof(cityList));
        scanf("%d %d %d %d",&M,&N,&K,&T);
        HeadQ redHQ(M,"red"),blueHQ(M,"blue");
        for(int i = 0 ;i  < 5; ++i){
            cin>>initLife[i];
        }
        for(int i = 0 ;i  < 5; ++i){
            cin>>initAtt[i];
        }
        
        // start a case;
        cout<<"Case "<<caseCnt<<":"<<endl;
        for(int i = 0 ; i <= N+1;i++){
            cityList[i].idx = i;
        }

        int hour,min;
        int tt;
        bool isOver = false;
        for(tt = 0; tt<=T ;tt+=5){
            if(isOver)
                break;
            hour = tt/60; min = tt%60;
            switch(min){
                case 0:{//make warrior
                    if(!redHQ.hasStopped()){
                    cityList[0].redWarrior = redHQ.makeWarrior(hour,min);
                    }
                    if(!blueHQ.hasStopped()){
                    cityList[N+1].blueWarrior = blueHQ.makeWarrior(hour,min);
                    }
                    break;
                }
                
                case 5:{//lion runaway
                    for(int i = 0 ; i <=N+1;++i){
                        if(cityList[i].blueWarrior!= NULL &&cityList[i].blueWarrior->getType()[0]=='l'){//is lion
                            if(i!=N+1){    
                                if(cityList[i].blueWarrior->getLoyalty()<=0){
                                //print
                                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                                cityList[i].blueWarrior->runAway();
                                blueHQ.lionRunaway(cityList[i].blueWarrior->getID());
                                cityList[i].blueWarrior = NULL;
                                }
                            }
                        }
                        if(cityList[i].redWarrior!= NULL &&cityList[i].redWarrior->getType()[0]=='l'){//is lion
                            if(i!=0){
                                if(cityList[i].redWarrior->getLoyalty()<=0){
                                //print
                                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                                cityList[i].redWarrior->runAway();
                                redHQ.lionRunaway(cityList[i].redWarrior->getID());
                                cityList[i].redWarrior = NULL;
                                }
                            }
                        }
                    }
                    break;
                    }

                case 10:{//march on
                    for(int i = N; i >=0;--i)
                        cityList[i+1].redWarrior = cityList[i].redWarrior;
                    cityList[0].redWarrior = NULL;
                    for(int i = 0; i <N+1;++i)
                        cityList[i].blueWarrior = cityList[i+1].blueWarrior;
                    cityList[N+1].blueWarrior = NULL;
                    
                    for(int i = 0; i<=N+1; ++i){
                        if(i==N+1){
                            if(cityList[i].redWarrior!=NULL){// blue is taken down
                                cityList[i].redWarrior->marchOn(i);
                                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                                cout<<"red "<<cityList[i].redWarrior->getType()<<' '<<cityList[i].redWarrior->getID()<<' ';
                                cout<<"reached blue headquarter with "<<cityList[i].redWarrior->getElements()<<' ';
                                cout<<"elements and force "<<cityList[i].redWarrior->getForce()<<endl;
                                
                                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                                cout<<"blue headquarter was taken"<<endl;
                                isOver = true;
                            }
                        }else if(i==0){
                            if(cityList[i].blueWarrior!=NULL){// blue is taken down
                                cityList[i].blueWarrior->marchOn(i);
                                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                                cout<<"blue "<<cityList[i].blueWarrior->getType()<<' '<<cityList[i].blueWarrior->getID()<<' ';
                                cout<<"reached red headquarter with "<<cityList[i].blueWarrior->getElements()<<' ';
                                cout<<"elements and force "<<cityList[i].blueWarrior->getForce()<<endl;
                                
                                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                                cout<<"red headquarter was taken"<<endl;
                                isOver = true;
                            }
                        }else{
                            if(cityList[i].redWarrior!=NULL){
                                cityList[i].redWarrior->marchOn(i);
                                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                                cout<<"red "<<cityList[i].redWarrior->getType()<<' '<<cityList[i].redWarrior->getID()<<' ';
                                cout<<"marched to city "<<cityList[i].idx<<" with "<<cityList[i].redWarrior->getElements()<<' ';
                                cout<<"elements and force "<<cityList[i].redWarrior->getForce()<<endl;
                            }
                            if(cityList[i].blueWarrior!=NULL){
                                cityList[i].blueWarrior->marchOn(i);
                                cout<<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<min<<' ';
                                cout<<"blue "<<cityList[i].blueWarrior->getType()<<' '<<cityList[i].blueWarrior->getID()<<' ';
                                cout<<"marched to city "<<cityList[i].idx<<" with "<<cityList[i].blueWarrior->getElements()<<' ';
                                cout<<"elements and force "<<cityList[i].blueWarrior->getForce()<<endl;
                            }
                        }
                    }
                        
                    break;
                }
                case 35:{// wolves rob
                    for(int i = 1; i <N+1;++i){
                        if(cityList[i].ready4Battle()){
                            if(cityList[i].redWarrior->getType()[0]=='w'){
                                #ifdef DEBUG
                               // cout<<"red wolves rob"<<endl;
                                #endif
                                cityList[i].redWarrior->robWeapon(*cityList[i].blueWarrior,hour,min);
                            }
                            if(cityList[i].blueWarrior->getType()[0]=='w'){
                                #ifdef DEBUG
                               // cout<<"blue wolves rob"<<endl;
                                #endif
                                cityList[i].blueWarrior->robWeapon(*cityList[i].redWarrior,hour,min);
                            }
                        }
                    }
                    break;
                }
                case 40:{// battle
                    for(int i = 1; i <N+1;++i){
                        if(cityList[i].ready4Battle()){
                            cityList[i].Battle(hour,min);
                        }
                    }
                    break;
                }
                case 50:{//hq report
                    redHQ.reportElements(hour,min);
                    blueHQ.reportElements(hour,min);
                    break;
                }
                case 55:{
                    for(int i = 1; i <N+1;++i){
                        if(cityList[i].redWarrior!=NULL){
                            cityList[i].redWarrior->report(hour,min);
                        }
                        if(cityList[i].blueWarrior!=NULL){
                            cityList[i].blueWarrior->report(hour,min);
                        }
                    }
                    break;
                }
                default:break;
            }
        }
    }
    return 0;
}