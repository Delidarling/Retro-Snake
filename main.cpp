#include <iostream>
#include <fstream>
#include "gotoxy.h"
#include <windows.h>
#include <ctime>
#include <time.h>
#include <cstdio>
#include <conio.h>
#include <cmath>
#define map_length 70
#define menu 24
#define map_width  36
#define snakemaxsize 50//设置蛇的最大长度（包括头）
#define movespeedmax 200//设置蛇的最大移动速度
#define snakehead "■"//蛇头样式
using namespace std;



string str="■";



//全局变量的声明
time_t t_start,t_end;
int st;
int judge;
char keytrack;
char ch;//接收玩家的移动指令
int foodx,foody;//食物的横纵坐标
int mapmode;//地图模式选择




//函数声明
void recycle();//将擦除蛇身和打印新蛇身的代码放进来实现重用
void foodappear();//食物出现函数
void map1();//地图模式1
void map2();//地图模式2
void mapinit();//地图边界
void menuinit();//游戏过程中右侧信息栏声明
void highest_record();//记录并显示最高成绩
void Menu();//游戏开始菜单
void difficulty();//游戏难度即初始速度的设置
void mode();//游戏地图选择
void grade();//历史最高分查看界面
void explanation();//游戏说明界面
void endingpage();//游戏失败界面





//定义snake类
class snake
{
private:
    int x[snakemaxsize],y[snakemaxsize];//蛇的每一节的点坐标
    int record;//记录蛇包括头的的节数
    int movespeed;//蛇的移动速度
    int trackx,tracky;//记录蛇的最后一节使得蛇节能增加
public:
    snake():movespeed(400){}//构造函数初始化蛇的移动速度
    void snakebody();//绘制蛇身函数
    void initmove();//进入游戏后蛇的初始化移动
    void snakemove();//蛇的移动
    void knock();//蛇撞墙或撞到自己的检测函数
    void recycle();//将擦除蛇身和打印新蛇身的代码放进来实现重用
    friend void foodappear();
    friend void difficulty();
    friend void highest_record();
}s;//定义一个蛇类的对象


void snake::knock() //游戏失败，碰撞检测
{
    int i;
    for(i=1;i<record;i++)
    {
        if(x[0]==x[i]&&y[0]==y[i])//蛇头碰到自己身体
           {system("cls");
           t_end=time(NULL);
           st=difftime(t_end,t_start);
           endingpage();
           }
    }
        if(x[0]<=1||x[0]>=map_length-3||y[0]==0||y[0]==map_width-1)   //蛇头撞墙
           {system("cls");
           t_end=time(NULL);
           st=difftime(t_end,t_start);
            endingpage();
           }
    if(mapmode==3)
    {
       if((x[0]==14&&y[0]>4&&y[0]<20)||(x[0]==54&&y[0]>8&&y[0]<29)||(y[0]==19&&x[0]>=14&&x[0]<=32)||(y[0]==9&&x[0]>=32&&x[0]<=54))//蛇头碰到障碍物上
           {system("cls");
           t_end=time(NULL);
           st=difftime(t_end,t_start);
            endingpage();
           }
    }
    else if(mapmode==2)
    {
        if(((x[0]>=16&&x[0]<22)||(x[0]>=48&&x[0]<54))&&((y[0]>=8&&y[0]<11)||(y[0]>=19&&y[0]<22)))//蛇头碰到障碍物上
           {system("cls");
           t_end=time(NULL);
           st=difftime(t_end,t_start);
            endingpage();
           }
    }
}

void snake::snakebody()
{
    int i;
    x[0]=map_length/2-1;
    y[0]=map_width/2-1;
    gotoxy(x[0],y[0]);
    cout << snakehead;//蛇头
    for(i=1;i<=3;i++)//打印三节蛇身
    {
        y[i]=y[0];
        x[i]=x[i-1]-2;
        gotoxy(x[i],y[i]);
        cout << str;
    }
    record=4;//记录已有蛇节数
}

