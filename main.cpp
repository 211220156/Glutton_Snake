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
	{0,-1},//��
	{0, 1},//��
	{-1,0},//��
	{1, 0}//��
};
int xb = 20, yb = 12;//��ͼ��С
int map[32][32] = { 0 };
bool random_map = true;//�Ƿ�������ɵ�ͼ
bool speed_up = true;//�������ٹ���
char theme[6][5] = { "����","���","�̰�","�ƻ�","�׺�","�ڰ�" };
int color = 0;//0:���� 1:��� 2:�̰� 3:�ƻ� 4:�׺� 5:�ڰ�

bool speed_status = false;//�ж��Ƿ��ڼ���״̬

char CHN_choice[2][3] = { "��","��" };
int food = 0;
int length = 4;
int speed = 45; //����30��55֮�䣬�·�����ѭ������60-speed��5~30

int curr_direction = -10;//��ǰ����
COORD head;//��¼��ͷλ��
void CreateFood()
{
	int x=GetRandomNum(xb)+1, y=GetRandomNum(yb)+1;//���λ������ʳ��
	while (map[x][y]!=0)	{ x = GetRandomNum(xb) + 1; y = GetRandomNum(yb) + 1; }//����λ�÷ǿգ���������
	SetCursorPosition(x, y);
	map[x][y] = -5;//����ʳ���λ��
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
	cout << "��";
}
int GetRandomNum(int num)
{
	return rand() % num;
}
void SetCursorPosition(int x, int y)//���ù��λ��
{
	COORD c;
	c.X = x * 2;//��Ϊ����ռ����λ�ã�����Ҫ*2
	c.Y = y+1;//���Ϸ�Ϊ״̬������һ�п�λ
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void SetColor(int num)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);//���������ɫ
}
void View()
{
	system("cls");
	system("color 0F");
	SetCursorPosition(10, 0); SetColor(189); cout << "��������������������������������������������������";
	SetCursorPosition(10, 1); cout << "��                                              ��";
	SetCursorPosition(10, 2); cout << "��            Gluttonous Snake                  ��";
	SetCursorPosition(10, 3); cout << "��                                              ��";
	SetCursorPosition(10, 4); cout << "��                ̰����                        ��";
	SetCursorPosition(10, 5); cout << "��                                              ��";
	SetCursorPosition(10, 6); cout << "��            Developer:XJ-Gao                  ��";
	SetCursorPosition(10, 7); cout << "��������������������������������������������������";
}
bool JudgeEnd()//����ͼû�п�λ�ˣ���ʤ��
{
	for (int i = 1; i <= xb; i++)
		for (int j = 1; j <= yb; j++)
			if (map[i][j] == 0)
				return false;
	return true;
}
bool JudgeLegitimacy()//�����ɵĵ�ͼ�д���һ��λ���������涼��ǽ������Ϊ���Ϸ�
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
	if (!JudgeLegitimacy())//�����Ϸ����������ɵ�ͼ
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
				cout << "��";
			}
}
bool MovingSnake()
{
	head.X += direction[curr_direction][0];
	head.Y += direction[curr_direction][1];
	if (map[head.X][head.Y] != 0 && map[head.X][head.Y] != -5)//����ͷ���Ȳ���ʳ��Ҳ���ǿյأ����˳�
	{
		return false;
	}
	if (map[head.X][head.Y] == -5)//�Ե���ʳ�������ֵ����Ҫ�ı䣬ֻ��Ҫ������ͷ
	{
		length++;
		food = 0;
		SetCursorPosition(13, -1);
		SetColor(15);
		cout << "��ǰ���ȣ�" << length;
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
						cout << "��";
					}
				}
	}
	map[head.X][head.Y] = length;
	SetCursorPosition(head.X, head.Y);//�����ͷ
	if (color == 0)	SetColor(63);
	if (color == 1)	SetColor(79);
	if (color == 2)	SetColor(175);
	if (color == 3)	SetColor(159);
	if (color == 4)	SetColor(79);
	if (color == 5)	SetColor(120);
	cout << "��";
	return true;
}
int game()
{
	system("cls"); 
	system("color 0F");
	length = 4;
	for (int i = 0; i <= xb + 1; i++)
		for (int j = 0; j <= yb + 1; j++)
			if (i == 0 || j == 0 || i == xb + 1 || j == yb + 1) map[i][j] = -1;//��ʼ����ͼ	
			else map[i][j] = 0;

	if (random_map)//�ж��Ƿ�����������ɵ�ͼ�Ĺ���
		CreateMap();

	PrintMap();//��ӡ��ͼ
	CreateFood();//����ʳ��
	curr_direction = -10;//����Сһ�㣬��ֹ�����жϳ���
	int prev_direction = curr_direction,key=0;//��¼֮ǰ�ķ����ж���û�иı�
	bool input = false;//�ж���û�а�������
	int real_speed=speed;//��ʾ��ʵ���ٶȣ������ڼ���״̬���ô˱���Ϊ55���������״̬
	head.X = GetRandomNum(xb) + 1;//���������ͷ
	head.Y = GetRandomNum(yb) + 1;
	while (map[head.X][head.Y]!=0)	{
		head.X = GetRandomNum(xb) + 1;
		head.Y = GetRandomNum(yb) + 1;
	}//�����ͷ����Ϊ�գ���������
	map[head.X][head.Y] = length;//��ʼ��ͷ��λ��
	SetCursorPosition(head.X, head.Y);
	if (color == 0)	SetColor(63);
	if (color == 1)	SetColor(79);
	if (color == 2)	SetColor(175);
	if (color == 3)	SetColor(159);
	if (color == 4)	SetColor(79);
	if (color == 5)	SetColor(120);
	cout << "��";
	SetCursorPosition(5, -1); 
	SetColor(15);
	cout << "��ǰ�ٶȣ�" << real_speed;
	cout << "    ��ǰ���ȣ�" << length;
	while (!_kbhit()) { Sleep(50); }//�����û�����룬��ȴ�
	while (true)
	{
		prev_direction = curr_direction;
		key = 0;
		input = false;
		real_speed = speed;
		if (speed_status)//�ж��Ƿ��ڼ���״̬
		{
			real_speed = 55;
			SetCursorPosition(5, -1);
			SetColor(15);
			cout << "�����У�    " ;
		}
		else
		{
			SetCursorPosition(5, -1);
			SetColor(15);
			cout << "��ǰ�ٶȣ�" << real_speed;
		}

		for (int i = 1; i <= 60 - real_speed; i++)
		{
			if (_kbhit() && input == false)//���������ҵ�ǰ�ִ�û�ж�ȡ�����룬���ȡ
			{
				input = true;
				key = _getch();
				if (key == 224)
				{
					key = _getch();
					if (key == 72) curr_direction = 0;//�� 
					if (key == 80) curr_direction = 1;//�� 
					if (key == 75) curr_direction = 2;//�� 
					if (key == 77) curr_direction = 3;//��	
				}
			}
			Sleep(5);//�������ȡ�����ѭ����ִ���ٶȣ��Ż��ָ�
		}
		if (curr_direction == prev_direction && speed_up && input)//�����ǰ�ִ��������ҷ���û���ҿ����˼��ٹ��ܣ������״̬����
			speed_status = true;
		else
			speed_status = false;
		if (curr_direction != prev_direction && (curr_direction + prev_direction == 1 || curr_direction + prev_direction == 5))	curr_direction = prev_direction;
		//����ǰ��������һ�غϷ����෴����˴�������Ч
		if (!MovingSnake())//˵����Ϸ������
		{
			if (JudgeEnd())
			{
				SetCursorPosition(5, yb + 2);
				SetColor(160);
				cout << "��ϲ����Ӯ�ˣ���";
			}
			else
			{
				SetCursorPosition(5, yb + 2);
				SetColor(192);
				cout << "���ź��������ˣ���";
			}
			SetCursorPosition(5, yb + 4);
			SetColor(240); cout << "�㱾�εĳɼ�Ϊ��" << length;
			SetCursorPosition(3, yb + 6);
			SetColor(15); cout << ">>";
			SetColor(240); cout << "����һ��";
			SetCursorPosition(3, yb + 8);
			SetColor(15);	cout << "  ����������";
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
							SetColor(240); cout << "����һ��";
							SetCursorPosition(3, yb + 8);
							SetColor(15);	cout << "  ����������";
						}
						else
						{
							SetCursorPosition(3, yb + 6);
							SetColor(15); cout << "  ";
							cout << "����һ��";
							SetCursorPosition(3, yb + 8);
							SetColor(15);	cout << ">>";
							SetColor(240);  cout << "����������";
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
	SetCursorPosition(15, 9); cout << "   ��ESC����������   ";
	SetCursorPosition(13, 12); cout << ">>" << "  " << "��ͼ���ҿ��";
	SetCursorPosition(15, 13); cout << "��ͼ���³���";
	SetCursorPosition(15, 14); cout << "�ƶ��ٶ�";
	SetCursorPosition(15, 15); cout << "��������ϰ�";
	SetCursorPosition(15, 16); cout << "�����������ٹ���";
	SetCursorPosition(15, 17); cout << "����ѡ��";

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
			case 75://��
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
			case 77://��
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
	CONSOLE_CURSOR_INFO info = { 1,false };//��ʼ�����ù����Ϣ
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);//���ع��

	srand(time(0));//�������������
	SetCursorPosition(17, 10); SetColor(240); cout  << "   ����ʼ��Ϸ   ";
	SetCursorPosition(17, 12); SetColor(15); cout  << "   ����Ϸ����   ";

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
				SetCursorPosition(17, 10); SetColor(240); cout << "   ����ʼ��Ϸ   ";
				SetCursorPosition(17, 12); SetColor(15); cout << "   ����Ϸ����   ";
			}
			else
			{
				SetCursorPosition(17, 10); SetColor(15); cout << "   ����ʼ��Ϸ   ";
				SetCursorPosition(17, 12); SetColor(240); cout << "   ����Ϸ����   ";
			}
		}
	}

	if (choice == 1)
	{
		settings();
	}

	while (true)
	{
		if (game() == 1)//��Ϸ�����᷵��һ��ֵ��0��ʾ����һ�֣�1Ϊ���������棬��ʱ�ٴν���main�������ɡ�
			main();
	}
	return 0;
}