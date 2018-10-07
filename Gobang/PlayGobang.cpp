#include "PlayGobang.h"

const int BLACK = 1;     //�ڷ�
const int WHITE = 2;     //�׷�
const int DEFAULT = -1;  //Ĭ�ϱ��
const int WIN = 0;      //Ӯ�ı��λ
const int FORBIDDEN = 3; //���ֱ�� 
const int LR = 5;  //���ҷ���
const int TB = 6;   //���·���
const int LB = 7;   //����
const int RB = 8;   //����
int forbiddenLineCount = 1;  //�ڷ�һ������������һ�����ӵ�������������
int emptyCount = 0;          //���λ
int direct[4] = { LR,LB,TB,RB};  //���ڱ�������
int nowRole = WHITE;            //��ǰ��ɫ
int lineCount = 1;             //�������ӵ�����
int edgeNotEmpty = 0;          //��Ե�뷴���Ӵ�����������
PlayGobang::PlayGobang(Board * board, Data * data, Player * player)
	:board(board),data(data),player(player)
{
	board->resetBoard(data);
}


PlayGobang::~PlayGobang()
{
}
void PlayGobang::start()
{
	resetBoard();
	int x = 0, y = 0;
	int inputX = 0;
	int inputY = 0;
	int state = DEFAULT;
	do
	{
		if (nowRole == BLACK)
		{
			cout << "�ڷ�������:" << endl;
			cin >> inputX;
			if (inputX < 0 || inputX>15)
			{
				if (inputX == -1)
				{
					return;
				}
				cout << "�����в�����Ҫ����������" << endl;
				continue;
			}
			cout << "�ڷ������У�" << endl;
			cin >> inputY;
			if (inputY < 0 || inputY>15)
			{
				if (inputY == -1)
				{
					return;
				}
				cout << "�����в�����Ҫ����������" << endl;
				continue;
			}
		}
		else if (nowRole == WHITE)

		{
			cout << "�׷�������:" << endl;
			cin >> inputX;
			if (inputX < 0 || inputX>15)
			{
				if (inputX == -1)
				{
					return;
				}
				cout << "�����в�����Ҫ����������" << endl;
				continue;
			}
			cout << "�׷������У�" << endl;
			cin >> inputY;

			if (inputY<0||inputY>15)
			{
				if (inputY == -1)
				{
					return;
				}
				cout << "�����в�����Ҫ����������" << endl;
				continue;
			}
		}
		x = inputX - 1;
		y = inputY - 1;
		if (data->getPos(x, y) == BLACK || data->getPos(x, y) == WHITE)
		{
			cout << "��λ���ѱ�ռ��" << endl;
			continue;
		}
		 putChess(x, y, nowRole);
		 printChessBoard();
		 switch (test(x,y,nowRole))
		 {
		 case WIN: {
			 player->win(nowRole);
			 cout << "����1����,����-1�˳�" << endl;
			 cin >> inputX;
			 if (inputX == 1)
			 {
				 resetBoard();
				 continue;
			 }
			 else if (inputX == -1)
			 {
				 return;
			 }
			
		 }
		 case FORBIDDEN: {
			 player->lost(BLACK);
			 cout << "����1����,����0������һ���ӣ�����-1�˳�" << endl;
			 cin >> inputX;
			 if (inputX == 1)
			 {
				 resetBoard();
				 continue;
			 }
			 else if (inputX == -1)
			 {
				 return;
			 }
			 else if (inputX == 0)
			 {
				 board->remove(x, y,data);
				 printChessBoard();
				 continue;
			 }
		 }
		 default:
			 break;
		 }
	nowRole = (nowRole == BLACK ? WHITE : BLACK);

	} while (1);
}


void PlayGobang::resetBoard()
{
	board->resetBoard(data);
	data->setPosWithoutInput(6, 6, BLACK);
	board->printBoard(data);
}