void snake::recycle()
{
     int i,h;
     i=s.record-1;
     s.trackx=s.x[i];//记录蛇在吃到食物前最后一节的坐标,便于蛇吃到食物后在尾部增长一节身体
     s.tracky=s.y[i];
     gotoxy(s.x[i],s.y[i]);
     cout << "  ";//擦掉蛇的最后一节
     for(i=s.record-1;i>=1;i--)//记录下一次蛇的各节所在位置
          {
             s.x[i]=s.x[i-1];
             s.y[i]=s.y[i-1];
          }
     if(s.x[0]==foodx&&s.y[0]==foody)//吃到食物
     {s.record++;
     s.x[s.record-1]=s.trackx;
     s.y[s.record-1]=s.tracky;
     gotoxy(s.trackx,s.tracky);
     cout << str;//打印新的蛇身
     foodappear();//打印新食物
     highest_record();
     gotoxy(map_length+18,4);
     cout << s.record-4;
     ifstream infile("record.txt");
     infile>>h;
     gotoxy(map_length+18,6);
     cout<<h;
     }
}

void snake::snakemove()
{
    switch(ch)
    {
    //往上走
    case 'w':keytrack=ch;//记录上一次的指令，使得蛇接收到无关指令时执行default语句，保持原来的运动方向
             while(!_kbhit())
             {
             recycle();
             x[0]=x[1];//往上走的时候蛇头和第一节蛇身的横坐标相同
             y[0]=y[0]-1;
             gotoxy(x[0],y[0]);
             cout << snakehead;//接受上移指令后打印新的蛇头
             knock();//检测是否撞墙
             if(x[0]==foodx&&y[0]==foody)//头碰到食物
             {
                 if(movespeed>=movespeedmax)
                    movespeed-=100;//吃到食物后速度增加
                 continue;//直接进行循环
             }
             Sleep(movespeed);
             if(_kbhit())
             {
                ch=_getch();   //记录第一次更改的指令
                while(_kbhit())
                    {_getch();}//清空缓冲区解决快速连按，小蛇停止移动的问题
                if(ch!='s')
                snakemove();
                else
                    continue;//按相反方向蛇继续按之前方向移动
             }
             }
             break;

    //往左走
    case 'a':keytrack=ch;//记录上一次的指令，使得蛇接收到无关指令时执行default语句，保持原来的运动方向
             while(!_kbhit())
             {
             recycle();
             x[0]=x[1]-2;
             y[0]=y[0];
             gotoxy(x[0],y[0]);
             cout << snakehead;//接受指令后打印新的蛇头
             knock();//检测是否撞墙
             if(x[0]==foodx&&y[0]==foody)//头碰到食物
             {
                 if(movespeed>=movespeedmax)
                    movespeed-=100;//吃到食物后速度增加
                 continue;//直接进行循环
             }
             Sleep(movespeed);
             if(_kbhit())
             {
                ch=_getch();//记录第一次更改的指令
                while(_kbhit())
                    {_getch();}//清空缓冲区解决快速连按，小蛇停止移动的问题
                if(ch!='d')
                snakemove();
                else
                   continue;//按相反方向蛇继续按之前方向移动
             }
             }
             break;

    //往下走
    case 's':keytrack=ch;//记录上一次的指令，使得蛇接收到无关指令时执行default语句，保持原来的运动方向
             while(!_kbhit())
             {
             recycle();
             x[0]=x[1];
             y[0]=y[0]+1;
             gotoxy(x[0],y[0]);
             cout << snakehead;//接受指令后打印新的蛇头
             knock();//检测是否撞墙
             if(x[0]==foodx&&y[0]==foody)//头碰到食物
             {
                 if(movespeed>=movespeedmax)
                    movespeed-=100;//吃到食物后速度增加
                 continue;//直接进行循环
             }
             Sleep(movespeed);
             if(_kbhit())
             {
                ch=_getch();//记录第一次更改的指令
                while(_kbhit())
                    {_getch();}//清空缓冲区解决快速连按，小蛇停止移动的问题
                if(ch!='w')
                snakemove();
                else
                    continue;//按相反方向蛇继续按之前方向移动
             }
             }
             break;

    //往右走
    case 'd':keytrack=ch;//记录上一次的指令，使得蛇接收到无关指令时执行default语句，保持原来的运动方向
             while(!_kbhit())
             {
             recycle();
             x[0]=x[1]+2;//改了一下这个数字再改回去又能增加蛇节了？
             y[0]=y[0];
             gotoxy(x[0],y[0]);
             cout << snakehead;//接受指令后打印新的蛇头
              knock();//检测是否撞墙
             if(x[0]==foodx&&y[0]==foody)//头碰到食物
             {
                 if(movespeed>=movespeedmax)
                    movespeed-=100;//吃到食物后速度增加
                 continue;//直接进行循环
             }
             Sleep(movespeed);
             if(_kbhit())
             {
                ch=_getch();//记录第一次更改的指令
                while(_kbhit())
                    {_getch();}//清空缓冲区解决快速连按，小蛇停止移动的问题
                if(ch!='a')
                snakemove();
                else
                  continue;//按相反方向蛇继续按之前方向移动
             }
             }
             break;

    //暂停
    case ' ':system("pause>>nul");//输入空格后暂停

    default:{  ch=keytrack;//把上一次的指令赋值给ch再递归调用snakemove函数
               snakemove();
               break;
            }
    }
}








