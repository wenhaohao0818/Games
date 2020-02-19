#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <easyx.h>
#include <mmsystem.h>
#define width 400
#define height 600
#define enemy_width 50 
#define enemy_height 30
#define hero_width 80
#define hero_height 50
#define bullet_width 20
#define bullet_height 40
#pragma comment(lib,"winmm.lib")

char key;
int score;
int enemy_create_speed;
int bullet_create_speed;
int hero_speed;
int GAME_speed;

IMAGE BG;
IMAGE enemys;
IMAGE my;
IMAGE boom[19];
IMAGE bullets;

struct {
	int x;
	int y;
	int f;
	int v;

}enemy[20];
struct {
	int x=160;
	int y=550;
}hero;
struct{
	int x;
	int y;
	int f;
	int v;
}bullet[20];
void GAMEINI()//初始化
{
	loadimage(&BG, "bg.jpg", width, height);
	loadimage(&enemys, "enemy.png", enemy_width, enemy_height);
	loadimage(&my, "hero.png", hero_width, hero_height);

	{
		TCHAR filename[20] = _T("");
		for (int i = 0;i < 19;i++) {
			wsprintf(filename, _T("explosion%d.png"), i + 1);
		}

		for (int i = 0;i < 19;i++) {
			loadimage(&boom[i], filename, 64, 48);
		}

	}

	loadimage(&bullets, "bullet.png", bullet_width, bullet_height);
	PlaySound("bgm.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	
	hero_speed = 20;//我的移动速度
	score = 0;
	enemy_create_speed=600;//敌机产生速度
	bullet_create_speed=250;//子弹发射速度
	GAME_speed = 8;//全局游戏速度

}
//PNG透明函数
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{

	// 变量初始化
	DWORD *dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD *draw = GetImageBuffer();
	DWORD *src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

					 // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

void create_enemy()//创建敌机
{
	for (int i = 0;i < 20;i++) {
		
		if (enemy[i].f == 0) {
			enemy[i].y = 0;
			enemy[i].x = rand() % (width- enemy_width);
			enemy[i].f = 1;
			enemy[i].v = 2;//敌机移动速度
			break;
		}
		
		
	}
}
void enemy_move()//敌机移动
{
	for (int i = 0;i < 20;i++) {
		if (enemy[i].f == 1) {
			enemy[i].y += enemy[i].v;
		}
		if (enemy[i].y > height- enemy_height-10) {
			enemy[i].f = 0;
			enemy[i].y = 0;
		}
	}
}
void create_bullet()//创建子弹
{
	for (int i = 0;i < 20;i++) {

		if (bullet[i].f == 0) {
			bullet[i].f = 1;
			bullet[i].x = hero.x + hero_width/2-bullet_width/2;//使子弹贴图到飞机的中间
			bullet[i].y = hero.y - bullet_height;
			bullet[i].v = 4;//子弹的移动速度
			break;
		}
		
	}
}
void bullet_move()//子弹移动
{
	for (int i = 0;i < 20;i++) {
		if (bullet[i].f == 1) bullet[i].y -= bullet[i].v;
		if (bullet[i].y <0) bullet[i].f = 0;
	}
}
void hero_move()//我的移动
{
	
		if (_kbhit()) {
				key = _getch();
					switch (key)
					{
					case 'W':
					case 'w':hero.y -= hero_speed;
						break;
					case 'S':
					case 's':hero.y += hero_speed;
						break;
					case 'A':
					case 'a':hero.x-= hero_speed;
						break;
					case 'D':
					case 'd':hero.x+= hero_speed;
						break;
					}
			}
		//防出界
		if (hero.x<0) {
			hero.x = 0;
		}
		if (hero.x > width-hero_width) {
			hero.x = width - hero_width;
		}
		if (hero.y < 0) {
			hero.y = 0;
		}
		if (hero.y > height-hero_height) {
			hero.y = height - hero_height;
		}
	
		

}
void shoot()//判断击中
{
	for (int i = 0;i < 20;i++) {
		for (int j = 0;j < 20;j++) {
			if ((bullet[i].f==1)&&(enemy[j].f==1)) {
				if ((bullet[i].x >= enemy[j].x-bullet_width) && (bullet[i].x <= enemy[j].x + enemy_width) && (bullet[i].y >= enemy[j].y) && (bullet[i].y <= enemy[j].y + enemy_height)) {
					bullet[i].f = 0;
					enemy[j].f = 0;
					score++;//分数


					DWORD ttt1, ttt2; 
					ttt1=GetTickCount();
					int k = 19;
					while (k>=14) {
						ttt2 = GetTickCount();
						if (ttt2 - ttt1 > 1) {//贴爆炸图片，多了会卡顿，待优化

								drawAlpha(&boom[k--], enemy[j].x, enemy[j].y);
								ttt1 = ttt2;
						}
					}

					break;
				}
				
			}
		}
	}
	for (int i = 0;i < 20;i++) {//飞机相撞
		if (enemy[i].f == 1) {
			if ((hero.x >= enemy[i].x-hero_width) && (hero.x <= enemy[i].x +enemy_width) && (hero.y >= enemy[i].y) && (hero.y <= enemy[i].y + enemy_height)) {

				enemy[i].f = 0;
				DWORD tttt1, tttt2;
				tttt1 = GetTickCount();
				int k = 19;
				while (k >=14) {
					tttt2 = GetTickCount();
					if (tttt2 - tttt1 > 0.1) {
						drawAlpha(&boom[k--], enemy[i].x, enemy[i].y);
						tttt1 = tttt2;
					}
				}

			}
		}
	}
}
void Game_paint()//画面绘制
{
	BeginBatchDraw();
	
	cleardevice();
	putimage(0, 0, &BG);//背景图
	drawAlpha(&my, hero.x,hero.y);//我的飞机

	for (int i = 0;i < 20;i++) {
		if (enemy[i].f == 1) {
			drawAlpha( &enemys, enemy[i].x, enemy[i].y);//敌机
		}
	}

	for (int i = 0;i < 20;i++) {
		if (bullet[i].f == 1) {
			drawAlpha(&bullets,bullet[i].x, bullet[i].y);//子弹
		}
	}
	TCHAR s[5];
	wsprintf(s, _T("%d"), score);//分数
	outtextxy(50, 10, s);
	EndBatchDraw();
}
int main()
{
	initgraph(width, height);//界面
	GAMEINI();//初始化
	DWORD t1, t2, tt1, tt2;
	t1 = GetTickCount();
	tt1 = GetTickCount();
	while (1) {
		t2 = GetTickCount();
		if (t2 - t1 >= enemy_create_speed)
		{
			create_enemy();   
			t1 = t2;
		}
		tt2 = GetTickCount();
		if (tt2 - tt1 >= bullet_create_speed)
		{
			create_bullet();
			tt1 = tt2;
		}
		enemy_move();
		bullet_move();
		hero_move();
		shoot();
		Game_paint();
		Sleep(GAME_speed);
	};
	return 0;
}
