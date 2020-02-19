#include <stdio.h>
#include <windows.h>
#include <conio.h>
#define height 26
#define width 80
int minmid;//控制间隔 
int maxmid;
int key;
int changeflag;
int score=0;
int wall_move;
int interval;
int speed; 
struct
{
	int x;
	int y;
	
}fly;
struct
{
	int x;
	int upy;
	int downy;
}walls[5];

void gotoxy(int x,int y)//光标移动 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X=x;//列 
	coord.Y=y;//行 
	SetConsoleCursorPosition(handle,coord);
}

void original_state()//初始化数据 
{
	
	walls[0].x=width-2;
	walls[0].upy=10;
	walls[0].downy=20;
	
	walls[1].x=width-2-interval*1;
	walls[1].upy=12;
	walls[1].downy=18;
	
	walls[2].x=width-2-interval*2;
	walls[2].upy=10;
	walls[2].downy=16;
	
	walls[3].x=width-2-interval*3;
	walls[3].upy=14;
	walls[3].downy=20;
	
	walls[4].x=width-2-interval*4;
	walls[4].upy=10;
	walls[4].downy=20;
	
	score=0; 
}
void wall()
{
	for(int k=0;k<5;k++){
			if(walls[k].x==0){
			walls[k].x=width-2;
			int flag=0;
			while(!flag){
				int yup,ydown;
				yup=rand()%height;
				ydown=rand()%height;
				if((yup<ydown)&&((ydown-yup)>minmid)&&(yup>=height/4)&&(ydown<=height/4*3)&&((ydown-yup)<maxmid)){	
						walls[k].upy=yup;
						walls[k].downy=ydown;
						flag=1;
					
				}else{
					flag=0;
				}
			}
			
		}
		for(int i=1;i<walls[k].upy;i++){
			gotoxy(walls[k].x,i);
			printf("  ");
		}
		for(int i=height-1;i>walls[k].downy;i--){
			gotoxy(walls[k].x,i);
			printf("  ");
		}
		
		walls[k].x-=wall_move;//墙的移动（2的倍数） 
		
		for(int i=0;i<walls[k].upy;i++){
			gotoxy(walls[k].x,i);
			printf("█");
		}
		for(int i=height;i>walls[k].downy;i--){
			gotoxy(walls[k].x,i);
			printf("█");
		}
		gotoxy(0,0);
	}
	
	
	
	

}
int status()
{

	for(int k=0;k<5;k++){
		if(fly.x==walls[k].x){
			if(fly.y<=walls[k].upy||fly.y>=walls[k].downy) return 0;
			else score++;
		}
	}
	if((fly.y<1)||(fly.y>height-1)||(fly.x<2)||(fly.x>width-2)){
		return 0;
	}
	return 1;
	
	
}
void drawMap()
{
	int i;
	for(i=0;i<height;++i){//左右 
		gotoxy(0,i);
		printf("█");
		gotoxy(width,i);
		printf("█");
	}
	for(i=0;i<width+2;i+=2){//上下 
		gotoxy(i,0);
		printf("█");
		gotoxy(i,height);
		printf("█");
	}
	fly.x=width/4;
	fly.y=height/2;
	gotoxy(fly.x,fly.y);
	printf("█");
	gotoxy(0,0);


}
void keyDown()
{
	if(_kbhit()){
		key=getch();
	}

	if(!changeflag){
		gotoxy(fly.x,fly.y);
		printf("  ");
	}
	
	switch(key)
	{
		case 'W':
		case 'w': fly.y--;break;
		case 'S':
		case 's': fly.y++;break;
		case 'A':
		case 'a': fly.x-=2;break;
		case 'D':
		case 'd': fly.x+=2;break;
		case ' ': fly.y-=2;break;
	}
		gotoxy(fly.x,fly.y);
		printf("█");
		gotoxy(0,0);
	
	key='s';
	changeflag=0;
}

void admin()
{
	minmid=3;//最小间隔
	maxmid=7;
	wall_move=2;//墙每次移动格数 
	interval=12;//墙的初始间隔 
	speed=150;//游戏速度（全局） 
	
}
void Print()
{
	gotoxy(width+10,height/2-1);
	printf("You score is :%d",score);
	gotoxy(width+10,height/2);
	printf("You speed is :%d",speed);
	gotoxy(width+10,height/2+1);
	printf("MIN interval is :%d",minmid);
	gotoxy(width+10,height/2+2);
	printf("MAX interval is :%d",maxmid);
	gotoxy(0,0);
	
}
int main()
{
	admin(); 
	int cont=13;
	while(cont==13){
		system("cls");
		drawMap();
		original_state();
		while(status()){
			Sleep(speed);//速度 
			keyDown();
			wall();
			Print();
			
		}
		gotoxy(width/2,height/2);
		printf("Game over!");
		gotoxy(width/2-7,height/2+1);
		printf("Press enter to continue!");
		gotoxy(0,0);
		cont=getch();
	}
	gotoxy(0,height+1);
	
	return 0;
}
