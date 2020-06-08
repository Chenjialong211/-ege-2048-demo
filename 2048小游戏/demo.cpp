#include <graphics.h>
#include <cstdio>
#define SPACEING  15 //方格间距 
#define BLOCK_SIZE 105 //使数字与格子对齐 方格尺寸 
#define NUM_BLOCK 16//2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16834 32768 65536


PIMAGE blockImgs[NUM_BLOCK], backgroundImg;//库函数目录-＞图像处理相关函数-＞PIMAGE 保存图片对象 
int grid[4][4];//4*4网格游戏界面
int emptyBlock = 16; //空格子数量 


void ReadImage()//读取游戏元素所用所有图片 
{
	char imgName[100];
//读取数字 
	for (int i = 1, value = 2; i < NUM_BLOCK; i++, value *= 2) {
		blockImgs[i] = newimage();
		sprintf(imgName, "resource\\image\\block_%d.png", value);
		getimage(blockImgs[i], imgName);
	}
	backgroundImg = newimage();
	getimage(backgroundImg, "resource\\image\\background.png");
}
void addNum(int n)//增加一个2 
{	
	for (int i = 0; i < n; ++i) {
		if (emptyBlock == 0)//没有空间生成新数字 
			break;
		int r = rand() % emptyBlock--;		//随机选取当前的一个空位 
		int* ptr = &grid[0][0] - 1;
		int j=0;

		do {								
			while (*++ptr);
		} while (j++<r);

		*ptr = 1;//在当前空位生成2		
	}
}
void draw()//绘制游戏区域 
{
	putimage_withalpha(NULL, backgroundImg, 0, 0);//将图像缓存绘制到窗口上

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = (j + 1) * SPACEING + j * BLOCK_SIZE;
			int y =  (i + 1) * SPACEING + i * BLOCK_SIZE;

			if (grid[i][j] != 0)
				putimage_withalpha(NULL, blockImgs[grid[i][j]], x, y);//将数字与背景图片的方格对齐 
		}
	}
}
bool move(int direction)//移动、合并、静止 
{
	//位置检测常量  向某方向移动等于从某方向开始检测 
	static int x0[4] = { 0, 0, 3, 0 }, y0[4] = { 0, 0, 0, 3 };  
	static int IniValue[4][2] = { {1, 0},{0, 1},{-1, 0}, {0, -1} };
	static int NxtValue[4][2] = { {0, 1}, {1, 0}, {0, 1}, {1, 0} };//左、下为1，右上为-1 
	bool moved = false;//初始状态未移动 
	for (int i = 0; i < 4; i++) {
		int xMovTar = x0[direction] + i * NxtValue[direction][0];
		int yMovTar = y0[direction] + i * NxtValue[direction][1];
		int xIniTar = xMovTar, yIniTar = yMovTar;
		bool empty;
		for (int j = 1; j < 4; j++) {
			empty = false;
			xIniTar += IniValue[direction][0];
			yIniTar += IniValue[direction][1];		
			if (grid[yIniTar][xIniTar] != 0) {//发现非空格 
				if (grid[yMovTar][xMovTar] == 0) {//移动目标是空格 
					grid[yMovTar][xMovTar] = grid[yIniTar][xIniTar];//互换数值 
					grid[yIniTar][xIniTar] = 0;
					moved = empty = true; 
				}
				else if (grid[yIniTar][xIniTar] == grid[yMovTar][xMovTar]) {//合并 与移动目标相等 
					++grid[yMovTar][xMovTar];//移动目标数值*2 
					grid[yIniTar][xIniTar] = 0;//原始目标值为0 
					emptyBlock++;//空格数+1					
					moved = true;
				}
				else if (xIniTar != xMovTar + IniValue[direction][0]
					|| yIniTar != yMovTar + IniValue[direction][1]) {//两数不同，中间有空格，移动至空格，原始目标变为0 
					grid[yMovTar + IniValue[direction][1]][xMovTar + IniValue[direction][0]] = grid[yIniTar][xIniTar];
					grid[yIniTar][xIniTar] = 0;
					moved = true;
				}
				if (!empty) {//两个数不同且相邻，不移动 
					xMovTar += IniValue[direction][0];
					yMovTar += IniValue[direction][1];
				}
			}	
		}
	}
	return  moved;
}
void delImage()//释放掉用到的图片资源 
{
	for (int i = 0; i < NUM_BLOCK; i++)
		delimage(blockImgs[i]);
	delimage(backgroundImg);
}


int main(){
	initgraph(500, 500);
	ReadImage();//读取图片资源 
	addNum(2);//初始界面2个2
	draw(); 
	
	for (; is_run(); delay_fps(60)) {
	//检测键盘输入，控制移动方向  
	int direction = -1;
	while (kbmsg()) {
		key_msg keyMsg = getkey();
		if (keyMsg.msg == key_msg_down) {
			switch (keyMsg.key) {
				case key_left:	direction = 0; break; 
				case key_up:	direction = 1; break;
				case key_right:	direction = 2; break;
				case key_down:	direction = 3; break;
				}
			}
		}
		if (direction != -1 && move(direction)) {//检测 
			addNum(1);//增加一个2
			draw();
		}
	
	}
	delImage();
	closegraph();
}


