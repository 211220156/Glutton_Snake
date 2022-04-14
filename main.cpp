#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iostream>
using namespace std;
int GetRandomNum(int num);
void SetColor(int num);
void SetCursorPosition(int x, int y);
int main();
int direction[4][2] = {
	{0,-1},//上
	{0, 1},//下
	{-1,0},//左
	{1, 0}//右
};
int xb = 20, yb = 12;//地图大小
int map[32][32] = { 0 };
bool random_map = true;//是否随机生成地图
bool speed_up = true;//长按加速功能
char theme[6][5] = { "蓝白","红白","绿白","科幻","白红","黑白" };
int color = 0;//0:蓝白 1:红白 2:绿白 3:科幻 4:白红 5:黑白

bool speed_status = false;//判断是否处于加速状态

char CHN_choice[2][3] = { "否","是" };
int food = 0;
int length = 4;
int speed = 45; //介于30到55之间，下方控制循环次数60-speed：5~30

int curr_direction = -10;//当前方向
COORD head;//记录蛇头位置
void CreateFood()
{
	int x=GetRandomNum(xb)+1, y=GetRandomNum(yb)+1;//随机位置生成食物
	while (map[x][y]!=0)	{ x = GetRandomNum(xb) + 1; y = GetRandomNum(yb) + 1; }//若该位置非空，重新生成
	SetCursorPosition(x, y);
	map[x][y] = -5;//代表食物的位置
	food = 1;
	switch (color)
	{
	case 0:
	{
		SetColor(9);
		break;
	}
	case 1:
	{
		SetColor(12);
		break;
	}
	case 2:
	{
		SetColor(10);
		break;
	}
	case 3:
	{
		SetColor(3);
		break;
	}
	case 4:
	{
		SetColor(4);
		break;
	}
	case 5:
	{
		SetColor(15);
		break;
	}
	}
	cout << "●";
}
int GetRandomNum(int num)
{
	return rand() % num;
}
void SetCursorPosition(int x, int y)//设置光标位置
{
	COORD c;
	c.X = x * 2;//因为方格占两个位置，所以要*2
	c.Y = y+1;//最上方为状态栏，留一行空位
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void SetColor(int num)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);//设置输出颜色
}
void View()
{
	system("cls");
	system("color 0F");
	SetCursorPosition(10, 0); SetColor(189); cout << "■■■■■■■■■■■■■■■■■■■■■■■■■";
	SetCursorPosition(10, 1); cout << "■                                              ■";
	SetCursorPosition(10, 2); cout << "■            Gluttonous Snake                  ■";
	SetCursorPosition(10, 3); cout << "■                                              ■";
	SetCursorPosition(10, 4); cout << "■                贪吃蛇                        ■";
	SetCursorPosition(10, 5); cout << "■                                              ■";
	SetCursorPosition(10, 6); cout << "■            Developer:XJ-Gao                  ■";
	SetCursorPosition(10, 7); cout << "■■■■■■■■■■■■■■■■■■■■■■■■■";
}
bool JudgeEnd()//若地图没有空位了，则胜利
{
	for (int i = 1; i <= xb; i++)
		for (int j = 1; j <= yb; j++)
			if (map[i][j] == 0)
				return false;
	return true;
}
bool JudgeLegitimacy()//若生成的地图有存在一个位置其他三面都是墙，则判为不合法
{
	for (int i=1;i<=xb;i++)
		for (int j=1;j<=yb;j++)
			if (map[i][j] == 0)
			{
				if (map[i - 1][j] + map[i + 1][j] + map[i][j + 1] + map[i][j - 1] <= -3)
					return false;
			}
	return true;
}
void CreateMap()
{
	for (int i = 0; i <= xb + 1; i++)
		for (int j = 0; j <= yb + 1; j++)
			if (i == 0 || j == 0 || i == xb + 1 || j == yb + 1) map[i][j] = -1;	else map[i][j] = 0;
	int y=GetRandomNum(3)+1;
	int ran1, ran2;
	for (int j = y; j <= yb; j += GetRandomNum(3) + 1)
	{
		ran1 = GetRandomNum(xb) + 1;
		ran2 = GetRandomNum(int(xb / 3)) + 1;
		for (int i = ran1; i <= min(ran1 + ran2, xb); i++)	map[i][j] = -1;
	}
	if (!JudgeLegitimacy())//若不合法，重新生成地图
		CreateMap();
}

