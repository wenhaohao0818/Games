#include <stdio.h>
#include <graphics.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include <easyx.h>
#pragma comment(lib,"winmm.lib")
#define PI 3.1415926535
//int DrawText(当前窗口句柄，要输出的字符串，字符串长度-1自动计算，指向绘图区域的指针，文本格式化的选项);
int grade = 0;
void score()
{
	TCHAR s[5];
	RECT r = { 10, 10, 100, 30 };
	_stprintf(s, _T("%d"), grade);
	drawtext(L"Score:", &r, DT_LEFT);
	outtextxy(50, 10, s);
}
int x = 320, y = 380;
IMAGE image;
struct Balloon
{ 
	int x;
	int y;
	int c;
	int v;
	int f;
};
Balloon ball[15];
struct Bullet
{
	int x;
	int y;
	int c;
	double vx;
	double vy;
	int f;
};
Bullet Bull[15];


void Gameinit()
{
	srand((unsigned int)time(NULL));
	loadimage(&image,L"BG.jpg", 640, 480);

	mciSendString(L"open BG.mp3", 0, 0, 0);
	mciSendString(L"play BG.mp3", 0, 0, 0);


	for (int i = 0; i < 15;i++)
	{
		if (i<10)
		{
			ball[i].f = 1;
			ball[i].x=rand()%610;
			ball[i].y=rand()%430;
			ball[i].v = rand() % 2+1;
			ball[i].c=RGB(rand()%256, rand() % 256, rand() % 256);
		}
		else {
			ball[i].f = 0;
			ball[i].c = RGB(rand() % 256, rand() % 256, rand() % 256);
		}
	}
	for (int i = 0; i < 15;i++)
	{
		
		Bull[i].f = 0;
		Bull[i].c = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	
}



void GamePaint()
{
	BeginBatchDraw();
	cleardevice();
	putimage(0, 0, &image);
	//cleardevice();


	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 2);
	circle(320, 450, 60);
	
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	rectangle(10, 10, 80, 25);

	setlinestyle(PS_SOLID,10);
	line(320, 450, x, y);

	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 1);


	for (int i = 0;i < 15;i++) 
	{
		if (ball[i].f == 1) 
		{
			setfillcolor(ball[i].c);
			fillellipse(ball[i].x, ball[i].y, ball[i].x + 30, ball[i].y + 50);
			arc(ball[i].x + 5, ball[i].y + 5, ball[i].x + 25, ball[i].y + 45, 0, PI / 2);
			arc(ball[i].x + 15, ball[i].y + 35, ball[i].x + 35, ball[i].y + 65, PI, PI * 3 / 2);
		}
		if (Bull[i].f == 1)
		{
			setfillcolor(Bull[i].c);
			fillcircle(Bull[i].x, Bull[i].y, 5);

		}
	}

	EndBatchDraw();
}

void BallMove()
{
	for (int i = 0;i < 15;i++)
	{
		if (ball[i].f == 1)
		{
			ball[i].y-=ball[i].v;
			if ((rand() % 2 )) {
				ball[i].x += 1;
			}
			else {
				ball[i].x -= 1;
			}

			if ((ball[i].y < -50)||(ball[i].x<-30)||(ball[i].x>670))
			{
				ball[i].f = 0;
			}
		}
	}
}

void BullMove()
{
	for (int i = 0;i < 15;i++)
	{
		if (Bull[i].f == 1)
		{
			Bull[i].x += Bull[i].vx;
			Bull[i].y -= Bull[i].vy;
		}
		if (Bull[i].x < 0 || Bull[i].x>640 || Bull[i].y < 0 || Bull[i].y>480)
		{
			Bull[i].f = 0;
		}
	}
}

void CreateBall()
{
	for (int i = 0;i < 15;i++)
	{
		if (ball[i].f == 0)
		{
			ball[i].x = rand() % 610;
			ball[i].y = 480;
			ball[i].f = 1;
			break;
		}
	}
}

void CreateBull(double a )
{
	for (int i = 0;i < 15;i++)
	{
		if (Bull[i].f == 0)
		{
			Bull[i].x = x;
			Bull[i].y = y;
			Bull[i].vx = 15 * cos(a);
			Bull[i].vy = 15 * sin(a);
			Bull[i].f = 1;
			break;
		}
	}

}

void Shoot()
{
	for (int i = 0;i < 15;i++) 
	{
		for (int j = 0;j < 15;j++)
		{
			if (ball[i].f&&Bull[i].f)
			{
				if ((Bull[i].x > ball[j].x) && (Bull[i].x < ball[j].x + 30) &&
					(Bull[i].y > ball[j].y) && (Bull[i].y < ball[j].y + 50))
				{
					Bull[i].f = 0;
					ball[j].f = 0;
					grade++;
					
				}
			}
		
		}
	}
}

int main()
{
	DWORD t1, t2,t3,t4;
	initgraph(640, 480);
	
	double a;
	MOUSEMSG msg = { 0 };
	t1 = GetTickCount();
	t3 = GetTickCount();
	Gameinit();
	while (1) 
	{
		t4 = GetTickCount();
		if (t4 - t3 > 500)
		{
			CreateBall();
			t3= t4;
		}
		
		
		GamePaint();
		t2 = GetTickCount();
		if (t2 - t1 > 20)
		{
			BallMove();
			BullMove();
			t1 = t2;
		}
		score();
		if (MouseHit())
		{
			msg = GetMouseMsg();
			a = atan2(450.0 - msg.y, msg.x - 320.0);
			x = 320 + 70 * cos(a);
			y = 450 - 70 * sin(a);
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				CreateBull(a);
			}
			

		}
		
		Shoot();
		//Sleep(50);
		



	}
	
	getchar();
	return 0;
}
