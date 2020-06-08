#include <graphics.h>
#include <cstdio>
#define SPACEING  15 //������ 
#define BLOCK_SIZE 105 //ʹ��������Ӷ��� ����ߴ� 
#define NUM_BLOCK 16//2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16834 32768 65536


PIMAGE blockImgs[NUM_BLOCK], backgroundImg;//�⺯��Ŀ¼-��ͼ������غ���-��PIMAGE ����ͼƬ���� 
int grid[4][4];//4*4������Ϸ����
int emptyBlock = 16; //�ո������� 


void ReadImage()//��ȡ��ϷԪ����������ͼƬ 
{
	char imgName[100];
//��ȡ���� 
	for (int i = 1, value = 2; i < NUM_BLOCK; i++, value *= 2) {
		blockImgs[i] = newimage();
		sprintf(imgName, "resource\\image\\block_%d.png", value);
		getimage(blockImgs[i], imgName);
	}
	backgroundImg = newimage();
	getimage(backgroundImg, "resource\\image\\background.png");
}
void addNum(int n)//����һ��2 
{	
	for (int i = 0; i < n; ++i) {
		if (emptyBlock == 0)//û�пռ����������� 
			break;
		int r = rand() % emptyBlock--;		//���ѡȡ��ǰ��һ����λ 
		int* ptr = &grid[0][0] - 1;
		int j=0;

		do {								
			while (*++ptr);
		} while (j++<r);

		*ptr = 1;//�ڵ�ǰ��λ����2		
	}
}
void draw()//������Ϸ���� 
{
	putimage_withalpha(NULL, backgroundImg, 0, 0);//��ͼ�񻺴���Ƶ�������

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = (j + 1) * SPACEING + j * BLOCK_SIZE;
			int y =  (i + 1) * SPACEING + i * BLOCK_SIZE;

			if (grid[i][j] != 0)
				putimage_withalpha(NULL, blockImgs[grid[i][j]], x, y);//�������뱳��ͼƬ�ķ������ 
		}
	}
}
bool move(int direction)//�ƶ����ϲ�����ֹ 
{
	//λ�ü�ⳣ��  ��ĳ�����ƶ����ڴ�ĳ����ʼ��� 
	static int x0[4] = { 0, 0, 3, 0 }, y0[4] = { 0, 0, 0, 3 };  
	static int IniValue[4][2] = { {1, 0},{0, 1},{-1, 0}, {0, -1} };
	static int NxtValue[4][2] = { {0, 1}, {1, 0}, {0, 1}, {1, 0} };//����Ϊ1������Ϊ-1 
	bool moved = false;//��ʼ״̬δ�ƶ� 
	for (int i = 0; i < 4; i++) {
		int xMovTar = x0[direction] + i * NxtValue[direction][0];
		int yMovTar = y0[direction] + i * NxtValue[direction][1];
		int xIniTar = xMovTar, yIniTar = yMovTar;
		bool empty;
		for (int j = 1; j < 4; j++) {
			empty = false;
			xIniTar += IniValue[direction][0];
			yIniTar += IniValue[direction][1];		
			if (grid[yIniTar][xIniTar] != 0) {//���ַǿո� 
				if (grid[yMovTar][xMovTar] == 0) {//�ƶ�Ŀ���ǿո� 
					grid[yMovTar][xMovTar] = grid[yIniTar][xIniTar];//������ֵ 
					grid[yIniTar][xIniTar] = 0;
					moved = empty = true; 
				}
				else if (grid[yIniTar][xIniTar] == grid[yMovTar][xMovTar]) {//�ϲ� ���ƶ�Ŀ����� 
					++grid[yMovTar][xMovTar];//�ƶ�Ŀ����ֵ*2 
					grid[yIniTar][xIniTar] = 0;//ԭʼĿ��ֵΪ0 
					emptyBlock++;//�ո���+1					
					moved = true;
				}
				else if (xIniTar != xMovTar + IniValue[direction][0]
					|| yIniTar != yMovTar + IniValue[direction][1]) {//������ͬ���м��пո��ƶ����ո�ԭʼĿ���Ϊ0 
					grid[yMovTar + IniValue[direction][1]][xMovTar + IniValue[direction][0]] = grid[yIniTar][xIniTar];
					grid[yIniTar][xIniTar] = 0;
					moved = true;
				}
				if (!empty) {//��������ͬ�����ڣ����ƶ� 
					xMovTar += IniValue[direction][0];
					yMovTar += IniValue[direction][1];
				}
			}	
		}
	}
	return  moved;
}
void delImage()//�ͷŵ��õ���ͼƬ��Դ 
{
	for (int i = 0; i < NUM_BLOCK; i++)
		delimage(blockImgs[i]);
	delimage(backgroundImg);
}


int main(){
	initgraph(500, 500);
	ReadImage();//��ȡͼƬ��Դ 
	addNum(2);//��ʼ����2��2
	draw(); 
	
	for (; is_run(); delay_fps(60)) {
	//���������룬�����ƶ�����  
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
		if (direction != -1 && move(direction)) {//��� 
			addNum(1);//����һ��2
			draw();
		}
	
	}
	delImage();
	closegraph();
}


