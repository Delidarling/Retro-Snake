#include <windows.h>
#ifndef GOTOXY_H_INCLUDED
#define GOTOXY_H_INCLUDED
using namespace std;
void gotoxy(int x,int y)
{
   HANDLE hout;//HANDLE是一个定义好的结构体
   COORD coord;//COORD是一个定义好的结构体
   coord.X=x;
   coord.Y=y;
   hout=GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleCursorPosition(hout,coord);
}
#endif // GOTOXY_H_INCLUDED