//初始菜单
void Menu()
{
    void mode();
    void grade();
    void explanation();
    int i,choice;
        for(i=0;i<map_length+menu;i=i+2)//一个■是两倍字体宽度所以横坐标是偶数，打印方块的横坐标最大为68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//宽有map_width个方块
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//最后一列的横坐标为2*map_width-2
            cout << str;
        }
        gotoxy(19,8);
        for(i=0;i<56;i=i+2)
            cout<<str;
        gotoxy(30,7);
            cout<<"S        N       A        K        E";
        gotoxy(73,9);
            cout<<str;
        gotoxy(73,10);
            cout<<str;
        gotoxy(73,11);
            cout<<str;
        gotoxy(73,12);
            cout<<str;
        gotoxy(73,13);
            cout<<str;
        gotoxy(73,16);
            cout<<str;
        gotoxy(41,12);
            cout<<"**贪 吃 蛇**";
        gotoxy(40,20);
            cout<<"  ~(≧▽≦)~";
        gotoxy(37,24);
            cout<<"1.游   戏   地   图";
        gotoxy(37,26);
            cout<<"2.历 史 最 高 分 数";
        gotoxy(37,28);
            cout<<"3.游   戏   说   明";
        gotoxy(37,30);
            cout<<"4.退   出   游   戏";
        gotoxy(40,33);
            cout<<"请输入您的选择：";
        cin>>choice;

        if(cin.fail()||(choice!=1&&choice!=2&&choice!=3&&choice!=4))
        {
            system("cls");
            while(cin.get()!='\n')
            cin.clear();
            Menu();
        }

        if(choice==1)
        {
            system("cls");
            mode();
        }
        if(choice==2)
        {
            system("cls");
            grade();
        }
        if(choice==3)
        {
            system("cls");
            explanation();
        }
        if(choice==4)
        {
            system("cls");
            exit(0);
        }
}