void PrintMap()
{
	switch (color)
	{
	case 0:
	{
	}
	case 1:
	{
	}
	case 2:
	{
		SetColor(143);
		break;
	}
	case 3:
	{
		SetColor(25);
		break;
	}
	case 4:
	{
		SetColor(76);
		break;
	}
	case 5:
	{
		SetColor(127);
		break;
	}
	}
	for (int i = 0; i <= xb + 1; i++)
		for (int j = 0; j <= yb + 1; j++)
			if (map[i][j] == -1)
			{
				SetCursorPosition(i, j);
				cout << "■";
			}
}
bool MovingSnake()
{
	head.X += direction[curr_direction][0];
	head.Y += direction[curr_direction][1];
	if (map[head.X][head.Y] != 0 && map[head.X][head.Y] != -5)//若蛇头处既不是食物也不是空地，将退出
	{
		return false;
	}
	if (map[head.X][head.Y] == -5)//吃到了食物，蛇身数值不需要改变，只需要加上蛇头
	{
		length++;
		food = 0;
		SetCursorPosition(13, -1);
		SetColor(15);
		cout << "当前长度：" << length;
	}
	else
	{
		for (int i = 1; i <= xb; i++)
			for (int j = 1; j <= yb; j++)
				if (map[i][j] > 0)
				{
					map[i][j]--;
					if (map[i][j] == 0)
					{
						SetCursorPosition(i, j);
						SetColor(0);
						cout << "  ";
					}
					else
					{
						SetCursorPosition(i, j);
						if (color == 0)	SetColor(159);
						if (color == 1)	SetColor(79);
						if (color == 2)	SetColor(175);
						if (color == 3)	SetColor(159);
						if (color == 4)	SetColor(79);
						if (color == 5)	SetColor(120);
						cout << "■";
					}
				}
	}
	map[head.X][head.Y] = length;
	SetCursorPosition(head.X, head.Y);//输出蛇头
	if (color == 0)	SetColor(63);
	if (color == 1)	SetColor(79);
	if (color == 2)	SetColor(175);
	if (color == 3)	SetColor(159);
	if (color == 4)	SetColor(79);
	if (color == 5)	SetColor(120);
	cout << "■";
	return true;
}
int game()
{
	system("cls"); 
	system("color 0F");
	length = 4;
	for (int i = 0; i <= xb + 1; i++)
		for (int j = 0; j <= yb + 1; j++)
			if (i == 0 || j == 0 || i == xb + 1 || j == yb + 1) map[i][j] = -1;//初始化地图	
			else map[i][j] = 0;

	if (random_map)//判断是否开启了随机生成地图的功能
		CreateMap();

	PrintMap();//打印地图
	CreateFood();//生成食物
	curr_direction = -10;//设置小一点，防止下面判断出错
	int prev_direction = curr_direction,key=0;//记录之前的方向，判断有没有改变
	bool input = false;//判断有没有按键输入
	int real_speed=speed;//显示真实的速度，若处于加速状态则让此变量为55，处于最快状态
	head.X = GetRandomNum(xb) + 1;//随机生成蛇头
	head.Y = GetRandomNum(yb) + 1;
	while (map[head.X][head.Y]!=0)	{
		head.X = GetRandomNum(xb) + 1;
		head.Y = GetRandomNum(yb) + 1;
	}//如果蛇头处不为空，重新生成
	map[head.X][head.Y] = length;//初始化头部位置
	SetCursorPosition(head.X, head.Y);
	if (color == 0)	SetColor(63);
	if (color == 1)	SetColor(79);
	if (color == 2)	SetColor(175);
	if (color == 3)	SetColor(159);
	if (color == 4)	SetColor(79);
	if (color == 5)	SetColor(120);
	cout << "■";
	SetCursorPosition(5, -1); 
	SetColor(15);
	cout << "当前速度：" << real_speed;
	cout << "    当前长度：" << length;
	while (!_kbhit()) { Sleep(50); }//若玩家没有输入，则等待
	while (true)
	{
		prev_direction = curr_direction;
		key = 0;
		input = false;
		real_speed = speed;
		if (speed_status)//判断是否处于加速状态
		{
			real_speed = 55;
			SetCursorPosition(5, -1);
			SetColor(15);
			cout << "加速中！    " ;
		}
		else
		{
			SetCursorPosition(5, -1);
			SetColor(15);
			cout << "当前速度：" << real_speed;
		}

		for (int i = 1; i <= 60 - real_speed; i++)
		{
			if (_kbhit() && input == false)//若有输入且当前轮次没有读取过输入，则读取
			{
				input = true;
				key = _getch();
				if (key == 224)
				{
					key = _getch();
					if (key == 72) curr_direction = 0;//上 
					if (key == 80) curr_direction = 1;//下 
					if (key == 75) curr_direction = 2;//左 
					if (key == 77) curr_direction = 3;//右	
				}
			}
			Sleep(5);//放慢点读取输入的循环的执行速度，优化手感
		}
		if (curr_direction == prev_direction && speed_up && input)//如果当前轮次有输入且方向没变且开启了加速功能，则加速状态开启
			speed_status = true;
		else
			speed_status = false;
		if (curr_direction != prev_direction && (curr_direction + prev_direction == 1 || curr_direction + prev_direction == 5))	curr_direction = prev_direction;
		//若当前方向与上一回合方向相反，则此次输入无效
		if (!MovingSnake())//说明游戏结束了
		{
			if (JudgeEnd())
			{
				SetCursorPosition(5, yb + 2);
				SetColor(160);
				cout << "恭喜！你赢了！！";
			}
			else
			{
				SetCursorPosition(5, yb + 2);
				SetColor(192);
				cout << "很遗憾，你输了！！";
			}
			SetCursorPosition(5, yb + 4);
			SetColor(240); cout << "你本次的成绩为：" << length;
			SetCursorPosition(3, yb + 6);
			SetColor(15); cout << ">>";
			SetColor(240); cout << "再玩一局";
			SetCursorPosition(3, yb + 8);
			SetColor(15);	cout << "  返回主界面";
			key = 0;
			int player_choice=0;
			while (key != 13)
			{
				key = _getch();
				if (key == 224)
				{
					key = _getch();
					if (key == 72 || key == 80)
					{
						player_choice = 1 - player_choice;
						if (player_choice == 0)
						{
							SetCursorPosition(3, yb + 6);
							SetColor(15); cout << ">>";
							SetColor(240); cout << "再玩一局";
							SetCursorPosition(3, yb + 8);
							SetColor(15);	cout << "  返回主界面";
						}
						else
						{
							SetCursorPosition(3, yb + 6);
							SetColor(15); cout << "  ";
							cout << "再玩一局";
							SetCursorPosition(3, yb + 8);
							SetColor(15);	cout << ">>";
							SetColor(240);  cout << "返回主界面";
						}
					}
				}
			}
			return player_choice;
		}
		if (food == 0)
			CreateFood();
	}
	return 1;
}

