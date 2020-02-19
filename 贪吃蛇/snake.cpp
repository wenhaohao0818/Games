#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h> 
#define MAP_HEIGHT 24
#define MAP_WIDTH 80
#define SNAKESIZE 50 
struct
{
	int x;
	int y;
}food;

struct
{
	int x[SNAKESIZE];
	int y[SNAKESIZE];
	int len;
	int speed;
}snake;

int key='w';
int asdw='w';
int changeflag=0;
int die=0;

void drawMap()
{
	int i;
	srand((unsigned int)time(NULL));
	for(i=0;i<=MAP_HEIGHT;i++){//画左右边框 
		gotoxy(0,i);
		printf("█");
		gotoxy(MAP_WIDTH,i);
		printf("█");
	}
	
	for(i=0;i<=MAP_WIDTH;i+=2){//画上下边框 
		gotoxy(i,0);
		printf("█");
		gotoxy(i,MAP_HEIGHT);
		printf("█");
	}
	snake.len=3;//蛇的初始长度 
	snake.speed=350;//蛇的初始速度 
	snake.x[0]=MAP_WIDTH/2;//蛇头的初始位置 
	snake.y[0]=MAP_HEIGHT/2;
	gotoxy(snake.x[0],snake.y[0]);
	printf("◎");//打印蛇头
	
	for(i=1;i<snake.len;i++){//打印蛇身
		snake.x[i]=snake.x[i-1]+2;
		snake.y[i]=snake.y[i-1];
		gotoxy(snake.x[i],snake.y[i]);
		printf("◎");
	}
	
	while(1){//产生第一个食物 
		food.x=rand()%(MAP_WIDTH-4)+2;
		food.y=rand()%(MAP_HEIGHT-2)+1;
		if(food.x%2==0){//与蛇头对齐 
			gotoxy(food.x,food.y);
			printf("★");//打印食物 
			break;
		}
	}
	
	
}

void createFood()
{//如果吃到食物 
	if(snake.x[0]==food.x&&snake.y[0]==food.y){
		srand((unsigned int)time(NULL));
		while(1){//循环找到一个合适的食物 
			int flag=1;//位置合适的标记1不合适0 
			int i;
			food.x=rand()%(MAP_WIDTH-4)+2;
			food.y=rand()%(MAP_HEIGHT-2)+1;
			if(snake.x[0]==food.x&&snake.y[0]==food.y){
					flag=0;
					break;
			}
			for(i=1;i<snake.len;i++){
				if(food.x==snake.x[i]&&snake.y[i]==food.y){
					flag=0;
					break;
				}
			}
			if(flag&&food.x%2==0){
				break;
			}
		}
		gotoxy(food.x,food.y);
		printf("★");
		snake.len++;//蛇长度加1 
		changeflag=1;//蛇身改变的标记 
	}
	
	
	
}
void keyDown()
{
	int i;
	asdw=key;//将上次的按键存到asdw 
	if (_kbhit())//如果键盘有输入 
	{		
		fflush(stdin);
		key = _getch();
	}
	if(!changeflag){//擦除最后一节 
		gotoxy(snake.x[snake.len-1],snake.y[snake.len-1]);
		printf("  ");
	}
	for(i=snake.len-1;i>0;i--){//往前移动 
		snake.x[i]=snake.x[i-1];
		snake.y[i]=snake.y[i-1];
	}
	
	if(asdw=='w'&&key=='s'){key='w';}//不回头检测 
	else if(asdw=='s'&&key=='w'){key='s';}//不回头检测
	else if(asdw=='a'&&key=='d'){key='a';}//不回头检测
	else if(asdw=='d'&&key=='a'){key='d';}//不回头检测
	if(key!='w'&&key!='s'&&key!='a'&&key!='d') key=asdw;

	switch(key){
				case 'W':
				case 'w':snake.y[0]--;
					break;
				case 'S':
				case 's':snake.y[0]++;
					break;
				case 'A':
				case 'a':snake.x[0]-=2;
					break;
				case 'D':
				case 'd':snake.x[0]+=2;
					break;
				
	}
	asdw=key;
	gotoxy(snake.x[0],snake.y[0]);
	printf("◎");	
	changeflag=0;
	gotoxy(MAP_HEIGHT+2,0);

	
}
int snakeStatus()//判断蛇的状态 
{
	int i;
	die=0;
	if(snake.x[0]==2||snake.x[0]==MAP_WIDTH-2||snake.y[0]==0||snake.y[0]==MAP_HEIGHT-1){
		 die=1;
	}
	for(i=1;i<snake.len;i++){
		if(snake.x[0]==snake.x[i]&&snake.y[i]==snake.y[0]){
			die=1;
		}
	}
	return die;
}
void gotoxy(int x,int y)//光标移动 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(handle,coord);
}
int main()
{
	system("pause");
	drawMap();
	while(1)
	{   
		if(snake.len>5){
			snake.speed=300;
		}
		if(snake.len>10){
			snake.speed=150;
		}if(snake.len>15){
			snake.speed=100;
		}
		createFood(); 
		gotoxy(MAP_WIDTH+5,MAP_HEIGHT/2-1);
		printf("蛇的长度：%d",snake.len);
		gotoxy(MAP_WIDTH+5,MAP_HEIGHT/2);
		printf("当前运动方向：%c",key);
		gotoxy(MAP_WIDTH+5,MAP_HEIGHT/2+1);
		printf("休眠时间：%d",snake.speed);
		Sleep(snake.speed);
		keyDown();
		die=snakeStatus();
		if(die){
			break;
		}	
	}
	gotoxy(MAP_WIDTH/2,MAP_HEIGHT+3);
	printf("GAMEOVER\t");
	system("pause");
	return 0;
}