//游戏地图的选择
void mode()    //选择游戏地图
{
    int i,choice;
        for(i=0;i<map_length+menu;i=i+2)//一个■是两倍字体宽度所以横坐标是偶数，打印方块的横坐标最大为68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//宽有map_width个方块
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//最后一列的横坐标为2*map_width-2
            cout << str;
        }
        gotoxy(19,8);
        for(i=0;i<56;i=i+2)
            cout<<str;
        gotoxy(30,7);
            cout<<"S        N       A        K        E";
        gotoxy(73,9);
            cout<<str;
        gotoxy(73,10);
            cout<<str;
        gotoxy(73,11);
            cout<<str;
        gotoxy(73,12);
            cout<<str;
        gotoxy(73,13);
            cout<<str;
        gotoxy(73,16);
            cout<<str;
        gotoxy(41,12);
            cout<<"**贪 吃 蛇**";
        gotoxy(40,24);
            cout<<"1.M A P1（易）";
        gotoxy(40,26);
            cout<<"2.M A P2（中）";
        gotoxy(40,28);
            cout<<"3.M A P3（难）";
        gotoxy(40,30);
            cout<<"4.返回上级菜单";
        gotoxy(40,32);
            cout<<"请输入您的选择：";
        cin>>choice;

        if(cin.fail()||(choice!=1&&choice!=2&&choice!=3&&choice!=4))
        {
            system("cls");
            while(cin.get()!='\n')
            cin.clear();
            mode();
        }

        mapmode=choice;
        if(choice==1)
        {
            system("cls");
            difficulty();
        }
        if(choice==2)
        {
            system("cls");
            difficulty();
        }
        if(choice==3)
        {
            system("cls");
            difficulty();
        }
        if(choice==4)
        {
            system("cls");
            Menu();
        }
}

//MAP1
void map1()
{
    srand((unsigned)time(0));
    s.snakebody();//打印蛇身
    mapinit();//打印地图边界
    menuinit();
    foodappear();//打印食物
    s.initmove();//初始化蛇移动
    while(1)//用循环防止连按两次相反的方向后蛇不动
    {
    if(_kbhit())
      {ch=_getch();//接受字符
       while(_kbhit())
            {_getch();}//清空缓冲区解决快速连按，小蛇停止移动的问题
             if(ch!='a'&&(ch=='w'||ch=='s'||ch=='d'))//只要不往相反的方向走就行,但按其他键就按之前方向移动
                {s.snakemove();
                 break;}
             else if(ch==' ')
                    {system("pause>>nul");
                     s.initmove();}
             else
                    s.initmove();
      }
     }

}

//MAP2
void map2()//地图模式2
{
    srand((unsigned)time(0));
    s.snakebody();
    mapinit();//打印地图边界
    menuinit();
    int x,y,i,j;
    x=16;y=8;
    for(j=0;j<=2;j++)
    {
        for(i=0;i<=4;i+=2)
        {
            gotoxy(x+i,y+j);
            cout << str;
        }
    }
    x=48;y=8;
    for(j=0;j<=2;j++)
    {
        for(i=0;i<=4;i+=2)
        {
            gotoxy(x+i,y+j);
            cout << str;
        }
    }
    x=16;y=19;
    for(j=0;j<=2;j++)
    {
        for(i=0;i<=4;i+=2)
        {
            gotoxy(x+i,y+j);
            cout << str;
        }
    }
    x=48;y=19;
    for(j=0;j<=2;j++)
    {
        for(i=0;i<=4;i+=2)
        {
            gotoxy(x+i,y+j);
            cout << str;
        }
    }
    foodappear();//打印食物
    s.initmove();//初始化蛇移动
    while(1)//用循环防止连按两次相反的方向后蛇不动
    {
    if(_kbhit())
      {ch=_getch();//接受字符
       while(_kbhit())
            {_getch();}//清空缓冲区解决快速连按，小蛇停止移动的问题
             if(ch!='a'&&(ch=='w'||ch=='s'||ch=='d'))//只要不往相反的方向走就行,但按其他键就按之前方向移动
                {s.snakemove();
                 break;}
             else if(ch==' ')
                    {system("pause>>nul");
                     s.initmove();}
             else
                    s.initmove();
      }
      }

}