//��ʾ���̺����ӽ���
void PlayGobang::printChessBoard(void)
{
	board->printBoard(data);
}


//���㾭���ҵ�ĳ������������ɫ��ͬ������������Ŀ
void PlayGobang::continousChessCount(int x, int y, int direct,int convert,int role,int limitCount)
{
	
	switch (direct)
	{
	case LR:x += convert;
		break;
	case TB:y += convert;
		break;
	case LB: {
		y += convert;
		x += convert;
	}
			 break;
	case RB: {
		y -= convert;
		x += convert;
	}
			 break;
	default:
		break;
	}

	if (x>=0&&y>=0&&x<15&&y<15)
	{
		if (data->getPos(x,y) == role)
		{
			lineCount+=1;
			if (lineCount == limitCount)
			{
				return;
			}
			continousChessCount(x, y, direct, convert, role,limitCount);
		}
		else if (data->getPos(x,y) == (role == BLACK?WHITE:BLACK))
		{
			edgeNotEmpty++;
		}
	}
	else
	{
		edgeNotEmpty++;
	}
}

//������x,y�������Ƿ��Ѿ�ʤ��
int PlayGobang::success(int x, int y)
{
	lineCount = 1;
	for (size_t i = 0; i < 4; i++)
	{
		int d = direct[i];
		continousChessCount(x, y,d, 1, nowRole,6);
		continousChessCount(x, y, d, -1, nowRole,6);
		if (lineCount == 5)
		{
			return WIN;
		}
		lineCount = 1;
	}
	return DEFAULT;
}

//��������
bool PlayGobang::longForbidden(int x, int y)
{
	lineCount = 1;
	for (size_t i = 0; i < 4; i++)
	{
		int d = direct[i];
		continousChessCount(x, y, d, 1, BLACK, 6);
		continousChessCount(x, y, d, -1, BLACK, 6);
		if (lineCount >5)
		{
			return true;
		}
		lineCount = 1;
	}
	return false;
}



//���Ľ���
bool PlayGobang::live4forbidden(int x, int y)
{
	int live4Count = 0;
	emptyCount = 0;
	forbiddenLineCount = 1;
	for (size_t i = 0; i < 4; i++)  //�ַ��������Χ����
	{
		edgeNotEmpty = 0;
		int d = direct[i];
		oneEmptyCount(x, y, d, 1);   
		emptyCount = 0;
		oneEmptyCount(x, y, d, -1);
		emptyCount = 0;
		if (forbiddenLineCount > 3)
		{
			live4Count++;
		}

		if (edgeNotEmpty>1)
		{
			live4Count--;
		}
		forbiddenLineCount = 1;
	}
	if (live4Count>1)
	{
		return true;
	}
	return false;
}

void PlayGobang::oneEmptyCount(int x, int y, int direct, int one)
{
	switch (direct)
	{
	case LR:x += one;
		break;
	case TB:y += one;
		break;
	case LB: {
		y += one;
		x += one;
	}
			 break;
	case RB: {
		y += one;
		x -= one;
	}
			 break;
	default:
		break;
	}

	if (x >= 0 && y >= 0 && x < 15 && y < 15)
	{
		if (data->getPos(x, y) !=WHITE)
		{
			if (data->getPos(x, y) != BLACK)
			{
				emptyCount++;
				if (emptyCount > 1)
				{
					return;
				}
			}
			else
			{
				forbiddenLineCount++;
			}
			oneEmptyCount(x, y, direct, one);
		}
		else
		{
			edgeNotEmpty++;
		}
	}
	else
	{
		edgeNotEmpty++;
	}
}

