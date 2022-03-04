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
#define snakemaxsize 50//�����ߵ���󳤶ȣ�����ͷ��
#define movespeedmax 200//�����ߵ�����ƶ��ٶ�
#define snakehead "��"//��ͷ��ʽ
using namespace std;



string str="��";



//ȫ�ֱ���������
time_t t_start,t_end;
int st;
int judge;
char keytrack;
char ch;//������ҵ��ƶ�ָ��
int foodx,foody;//ʳ��ĺ�������
int mapmode;//��ͼģʽѡ��




//��������
void recycle();//����������ʹ�ӡ������Ĵ���Ž���ʵ������
void foodappear();//ʳ����ֺ���
void map1();//��ͼģʽ1
void map2();//��ͼģʽ2
void mapinit();//��ͼ�߽�
void menuinit();//��Ϸ�������Ҳ���Ϣ������
void highest_record();//��¼����ʾ��߳ɼ�
void Menu();//��Ϸ��ʼ�˵�
void difficulty();//��Ϸ�Ѷȼ���ʼ�ٶȵ�����
void mode();//��Ϸ��ͼѡ��
void grade();//��ʷ��߷ֲ鿴����
void explanation();//��Ϸ˵������
void endingpage();//��Ϸʧ�ܽ���





//����snake��
class snake
{
private:
    int x[snakemaxsize],y[snakemaxsize];//�ߵ�ÿһ�ڵĵ�����
    int record;//��¼�߰���ͷ�ĵĽ���
    int movespeed;//�ߵ��ƶ��ٶ�
    int trackx,tracky;//��¼�ߵ����һ��ʹ���߽�������
public:
    snake():movespeed(400){}//���캯����ʼ���ߵ��ƶ��ٶ�
    void snakebody();//����������
    void initmove();//������Ϸ���ߵĳ�ʼ���ƶ�
    void snakemove();//�ߵ��ƶ�
    void knock();//��ײǽ��ײ���Լ��ļ�⺯��
    void recycle();//����������ʹ�ӡ������Ĵ���Ž���ʵ������
    friend void foodappear();
    friend void difficulty();
    friend void highest_record();
}s;//����һ������Ķ���


void snake::knock() //��Ϸʧ�ܣ���ײ���
{
    int i;
    for(i=1;i<record;i++)
    {
        if(x[0]==x[i]&&y[0]==y[i])//��ͷ�����Լ�����
           {system("cls");
           t_end=time(NULL);
           st=difftime(t_end,t_start);
           endingpage();
           }
    }
        if(x[0]<=1||x[0]>=map_length-3||y[0]==0||y[0]==map_width-1)   //��ͷײǽ
           {system("cls");
           t_end=time(NULL);
           st=difftime(t_end,t_start);
            endingpage();
           }
    if(mapmode==3)
    {
       if((x[0]==14&&y[0]>4&&y[0]<20)||(x[0]==54&&y[0]>8&&y[0]<29)||(y[0]==19&&x[0]>=14&&x[0]<=32)||(y[0]==9&&x[0]>=32&&x[0]<=54))//��ͷ�����ϰ�����
           {system("cls");
           t_end=time(NULL);
           st=difftime(t_end,t_start);
            endingpage();
           }
    }
    else if(mapmode==2)
    {
        if(((x[0]>=16&&x[0]<22)||(x[0]>=48&&x[0]<54))&&((y[0]>=8&&y[0]<11)||(y[0]>=19&&y[0]<22)))//��ͷ�����ϰ�����
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
    cout << snakehead;//��ͷ
    for(i=1;i<=3;i++)//��ӡ��������
    {
        y[i]=y[0];
        x[i]=x[i-1]-2;
        gotoxy(x[i],y[i]);
        cout << str;
    }
    record=4;//��¼�����߽���
}

void snake::recycle()
{
     int i,h;
     i=s.record-1;
     s.trackx=s.x[i];//��¼���ڳԵ�ʳ��ǰ���һ�ڵ�����,�����߳Ե�ʳ�����β������һ������
     s.tracky=s.y[i];
     gotoxy(s.x[i],s.y[i]);
     cout << "  ";//�����ߵ����һ��
     for(i=s.record-1;i>=1;i--)//��¼��һ���ߵĸ�������λ��
          {
             s.x[i]=s.x[i-1];
             s.y[i]=s.y[i-1];
          }
     if(s.x[0]==foodx&&s.y[0]==foody)//�Ե�ʳ��
     {s.record++;
     s.x[s.record-1]=s.trackx;
     s.y[s.record-1]=s.tracky;
     gotoxy(s.trackx,s.tracky);
     cout << str;//��ӡ�µ�����
     foodappear();//��ӡ��ʳ��
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
    //������
    case 'w':keytrack=ch;//��¼��һ�ε�ָ�ʹ���߽��յ��޹�ָ��ʱִ��default��䣬����ԭ�����˶�����
             while(!_kbhit())
             {
             recycle();
             x[0]=x[1];//�����ߵ�ʱ����ͷ�͵�һ������ĺ�������ͬ
             y[0]=y[0]-1;
             gotoxy(x[0],y[0]);
             cout << snakehead;//��������ָ����ӡ�µ���ͷ
             knock();//����Ƿ�ײǽ
             if(x[0]==foodx&&y[0]==foody)//ͷ����ʳ��
             {
                 if(movespeed>=movespeedmax)
                    movespeed-=100;//�Ե�ʳ����ٶ�����
                 continue;//ֱ�ӽ���ѭ��
             }
             Sleep(movespeed);
             if(_kbhit())
             {
                ch=_getch();   //��¼��һ�θ��ĵ�ָ��
                while(_kbhit())
                    {_getch();}//��ջ������������������С��ֹͣ�ƶ�������
                if(ch!='s')
                snakemove();
                else
                    continue;//���෴�����߼�����֮ǰ�����ƶ�
             }
             }
             break;

    //������
    case 'a':keytrack=ch;//��¼��һ�ε�ָ�ʹ���߽��յ��޹�ָ��ʱִ��default��䣬����ԭ�����˶�����
             while(!_kbhit())
             {
             recycle();
             x[0]=x[1]-2;
             y[0]=y[0];
             gotoxy(x[0],y[0]);
             cout << snakehead;//����ָ����ӡ�µ���ͷ
             knock();//����Ƿ�ײǽ
             if(x[0]==foodx&&y[0]==foody)//ͷ����ʳ��
             {
                 if(movespeed>=movespeedmax)
                    movespeed-=100;//�Ե�ʳ����ٶ�����
                 continue;//ֱ�ӽ���ѭ��
             }
             Sleep(movespeed);
             if(_kbhit())
             {
                ch=_getch();//��¼��һ�θ��ĵ�ָ��
                while(_kbhit())
                    {_getch();}//��ջ������������������С��ֹͣ�ƶ�������
                if(ch!='d')
                snakemove();
                else
                   continue;//���෴�����߼�����֮ǰ�����ƶ�
             }
             }
             break;

    //������
    case 's':keytrack=ch;//��¼��һ�ε�ָ�ʹ���߽��յ��޹�ָ��ʱִ��default��䣬����ԭ�����˶�����
             while(!_kbhit())
             {
             recycle();
             x[0]=x[1];
             y[0]=y[0]+1;
             gotoxy(x[0],y[0]);
             cout << snakehead;//����ָ����ӡ�µ���ͷ
             knock();//����Ƿ�ײǽ
             if(x[0]==foodx&&y[0]==foody)//ͷ����ʳ��
             {
                 if(movespeed>=movespeedmax)
                    movespeed-=100;//�Ե�ʳ����ٶ�����
                 continue;//ֱ�ӽ���ѭ��
             }
             Sleep(movespeed);
             if(_kbhit())
             {
                ch=_getch();//��¼��һ�θ��ĵ�ָ��
                while(_kbhit())
                    {_getch();}//��ջ������������������С��ֹͣ�ƶ�������
                if(ch!='w')
                snakemove();
                else
                    continue;//���෴�����߼�����֮ǰ�����ƶ�
             }
             }
             break;

    //������
    case 'd':keytrack=ch;//��¼��һ�ε�ָ�ʹ���߽��յ��޹�ָ��ʱִ��default��䣬����ԭ�����˶�����
             while(!_kbhit())
             {
             recycle();
             x[0]=x[1]+2;//����һ����������ٸĻ�ȥ���������߽��ˣ�
             y[0]=y[0];
             gotoxy(x[0],y[0]);
             cout << snakehead;//����ָ����ӡ�µ���ͷ
              knock();//����Ƿ�ײǽ
             if(x[0]==foodx&&y[0]==foody)//ͷ����ʳ��
             {
                 if(movespeed>=movespeedmax)
                    movespeed-=100;//�Ե�ʳ����ٶ�����
                 continue;//ֱ�ӽ���ѭ��
             }
             Sleep(movespeed);
             if(_kbhit())
             {
                ch=_getch();//��¼��һ�θ��ĵ�ָ��
                while(_kbhit())
                    {_getch();}//��ջ������������������С��ֹͣ�ƶ�������
                if(ch!='a')
                snakemove();
                else
                  continue;//���෴�����߼�����֮ǰ�����ƶ�
             }
             }
             break;

    //��ͣ
    case ' ':system("pause>>nul");//����ո����ͣ

    default:{  ch=keytrack;//����һ�ε�ָ�ֵ��ch�ٵݹ����snakemove����
               snakemove();
               break;
            }
    }
}








//��ʼ�˵�
void Menu()
{
    void mode();
    void grade();
    void explanation();
    int i,choice;
        for(i=0;i<map_length+menu;i=i+2)//һ�������������������Ժ�������ż������ӡ����ĺ��������Ϊ68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//����map_width������
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//���һ�еĺ�����Ϊ2*map_width-2
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
            cout<<"**̰ �� ��**";
        gotoxy(40,20);
            cout<<"  ~(�R���Q)~";
        gotoxy(37,24);
            cout<<"1.��   Ϸ   ��   ͼ";
        gotoxy(37,26);
            cout<<"2.�� ʷ �� �� �� ��";
        gotoxy(37,28);
            cout<<"3.��   Ϸ   ˵   ��";
        gotoxy(37,30);
            cout<<"4.��   ��   ��   Ϸ";
        gotoxy(40,33);
            cout<<"����������ѡ��";
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






//��Ϸ��ͼ��ѡ��
void mode()    //ѡ����Ϸ��ͼ
{
    int i,choice;
        for(i=0;i<map_length+menu;i=i+2)//һ�������������������Ժ�������ż������ӡ����ĺ��������Ϊ68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//����map_width������
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//���һ�еĺ�����Ϊ2*map_width-2
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
            cout<<"**̰ �� ��**";
        gotoxy(40,24);
            cout<<"1.M A P1���ף�";
        gotoxy(40,26);
            cout<<"2.M A P2���У�";
        gotoxy(40,28);
            cout<<"3.M A P3���ѣ�";
        gotoxy(40,30);
            cout<<"4.�����ϼ��˵�";
        gotoxy(40,32);
            cout<<"����������ѡ��";
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
    s.snakebody();//��ӡ����
    mapinit();//��ӡ��ͼ�߽�
    menuinit();
    foodappear();//��ӡʳ��
    s.initmove();//��ʼ�����ƶ�
    while(1)//��ѭ����ֹ���������෴�ķ�����߲���
    {
    if(_kbhit())
      {ch=_getch();//�����ַ�
       while(_kbhit())
            {_getch();}//��ջ������������������С��ֹͣ�ƶ�������
             if(ch!='a'&&(ch=='w'||ch=='s'||ch=='d'))//ֻҪ�����෴�ķ����߾���,�����������Ͱ�֮ǰ�����ƶ�
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
void map2()//��ͼģʽ2
{
    srand((unsigned)time(0));
    s.snakebody();
    mapinit();//��ӡ��ͼ�߽�
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
    foodappear();//��ӡʳ��
    s.initmove();//��ʼ�����ƶ�
    while(1)//��ѭ����ֹ���������෴�ķ�����߲���
    {
    if(_kbhit())
      {ch=_getch();//�����ַ�
       while(_kbhit())
            {_getch();}//��ջ������������������С��ֹͣ�ƶ�������
             if(ch!='a'&&(ch=='w'||ch=='s'||ch=='d'))//ֻҪ�����෴�ķ����߾���,�����������Ͱ�֮ǰ�����ƶ�
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
void map3()//��ͼģʽ3
{
    srand((unsigned)time(0));
    s.snakebody();
    mapinit();//��ӡ��ͼ�߽�
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
    foodappear();//��ӡʳ��
    s.initmove();//��ʼ�����ƶ�

    while(1)//��ѭ����ֹ���������෴�ķ�����߲���
    {
    if(_kbhit())
      {ch=_getch();//�����ַ�
       while(_kbhit())
            {_getch();}//��ջ������������������С��ֹͣ�ƶ�������
             if(ch!='a'&&(ch=='w'||ch=='s'||ch=='d'))//ֻҪ�����෴�ķ����߾���,�����������Ͱ�֮ǰ�����ƶ�
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

//��Ϸ�Ѷȵ�ѡ��
void difficulty()
{
        void snake_time();
        int i,choice;
        for(i=0;i<map_length+menu;i=i+2)//һ�������������������Ժ�������ż������ӡ����ĺ��������Ϊ68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//����map_width������
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//���һ�еĺ�����Ϊ2*map_width-2
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
            cout<<"**̰ �� ��**";
        gotoxy(40,24);
            cout<<"1.��       ��";
        gotoxy(40,26);
            cout<<"2.��       ��";
        gotoxy(40,28);
            cout<<"3.��       ��";
        gotoxy(40,32);
            cout<<"����������ѡ��";
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
            cout << "��";
            s.movespeed=400;
        }
        if(choice==2)
        {
            system("cls");
            gotoxy(map_length+18,10);
            cout << "����";
            s.movespeed=350;
        }
        if(choice==3)
        {
            system("cls");
            gotoxy(map_length+18,10);
            cout << "����";
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






//�鿴��ʷ��߷���
void grade()
{
        void highest_record();
        int i,highest;
        for(i=0;i<map_length+menu;i=i+2)//һ�������������������Ժ�������ż������ӡ����ĺ��������Ϊ68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//����map_width������
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//���һ�еĺ�����Ϊ2*map_width-2
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
            cout<<"**̰ �� ��**";

        gotoxy(38,27);
            cout<<"��ǰ��ʷ��߷���Ϊ��";
            ifstream infile("record.txt");
            infile>>highest;
            cout<<highest;

        gotoxy(37,32);
            cout<<"��������������ϼ��˵�";

        system("pause>>nul");


            system("cls");
            Menu();



}







//��Ϸ����Ľ�����˵��
void explanation()
{
     void highest_record();
        int i;
        for(i=0;i<map_length+menu;i=i+2)//һ�������������������Ժ�������ż������ӡ����ĺ��������Ϊ68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//����map_width������
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//���һ�еĺ�����Ϊ2*map_width-2
            cout << str;
        }
        gotoxy(30,7);
            cout<<"S        N       A        K        E";
        gotoxy(30,11);
            cout<<"          CAUTION   OUT!!!";
        gotoxy(20,16);
            cout<<"1.ͨ��ѡ����Ϸ��ͼ�ɽ��벢ѡ�� ��   ��   �� ���ֲ�ͬ�Ѷȵĵ�ͼ";
        gotoxy(20,20);
            cout<<"2.��ͼѡ����ɺ��ֿ���ͨ������Ӧ��ͼ�ѶȵĶ���ֱ����ʼ�ٶ�";
        gotoxy(20,24);
            cout<<"3.ͨ��ѡ����Ϸ��߷����ɷֱ�鿴���ֵ�ͼ��Ӧ���ֳ�ʼ�ٶȵķ���";
        gotoxy(20,28);
            cout<<"4.***̰ �� ��***ײǽ����ײ���Լ����������ζ������ʧ����Ϸ����";
        gotoxy(30,32);
            cout<<"********���� �� �������ϼ��˵�********";
            system("pause>>nul");
            system("cls");
            Menu();


}




//��Ϸ��������
void endingpage()
{

        void snake_time();
        void highest_record();
        int i,choice;
        for(i=0;i<map_length+menu;i=i+2)//һ�������������������Ժ�������ż������ӡ����ĺ��������Ϊ68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//����map_width������
            cout << str;
        }
        for(i=1;i<map_width-1;i++)
        {
            gotoxy(0,i);
            cout << str;
            gotoxy(map_length+menu-2,i);//���һ�еĺ�����Ϊ2*map_width-2
            cout << str;
        }
        gotoxy(30,7);
            cout<<"S        N       A        K        E";
        gotoxy(30,9);
            cout<<"            �r(�s�n�t���q";
        gotoxy(30,17);
            cout<<"        ��   Ϸ   ʱ  ����"<<st<<"s";
        gotoxy(30,13);
            cout<<"     G  A  M  E   O  V  E  R  !!!";

        gotoxy(40,24);
            cout<<"1.�� �� �� ��";
        gotoxy(40,27);
            cout<<"2.�� �� �� Ϸ";
        gotoxy(40,32);
            cout<<"����������ѡ��";
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


void snake::initmove()//������Ϸ���ߵĳ�ʼ���ƶ�
{
    t_start=time(NULL);
    while(!_kbhit())//��kbhit()������޷����У�һ��ʼ������
    {
     int i;
     i=record-1;
     gotoxy(x[i],y[i]);
     cout << "  ";//������β
     for(i=s.record-1;i>=1;i--)//��¼��һ���ߵĸ�������λ��
          {
             x[i]=x[i-1];
             y[i]=y[i-1];
             gotoxy(x[i],y[i]);
             cout << str;//��ʼ��ӡ�µ��߽�
          }
     x[0]+=2;
     gotoxy(x[0],y[0]);
     cout << snakehead;//�µ���ͷλ��
     knock();
     Sleep(movespeed);
    }
}



void mapinit()//��ͼ�߽�
    {
        int i;
        for(i=0;i<map_length+menu;i=i+2)//һ�������������������Ժ�������ż������ӡ����ĺ��������Ϊ68
        {
            gotoxy(i,0);
            cout << str;
            gotoxy(i,map_width-1);//����map_width������
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
            gotoxy(map_length-2,i);//���һ�еĺ�����Ϊ2*map_width-2
            cout << str;
            gotoxy (map_length+menu-2,i);
            cout << str;           //����˵���
        }
    }



void foodappear()//ʳ����ֺ���,���һ���Ƿ�����ڲ��ó��ֵĵط�
{
    int i=s.record-1;
    while(1)//��ֹʳ��������������ͷ,��Ľ�����Ӧ���ƶ���ͷ����ı�
    {
     if(mapmode==1)//��ͬ��ͼʳ��������ֵ�λ��Ҳ��ͬ
     {
        foodx=4+(map_length-10)*rand()/RAND_MAX;
        foody=2+(map_width-5)*rand()/RAND_MAX;
        if((foodx==14&&foody>=4&&foody<20)||(foodx==54&&foody>=8&&foody<29)||(foody==19&&foodx>=14&&foodx<=32)||(foody==9&&foodx>=32&&foodx<=54))//��ֹʳ��������ϰ�����
           continue;
     }
     else if(mapmode==2)//��ͬ��ͼʳ��������ֵ�λ��Ҳ��ͬ
     {
        foodx=4+(map_length-10)*rand()/RAND_MAX;
        foody=2+(map_width-5)*rand()/RAND_MAX;
        if(((foodx>=16&&foodx<=20)||(foodx>=48&&foodx<54))&&((foody>=8&&foody<11)||(foody>=19&&foody<22))) //��ֹʳ��������ϰ�����
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
              goto label;//�����ʳ�ﲻ�������������κ�һ��λ���Ҳ������ϰ����Ͼ�����label����ӡʳ��
           else
              i=i-1;
         }
      else  break;//���������µ�ʳ������
     }
    }
label:  gotoxy(foodx,foody);
        cout << "��";

}


void highest_record()//��¼��Ϸ�����������ļ�
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


void menuinit()//��ӡ��Ϸ�����еĲ˵�����
{
    void snake_time();
    int highest;
    gotoxy(map_length+4,4);
    cout << "��ǰ��Ϸ������0";
    gotoxy(map_length+4,6);
    cout << "��ʷ��߷�����";
    ifstream infile("record.txt");
    infile>>highest;
    gotoxy(map_length+18,6);
    cout<<highest;
    gotoxy(map_length+4,8);
    cout << "��ǰ��Ϸ��ͼ��";
    gotoxy(map_length+4,10);
    cout << "��ǰ��Ϸ�Ѷȣ�";
    gotoxy(map_length+4,18);
    cout << "***W��  ��***";
    gotoxy(map_length+4,20);
    cout << "***S��  ��***";
    gotoxy(map_length+4,22);
    cout << "***A��  ��***";
    gotoxy(map_length+4,24);
    cout << "***D��  ��***";
    gotoxy(map_length+4,28);
    cout << "��<�� ��>��ͣ";
    gotoxy(map_length+4,32);
    cout << "*ף��Ϸ���*";
}






int main()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
    CursorInfo.bVisible = false; //���ؿ���̨���
    SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
    Menu();

    return 0;
}