//MAP3
void map3()//地图模式3
{
    srand((unsigned)time(0));
    s.snakebody();
    mapinit();//打印地图边界
    menuinit();
    int j;
     for(j=5;j<20;j++)
     {
         gotoxy(14,j);
         cout << str;
     }
     for(j=2;j<20;j+=2)
     {
         gotoxy(14+j,19);
         cout << str;
     }
     for(j=10;j<29;j++)
     {
        gotoxy(54,j);
        cout << str;
     }
     for(j=0;j<24;j+=2)
     {
         gotoxy(54-j,9);
         cout << str;
     }
    foodappear();//打印食物
    s.initmove();//初始化蛇移动

    while(1)//用循环防止连按两次相反的方向后蛇不动
    {
    if(_kbhit())
      {ch=_getch();//接受字符
       while(_kbhit())
            {_getch();}//清空缓冲区解决快速连按，小蛇停止移动的问题
             if(ch!='a'&&(ch=='w'||ch=='s'||ch=='d'))//只要不往相反的方向走就行,但按其他键就按之前方向移动
                {s.snakemove();
                 break;}
             else if(ch==' ')
                    {system("pause>>nul");
                     s.initmove();}
             else
                    s.initmove();
      }
     }
}

//游戏难度的选择
void difficulty()
{
        void snake_time();
        int i,choice;
        for(i=0;i<map_length+menu;i=i+2)//一个■是两倍字体宽度所以横坐标是偶数，打印方块的横坐标最大为68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//宽有map_width个方块
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//最后一列的横坐标为2*map_width-2
            cout << str;
        }
        gotoxy(19,8);
        for(i=0;i<56;i=i+2)
            cout<<str;
        gotoxy(30,7);
            cout<<"S        N       A        K        E";
        gotoxy(73,9);
            cout<<str;
        gotoxy(73,10);
            cout<<str;
        gotoxy(73,11);
            cout<<str;
        gotoxy(73,12);
            cout<<str;
        gotoxy(73,13);
            cout<<str;
        gotoxy(73,16);
            cout<<str;
        gotoxy(41,12);
            cout<<"**贪 吃 蛇**";
        gotoxy(40,24);
            cout<<"1.简       单";
        gotoxy(40,26);
            cout<<"2.适       中";
        gotoxy(40,28);
            cout<<"3.困       难";
        gotoxy(40,32);
            cout<<"请输入您的选择：";
        cin>>choice;

        if(cin.fail()||(choice!=1&&choice!=2&&choice!=3))
        {
            system("cls");
            while(cin.get()!='\n')
            cin.clear();
            difficulty();
        }

        if(choice==1)
        {
            system("cls");
            gotoxy(map_length+18,10);
            cout << "简单";
            s.movespeed=400;
        }
        if(choice==2)
        {
            system("cls");
            gotoxy(map_length+18,10);
            cout << "适中";
            s.movespeed=350;
        }
        if(choice==3)
        {
            system("cls");
            gotoxy(map_length+18,10);
            cout << "困难";
            s.movespeed=300;
        }
        if(mapmode==1)
        {
            gotoxy(map_length+18,8);
            cout << "map1";
            map1();
        }
        if(mapmode==2)
        {
            gotoxy(map_length+18,8);
            cout << "map2";
            map2();
        }
        if(mapmode==3)
        {
            gotoxy(map_length+18,8);
            cout << "map3";
            map3();
        }

}






