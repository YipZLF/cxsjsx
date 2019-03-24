# 程序设计实习作业-魔兽世界

## 写在前面

这个魔兽三实在是非常的麻烦。但我觉得在写和debug的过程有不少收获，在这里记录一下。希望以后再写这么长的东西的时候效率可以高一些。

[题目链接](<http://cxsjsxmooc.openjudge.cn/2019t3springall/045/>)

## 类的设计

其实可以用更简单的办法把这个题目的意思写出来。但是作为一个课程作业，我希望还是可以用一下课上学的东西。所以很多地方的写法其实略显麻烦。

**每个类的父子关系和从属关系需要设计。**依照原则

* “A是B”，则class A**继承**class B
* “A属于B”，则class A的指针是class B的成员变量

在题目中，武器属于武士，武士属于红蓝司令部；武士在城市、城市里有武士，也可以看成一种属于的关系。另外，swords, bomb,arrow都是武器；dragon,ninja,...都是武士。但是因为只能写一个cpp文件，所以完全按照这个写实现起来有些不方便（主要是定义类的顺序）。

所以这样设计：

```cpp
class Weapon{
    Warrior * owner;
};
class sword:public Weapon{/*unique attributes of swords*/};
class bomb:public Weapon{/*unique attributes of bomb*/};
class arrow:public Weapon{/*unique attributes of arrow*/};


class Warrior{
  	Weapon * weapon[MAX_WEAPON_CNT];  
};
class dragon:public Warrior{/*dragon attributes*/};
class ninja:public Warrior{/*dragon attributes*/};
/*the same for other types of warriors...*/

class City{
    Warrior* blueWarrior,*redWarrior;
public:
    void Battle(int hour,int min){/*battle logic*/};
}cityList[MAX_CITY_CNT];

class HeadQ{
    Warrior * warriorList[MAX_WARRIOR_CNT];
};
```

**每个类有什么样的方法（成员函数）也得设计，得符合这个类的本义。**作战发生在城市，所以由<u>城市</u>来**判断**作战结果(void Battle里面输出结果，得到本城市的武士的信息）；<u>武士</u>来进行**作战**（attackEnemy, getHurt, robWeapon, takeWeapon等方法）；<u>司令部</u>来**产生和管理**武士（makeWarrior等方法）。这样一来类的设计就比较清楚了。

还有个复杂的地方是**作战逻辑**。这个都写在City::Battle()的函数体里面。这个是需要仔细debug的部分。

具体的protected和public的使用区别，按照逻辑来像就好。完整的封装应该有 

```cpp
void myClass::getAttributes() const{}
void myClass::setAttributes() {}
```

这种函数，但是好麻烦……我没有完全按照这种方式封装类的成员便利。但是在其他的作业里面其实已经接触到了类似的类的实现方法，不允许其他类直接访问成员变量，只能通过限定的public的方法（成员函数）来进行访问或修改。

这样设计完之后，OOP的三大特征**封装**、**继承**、**多态，**我们就做到前两个了。第三个就按照题目的意思，例如ninja和其他warrior在使用bomb的时候效果不同，给这个attackEnemy写一个多态（虚函数）就好了。

### 写给OJ

如果真的要用这些类的话，设计完了也就完了。但这里毕竟还是打印些结果，（“要交OJ作业的嘛……”）所以得想想main函数怎么写。

按照时间和方向扫描战场（和题目说的意思一致）。在一个时刻，扫描cityList里的每一个城市，把由cityList找到武士、由武士得到武器状态；或者HeadQuarter产生武士。总之main能直接访问的对象就是City和HeadQuarter。

```cpp
for(tt =0; tt<T; ++tt){
    hour = tt/60; min = tt%60;
    switch(min){
        case 0:/*do sth*/;
        case 5:/*do sth*/;
            /*...*/;
        case 55:/*do sth*/;
    }
}
```

## 一些收获

### 代码是给人读的

有一句我觉得说得很好的话：<u>“代码是写给人看的，只是偶尔让机器运行一下而以。”</u>毕竟做开发的都是人，写的代码可读性要好，逻辑清晰——是在没法用代码清晰的，就用注释；命名明确——statusHasChanged比flag的可理解性就好的多。虽然老师并不检查代码，但是给自己debug方便，不如多写几个字。

### 多增加少重构

就算是自己写的代码，过几天不看也会忘记。除了可读性好，帮助自己和co-worker快速catch up，在进行代码修改的时候，尽量不要做大范围的重构，能增加就增加，实在冗余得不行再考虑重构。否则对于耦合度较高的模块之间，修改原来框架带来debug时间上的损失是得不偿失的。

### 逐块测试

想要一步到位写完整个代码然后一起debug只有**理论上的可能性**。这样耗费的时间太长了，而且如果bug之间有耦合，这就会出现“解决一个bug，另一个新的bug又来了”的情况。令人无比崩溃。所以要给自己设计的对象写测试模块，既有<u>测试用的成员函数</u>，又有<u>测试用的main函数里面的代码</u>，确认对象中重要的方法的实现是正确的。每实现完一个类、或者一个类的某个重要方法，就得自己给自己想一些test case，来进行测试。我认为这部分是<u>艺术</u>，debug功夫高的朋友应该可以想到一些很好的corner case。

>> > “无他，惟手熟耳。”

可以用#define DEBUG的办法，这样在最后提交的时候可以直接把这个宏注释掉，所有debug的输出都不会被编译进可执行文件里。

```cpp
#define DEBUG

#ifdef DEBUG
	/*test codes*/
	cout<<"infomation"<<endl;
#endif
```

### delete要小心

这部分其实没有收获很多，因为这个题目并非专门练习这个环节的。内存泄露可能带来的很多问题，在这个小程序里面一点内存泄露后果并不严重，但作为良好习惯的养成，还是得注意。在这个程序中，武器报废、武士死亡，都意味着对象的消亡，需要把相应的内存空间delete掉，不能只是用一个NULL替换原来的指针。

还得注意delete掉之后的内存空间不能再被调用，不然会出现segmentation fault，让人觉得莫名其妙。说来简单，但是像这里，一个指向warrior对象的指针又在city里又在headquarter里，任何一边delete掉之后，另外一边都要跟着相应处理。否则很容易出错。我的处理办法是，武士死亡应该交给司令部“管理”，所以delete的事情由司令部做，city只负责直接用NULL替换原来的指针。

### 善用gdb和IDE

就是debug的各种工具啦。

### 好好休息

有的逻辑错误——我承认自己很容易在if else 密集的地方犯逻辑错误——不是认真看就能看出来的，一方面写之前要仔细想，写完之后要测试，还有debug的时候也得给自己比较清醒的头脑。如果写到脑子成了浆糊还勉强继续，工作效率只会越来越低，还不如去休息一下。Work smart rather than work hard.



> 附上代码，上面有debug的痕迹……哈哈。