//��������
bool PlayGobang::live3forbidden(int x, int y)
{
	int live3Count = 0;
	emptyCount = 0;
	forbiddenLineCount = 1;
	int have3line = 0;
	
	for (size_t i = 0; i < 4; i++)
	{
		edgeNotEmpty = 0;
		int d = direct[i];
		oneEmptyCount(x, y, d, 1);
		emptyCount = 0;
		oneEmptyCount(x, y, d, -1);
		emptyCount = 0;
		if (forbiddenLineCount > 2)
		{
			live3Count++;
		}
		if (edgeNotEmpty>0)
		{
			live3Count--;
		}
		forbiddenLineCount = 1;
		lineCount = 1;
		edgeNotEmpty = 0;
		continousChessCount(x, y, d, 1, BLACK, 4);
		continousChessCount(x, y, d, -1, BLACK, 4);
		if (lineCount>2)
		{
			have3line++;
		}

		if (edgeNotEmpty>0)
		{
			have3line--;
		}
	}
	if ((live3Count > 1&&have3line>0)||have3line>1)
	{
		return true;
	}
	return false;
}

//����
int PlayGobang::forbidden(int x, int y)
{
	int state = 0;
	if (longForbidden(x,y))
	{
		state++;
	}

	if (live4forbidden(x,y))
	{
		state++;
	}

	if (live3forbidden(x,y))
	{
		state++;
	}
	if (live4special(x,y))
	{
		state++;
	}
	return state == 0?DEFAULT:FORBIDDEN;
}

//�����ӷ�������ָ��λ�ã��������ֵȴ���
int PlayGobang::putChess(int x, int y, int role)
{

	data->setPos(x, y, role);
	return DEFAULT;
}

bool PlayGobang::live4special(int x,int y)
{
	int live4Count = 0;
	int continueCount = -1;
	for (size_t i = 0; i < 4; i++)  //�ַ��������Χ����
	{
		int d = direct[i];
		emptyCount = 0;
		forbiddenLineCount = 0;
		lineCount = 1;
		oneEmptyCount(x, y, d, 1);
		emptyCount = 0;
		continousChessCount(x, y, d, -1, BLACK, 5);
		continueCount += lineCount;
		if (lineCount + forbiddenLineCount > 3)
		{
			live4Count++;
		}
		lineCount = 1;
		forbiddenLineCount = 0;
		oneEmptyCount(x, y, d, -1);
		emptyCount = 0;
		continousChessCount(x, y, d, 1, BLACK, 5);
		continueCount += lineCount;
		if (lineCount + forbiddenLineCount > 3)
		{
			live4Count++;
		}

		if (continueCount>3)
		{
			return false;
		}
		if (live4Count > 1)
		{
			return true;
		}
		lineCount = 1;
		continueCount = -1;
	}
		return false;
}