//查看历史最高分数
void grade()
{
        void highest_record();
        int i,highest;
        for(i=0;i<map_length+menu;i=i+2)//一个■是两倍字体宽度所以横坐标是偶数，打印方块的横坐标最大为68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//宽有map_width个方块
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//最后一列的横坐标为2*map_width-2
            cout << str;
        }
        gotoxy(19,8);
        for(i=0;i<56;i=i+2)
            cout<<str;
        gotoxy(30,7);
            cout<<"S        N       A        K        E";
        gotoxy(73,9);
            cout<<str;
        gotoxy(73,10);
            cout<<str;
        gotoxy(73,11);
            cout<<str;
        gotoxy(73,12);
            cout<<str;
        gotoxy(73,13);
            cout<<str;
        gotoxy(73,16);
            cout<<str;
        gotoxy(41,12);
            cout<<"**贪 吃 蛇**";

        gotoxy(38,27);
            cout<<"当前历史最高分数为：";
            ifstream infile("record.txt");
            infile>>highest;
            cout<<highest;

        gotoxy(37,32);
            cout<<"按下任意键返回上级菜单";

        system("pause>>nul");


            system("cls");
            Menu();



}







//游戏规则的介绍与说明
void explanation()
{
     void highest_record();
        int i;
        for(i=0;i<map_length+menu;i=i+2)//一个■是两倍字体宽度所以横坐标是偶数，打印方块的横坐标最大为68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//宽有map_width个方块
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//最后一列的横坐标为2*map_width-2
            cout << str;
        }
        gotoxy(30,7);
            cout<<"S        N       A        K        E";
        gotoxy(30,11);
            cout<<"          CAUTION   OUT!!!";
        gotoxy(20,16);
            cout<<"1.通过选择游戏地图可进入并选择 易   中   难 三种不同难度的地图";
        gotoxy(20,20);
            cout<<"2.地图选择完成后又可以通过对相应地图难度的定义分别定义初始速度";
        gotoxy(20,24);
            cout<<"3.通过选择游戏最高分数可分别查看三种地图对应三种初始速度的分数";
        gotoxy(20,28);
            cout<<"4.***贪 吃 蛇***撞墙或者撞击自己的身体既意味着任务失败游戏结束";
        gotoxy(30,32);
            cout<<"********按任 意 键返回上级菜单********";
            system("pause>>nul");
            system("cls");
            Menu();


}




//游戏结束界面
void endingpage()
{

        void snake_time();
        void highest_record();
        int i,choice;
        for(i=0;i<map_length+menu;i=i+2)//一个■是两倍字体宽度所以横坐标是偶数，打印方块的横坐标最大为68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//宽有map_width个方块
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//最后一列的横坐标为2*map_width-2
            cout << str;
        }
        gotoxy(30,7);
            cout<<"S        N       A        K        E";
        gotoxy(30,9);
            cout<<"            ╮(╯﹏╰）╭";
        gotoxy(30,17);
            cout<<"        游   戏   时  长："<<st<<"s";
        gotoxy(30,13);
            cout<<"     G  A  M  E   O  V  E  R  !!!";

        gotoxy(40,24);
            cout<<"1.返 回 菜 单";
        gotoxy(40,27);
            cout<<"2.退 出 游 戏";
        gotoxy(40,32);
            cout<<"请输入您的选择：";
            cin>>choice;

        if(cin.fail()||(choice!=1&&choice!=2))
        {
            system("cls");
            while(cin.get()!='\n')
            cin.clear();
            endingpage();
        }

        if(choice==1)
        {
            system("cls");
            Menu();
        }
        if(choice==2)
        {

            gotoxy(60,38);
            exit(0);
        }

}







//PREPARE


void snake::initmove()//进入游戏后蛇的初始化移动
{
    t_start=time(NULL);
    while(!_kbhit())//用kbhit()程序就无法运行，一开始往右走
    {
     int i;
     i=record-1;
     gotoxy(x[i],y[i]);
     cout << "  ";//擦除蛇尾
     for(i=s.record-1;i>=1;i--)//记录下一次蛇的各节所在位置
          {
             x[i]=x[i-1];
             y[i]=y[i-1];
             gotoxy(x[i],y[i]);
             cout << str;//开始打印新的蛇节
          }
     x[0]+=2;
     gotoxy(x[0],y[0]);
     cout << snakehead;//新的蛇头位置
     knock();
     Sleep(movespeed);
    }
}