void settings()
{
	View();
	SetColor(15);
	SetCursorPosition(15, 9); cout << "   按ESC返回主界面   ";
	SetCursorPosition(13, 12); cout << ">>" << "  " << "地图左右宽度";
	SetCursorPosition(15, 13); cout << "地图上下长度";
	SetCursorPosition(15, 14); cout << "移动速度";
	SetCursorPosition(15, 15); cout << "随机生成障碍";
	SetCursorPosition(15, 16); cout << "开启长按加速功能";
	SetCursorPosition(15, 17); cout << "主题选择";

	SetColor(240); SetCursorPosition(30, 12); cout << "<" << xb << ">";
	SetColor(15); SetCursorPosition(30, 13); cout << "<" << yb << ">";
	SetCursorPosition(30, 14); cout << "<" << speed << ">";
	SetCursorPosition(30, 15); cout << "<" << CHN_choice[random_map] << ">";
	SetCursorPosition(30, 16); cout << "<" << CHN_choice[speed_up] << ">";
	SetCursorPosition(30, 17); cout << "<" << theme[color] << ">";

	int key = 0;
	int choice = 0;
	while (key != 27)
	{
		key = _getch();
		if (key == 224)
		{
			key = _getch();
			switch (key)
			{
			case 72:
			{
				if (choice > 0) {
					SetCursorPosition(13, 12 + choice); cout << "  ";
					SetCursorPosition(30, 12 + choice); cout << "         ";
					SetColor(15);
					SetCursorPosition(30, 12); cout << "<" << xb << ">";
					SetCursorPosition(30, 13); cout << "<" << yb << ">";
					SetCursorPosition(30, 14); cout << "<" << speed << ">";
					SetCursorPosition(30, 15); cout << "<" << CHN_choice[random_map] << ">";
					SetCursorPosition(30, 16); cout << "<" << CHN_choice[speed_up] << ">";
					SetCursorPosition(30, 17); cout << "<" << theme[color] << ">";
					choice--; 
					SetCursorPosition(13, 12 + choice); cout << ">>";
					SetColor(240); SetCursorPosition(30, 12+choice);
					switch (choice)
					{
					case 0:
					{
						cout << "<" << xb << ">";
						break;
					}
					case 1:
					{
						cout << "<" << yb << ">";
						break;
					}
					case 2:
					{
						cout << "<" << speed << ">";
						break;
					}
					case 3:
					{
						cout << "<" << CHN_choice[random_map] << ">";
						break;
					}
					case 4:
					{
						cout << "<" << CHN_choice[speed_up] << ">";
						break;
					}
					case 5:
					{
						cout << "<" << theme[color] << ">";
						break;
					}
					}
					SetColor(15);
				}
				break;
			}
			case 80:
			{
				if (choice < 5) {
					SetCursorPosition(13, 12 + choice); cout << "  ";
					SetCursorPosition(30, 12 + choice); cout << "     ";
					SetColor(15);
					SetCursorPosition(30, 12); cout << "<" << xb << ">";
					SetCursorPosition(30, 13); cout << "<" << yb << ">";
					SetCursorPosition(30, 14); cout << "<" << speed << ">";
					SetCursorPosition(30, 15); cout << "<" << CHN_choice[random_map] << ">";
					SetCursorPosition(30, 16); cout << "<" << CHN_choice[speed_up] << ">";
					SetCursorPosition(30, 17); cout << "<" << theme[color] << ">";
					choice++; 
					SetCursorPosition(13, 12 + choice); cout << ">>";
					SetColor(240); SetCursorPosition(30, 12 + choice);
					switch (choice)
					{
					case 0:
					{
						cout << "<" << xb << ">";
						break;
					}
					case 1:
					{
						cout << "<" << yb << ">";
						break;
					}
					case 2:
					{
						cout << "<" << speed << ">";
						break;
					}
					case 3:
					{
						cout << "<" << CHN_choice[random_map] << ">";
						break;
					}
					case 4:
					{
						cout << "<" << CHN_choice[speed_up] << ">";
						break;
					}
					case 5:
					{
						cout << "<" << theme[color] << ">";
						break;
					}
					}
					SetColor(15);
				}
				break;
			}
			case 75://左
			{
				SetColor(240); SetCursorPosition(30, 12 + choice);
				switch (choice)
				{
				case 0:
				{
					if (xb > 10)	xb--;
					cout << "<" << xb << ">";
					break;
				}
				case 1:
				{
					if (yb > 8)	yb--;
					cout << "<" << yb << ">";
					break;
				}
				case 2:
				{
					if (speed > 30)	speed -= 1;
					cout << "<" << speed << ">";
					break;
				}
				case 3:
				{
					random_map = 1 - random_map;
					cout << "<" << CHN_choice[random_map] << ">";
					break;
				}
				case 4:
				{
					speed_up = 1 - speed_up;
					cout << "<" << CHN_choice[speed_up] << ">";
					break;
				}
				case 5:
				{
					color--;
					color = (color + 6) % 6;
					cout << "<" << theme[color] << ">";
					break;
				}
				}
				SetColor(15);
				break;
			}
			case 77://右
			{
				SetColor(240); SetCursorPosition(30, 12 + choice);
				switch (choice)
				{
				case 0:
				{
					if (xb < 30)	xb++;
					cout << "<" << xb << ">";
					break;
				}
				case 1:
				{
					if (yb < 20)	yb++;
					cout << "<" << yb << ">";
					break;
				}
				case 2:
				{
					if (speed < 55)	speed += 1;
					cout << "<" << speed << ">";
					break;
				}
				case 3:
				{
					random_map = 1 - random_map;
					cout << "<" << CHN_choice[random_map] << ">";
					break;
				}
				case 4:
				{
					speed_up = 1 - speed_up;
					cout << "<" << CHN_choice[speed_up] << ">";
					break;
				}
				case 5:
				{
					color++;
					color = (color + 6) % 6;
					cout << "<" << theme[color] << ">";
					break;
				}
				}
				SetColor(15);
				break;
			}
			}
		}
	}
	main();
}
int main()
{
	View();
	CONSOLE_CURSOR_INFO info = { 1,false };//初始化设置光标信息
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);//隐藏光标

	srand(time(0));//生成随机数种子
	SetCursorPosition(17, 10); SetColor(240); cout  << "   ·开始游戏   ";
	SetCursorPosition(17, 12); SetColor(15); cout  << "   ·游戏设置   ";

	int choice = 0, key = 0;
	while (key != 13)
	{
		key = _getch();
		if (key == 224)
		{
			key = _getch();
			if (key == 72 || key == 80)	choice = 1 - choice;
			if (choice == 0)
			{
				SetCursorPosition(17, 10); SetColor(240); cout << "   ·开始游戏   ";
				SetCursorPosition(17, 12); SetColor(15); cout << "   ·游戏设置   ";
			}
			else
			{
				SetCursorPosition(17, 10); SetColor(15); cout << "   ·开始游戏   ";
				SetCursorPosition(17, 12); SetColor(240); cout << "   ·游戏设置   ";
			}
		}
	}

	if (choice == 1)
	{
		settings();
	}

	while (true)
	{
		if (game() == 1)//游戏结束会返回一个值，0表示再来一局，1为返回主界面，此时再次进入main函数即可。
			main();
	}
	return 0;
}