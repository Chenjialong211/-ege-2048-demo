#include <graphics.h>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#define DEVIDE 15
#define GRID_WIDTH 106
#define NUM_BLOCK 18//2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16834 32768 65536 131072
PIMAGE blockImgs[NUM_BLOCK], backgroundImg;
int grid[4][4];//4*4网格游戏界面
int emptyBlock = 16; //空格子数量 
void ReadImage()//读取游戏元素所用所有图片 
{
	char imgName[100];

	for (int i = 1, value = 2; i < NUM_BLOCK; i++, value *= 2) {
		blockImgs[i] = newimage();
		sprintf(imgName, "resource\\image\\block_%d.png", value);
		getimage(blockImgs[i], imgName);
	}
	backgroundImg = newimage();
	getimage(backgroundImg, "resource\\image\\background.png");
}
void addRandomNum(int n)//生成一个随机数，添加2或4 
{	
	for (int i = 0; i < n; ++i) {
		if (emptyBlock == 0)//没有空间生成新数字 
			break;
		int r = rand() % emptyBlock--;		//随机选取当前的一个空位 
		int* ptr = &grid[0][0] - 1;
		int j=0;

		do {								//找到目标空位
			while (*++ptr);
		} while (j++<r);

		*ptr = (rand() % 10 == 0) + 1;		//随机数字，90%是1，10%是2
	}
}
void draw()
{
	putimage_withalpha(NULL, backgroundImg, 0, 0);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = (j + 1) * DEVIDE + j * GRID_WIDTH;
			int y =  (i + 1) * DEVIDE + i * GRID_WIDTH;

			if (grid[i][j] != 0)
				putimage_withalpha(NULL, blockImgs[grid[i][j]], x, y);
		}
	}
}
bool move(int direction)
{
	//0~3对应移动方向左上右下
	//初始检测位置及检测偏移
	static int x0[4] = { 0, 0, 3, 0 }, y0[4] = { 0, 0, 0, 3 };
	static int firstOffset[4][2] = { {1, 0},{0, 1},{-1, 0}, {0, -1} };
	static int secondOffset[4][2] = { {0, 1}, {1, 0}, {0, 1}, {1, 0} };
	bool moved = false;
	for (int i = 0; i < 4; i++) {
		int xLast = x0[direction] + i * secondOffset[direction][0];
		int yLast = y0[direction] + i * secondOffset[direction][1];
		int xCur = xLast, yCur = yLast;
		bool empty;
		for (int j = 1; j < 4; j++) {
			empty = false;
			xCur += firstOffset[direction][0];
			yCur += firstOffset[direction][1];		
			if (grid[yCur][xCur] != 0) {
				if (grid[yLast][xLast] == 0) {
					grid[yLast][xLast] = grid[yCur][xCur];
					grid[yCur][xCur] = 0;
					moved = empty = true;
				}
				//合并
				else if (grid[yCur][xCur] == grid[yLast][xLast]) {
					++grid[yLast][xLast];
					grid[yCur][xCur] = 0;
					emptyBlock++;					
					moved = true;
				}
				else if (xCur != xLast + firstOffset[direction][0]
					|| yCur != yLast + firstOffset[direction][1]) {
					grid[yLast + firstOffset[direction][1]][xLast + firstOffset[direction][0]] = grid[yCur][xCur];
					grid[yCur][xCur] = 0;
					moved = true;
				}
				if (!empty) {
					xLast += firstOffset[direction][0];
					yLast += firstOffset[direction][1];
				}
			}	
		}
	}
	return  moved;
}


int main(){
	initgraph(500, 500);
	ReadImage();
	addRandomNum(2);
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
		if (direction != -1 && move(direction)) {
			addRandomNum(1);
			cleardevice();
			draw();
		}
	
	}
	closegraph();
}