void mapinit()//地图边界
    {
        int i;
        for(i=0;i<map_length+menu;i=i+2)//一个■是两倍字体宽度所以横坐标是偶数，打印方块的横坐标最大为68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//宽有map_width个方块
            cout << str;
        }
        for(i=0;i<menu;i=i+2)
        {
            gotoxy(map_length+i,15);
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length-2,i);//最后一列的横坐标为2*map_width-2
            cout << str;
            gotoxy (map_length+menu-2,i);
            cout << str;           //输出菜单列
        }
    }



void foodappear()//食物出现函数,检查一下是否出现在不该出现的地方
{
    int i=s.record-1;
    while(1)//防止食物出现在蛇身或蛇头,需改进以适应蛇移动后头坐标改变
    {
     if(mapmode==1)//不同地图食物随机出现的位置也不同
     {
        foodx=4+(map_length-10)*rand()/RAND_MAX;
        foody=2+(map_width-5)*rand()/RAND_MAX;
        if((foodx==14&&foody>=4&&foody<20)||(foodx==54&&foody>=8&&foody<29)||(foody==19&&foodx>=14&&foodx<=32)||(foody==9&&foodx>=32&&foodx<=54))//防止食物出现在障碍物上
           continue;
     }
     else if(mapmode==2)//不同地图食物随机出现的位置也不同
     {
        foodx=4+(map_length-10)*rand()/RAND_MAX;
        foody=2+(map_width-5)*rand()/RAND_MAX;
        if(((foodx>=16&&foodx<=20)||(foodx>=48&&foodx<54))&&((foody>=8&&foody<11)||(foody>=19&&foody<22))) //防止食物出现在障碍物上
           continue;
     }
     else if(mapmode==3)
     {
        foodx=4+(map_length-10)*rand()/RAND_MAX;
        foody=2+(map_width-5)*rand()/RAND_MAX;
     }
     while(i>=0)
     {if(foody!=s.y[i]&&foodx!=s.x[i]&&foodx%2==0)
         { if(i==0)
              goto label;//检测完食物不会出现在蛇身的任何一个位置且不出现障碍物上就跳到label处打印食物
           else
              i=i-1;
         }
      else  break;//继续生成新的食物坐标
     }
    }
label:  gotoxy(foodx,foody);
        cout << "■";

}


void highest_record()//记录游戏分数并存入文件
{
    int rec;
    ifstream infile("record.txt");
    infile>>rec;
    if(rec<s.record-4)
        rec=s.record-4;
    try
    {
        fstream fout("record.txt",ios::out|ios::trunc);
        fout<<rec;
        fout.close();
    }
    catch(exception& e)
    {

    ifstream infile("record.txt");
    infile>>rec;
    gotoxy(map_length+18,6);
    cout<<rec;
    }

}


void menuinit()//打印游戏过程中的菜单界面
{
    void snake_time();
    int highest;
    gotoxy(map_length+4,4);
    cout << "当前游戏分数：0";
    gotoxy(map_length+4,6);
    cout << "历史最高分数：";
    ifstream infile("record.txt");
    infile>>highest;
    gotoxy(map_length+18,6);
    cout<<highest;
    gotoxy(map_length+4,8);
    cout << "当前游戏地图：";
    gotoxy(map_length+4,10);
    cout << "当前游戏难度：";
    gotoxy(map_length+4,18);
    cout << "***W：  上***";
    gotoxy(map_length+4,20);
    cout << "***S：  下***";
    gotoxy(map_length+4,22);
    cout << "***A：  左***";
    gotoxy(map_length+4,24);
    cout << "***D：  右***";
    gotoxy(map_length+4,28);
    cout << "按<空 格>暂停";
    gotoxy(map_length+4,32);
    cout << "*祝游戏愉快*";
}






int main()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = false; //隐藏控制台光标
    SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
    Menu();

    return 0;
}