//�Զ�������
void PlayGobang::autoTest(void)
{
	int state = DEFAULT;

	//ͼ������
	putChess(2, 2, BLACK);
	putChess(2, 4, BLACK);
	putChess(2, 5, BLACK);
	putChess(1, 3, BLACK);
	putChess(3, 3, BLACK);
	putChess(4, 3, BLACK);
	printChessBoard();
	system("pause");
	if (FORBIDDEN == test(2,3,BLACK))
	{
	putChess(2, 1, WHITE);
	putChess(2, 6, WHITE);
	putChess(2, 3, BLACK);
	printChessBoard();
	system("pause");
	if (DEFAULT == test(2, 3, BLACK))
	{
		cout << "�������Ľ�������1�ɹ�" << endl;
	}
	else
	{
		cout << "�������Ľ�������1ʧ��" << endl;
	}
	}
	else
	{
		cout << "�������Ľ�������1ʧ��" << endl;
	}
	system("pause");
	board->resetBoard(data);

	//ͼ������
	putChess(1, 2, BLACK);
	putChess(1, 4, BLACK);
	putChess(1, 5, BLACK);
	putChess(1, 6, BLACK);
	putChess(1, 7, BLACK);
	system("pause");
	if (FORBIDDEN == test(1, 3, BLACK))
	{
		putChess(1, 3, BLACK);
		printChessBoard();
		cout << "���Գ������ֳɹ�" << endl;
		system("pause");
	}
	board->resetBoard(data);

	//ͼ������
	putChess(3, 4, BLACK);
	putChess(3, 2, BLACK);
	putChess(4, 3, BLACK);
	putChess(2, 3, BLACK);
	printChessBoard();
	system("pause");
	if (FORBIDDEN == test(3, 3, BLACK))
	{
		putChess(3, 5, WHITE);
		putChess(3, 3, BLACK);
		printChessBoard();
		system("pause");
		if (DEFAULT == test(3, 3, BLACK))
		{
			cout << "����������������1�ɹ�" << endl;
		}
		else
		{
			cout << "����������������1ʧ��" << endl;
		}
	}
	else
	{
		cout << "����������������1ʧ��" << endl;
	}
	board->resetBoard(data);

	//ͼһ����
	putChess(2, 2, BLACK);
	putChess(2, 3, BLACK);
	putChess(4, 4, BLACK);
	putChess(5, 3, BLACK);
	printChessBoard();
	system("pause");
	if (FORBIDDEN == test(3, 3, BLACK))
	{
		putChess(1, 1, WHITE);
		putChess(3, 3, BLACK);
		printChessBoard();
		system("pause");
		if (DEFAULT == test(3, 3, BLACK))
		{
			cout << "����������������2�ɹ�" << endl;
		}
		else
		{
			cout << "����������������2ʧ��" << endl;
		}
	}
	else
	{
		cout << "����������������2ʧ��" << endl;
	}
	board->resetBoard(data);

	//ͼ������
	putChess(2, 4, BLACK);
	putChess(2, 3, BLACK);
	putChess(4, 3, BLACK);
	putChess(5, 2, BLACK);
	printChessBoard();
	system("pause");
	if (FORBIDDEN == test(2, 5, BLACK))
	{
		putChess(2, 2, WHITE);
		putChess(2, 5, BLACK);
		printChessBoard();
		system("pause");
		if (DEFAULT == test(2, 5, BLACK))
		{
			cout << "����������������3�ɹ�" << endl;
		}
		else
		{
			cout << "����������������3ʧ��" << endl;
		}
	}
	else
	{
		cout << "����������������3ʧ��" << endl;
	}

	system("pause");
	board->resetBoard(data);

	//ͼ������
	putChess(1, 2, BLACK);
	putChess(4, 2, BLACK);
	putChess(4, 4, BLACK);
	putChess(4, 5, BLACK);
	putChess(6, 3, BLACK);
	putChess(7, 3, BLACK);
	printChessBoard();
	system("pause");
	if (FORBIDDEN == test(4, 3, BLACK))
	{
		putChess(4, 1, WHITE);
		putChess(4, 6, BLACK);
		printChessBoard();
		system("pause");
		if (DEFAULT == test(4, 3, BLACK))
		{
			cout << "�������Ľ���������2�ɹ�" << endl;
		}
		else
		{
			cout << "�������Ľ�������2ʧ��" << endl;
		}
	}
	else
	{
		cout << "�������Ľ�������2ʧ��" << endl;
	}

	system("pause");
	board->resetBoard(data);
	//ͼ������
	putChess(2, 3, BLACK);
	putChess(2, 5, BLACK);
	putChess(2, 7, BLACK);
	putChess(2, 9, BLACK);
	printChessBoard();
	system("pause");
	if (FORBIDDEN == test(2, 6, BLACK))
	{
		cout << "�������Ľ���������3�ɹ�" << endl;
	}
	else
	{
		cout << "�������Ľ�������3ʧ��" << endl;
	}
	
	system("pause");
	board->resetBoard(data);
}

int PlayGobang::test(int x, int y, int role)
{
	int state = DEFAULT;
	state = success(x, y);
	if (state == WIN)
	{
		return state;
	}
	if (role == BLACK)
	{
		state = forbidden(x, y);
	}
	return state;
}
