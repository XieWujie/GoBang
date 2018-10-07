#include "PlayGobang.h"

const int BLACK = 1;     //黑方
const int WHITE = 2;     //白方
const int DEFAULT = -1;  //默认标记
const int WIN = 0;      //赢的标记位
const int FORBIDDEN = 3; //禁手标记 
const int LR = 5;  //左右方向
const int TB = 6;   //上下方向
const int LB = 7;   //左下
const int RB = 8;   //右下
int forbiddenLineCount = 1;  //黑方一个方向上最多空一个格子的连续棋子数量
int emptyCount = 0;          //标记位
int direct[4] = { LR,LB,TB,RB};  //用于遍历方向
int nowRole = WHITE;            //当前角色
int lineCount = 1;             //连续棋子的数量
int edgeNotEmpty = 0;          //边缘与反方接触的棋子数量
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
			cout << "黑方输入行:" << endl;
			cin >> inputX;
			if (inputX < 0 || inputX>15)
			{
				if (inputX == -1)
				{
					return;
				}
				cout << "输入行不满足要求，重新输入" << endl;
				continue;
			}
			cout << "黑方输入列：" << endl;
			cin >> inputY;
			if (inputY < 0 || inputY>15)
			{
				if (inputY == -1)
				{
					return;
				}
				cout << "输入列不满足要求，重新输入" << endl;
				continue;
			}
		}
		else if (nowRole == WHITE)

		{
			cout << "白方输入行:" << endl;
			cin >> inputX;
			if (inputX < 0 || inputX>15)
			{
				if (inputX == -1)
				{
					return;
				}
				cout << "输入行不满足要求，重新输入" << endl;
				continue;
			}
			cout << "白方输入列：" << endl;
			cin >> inputY;

			if (inputY<0||inputY>15)
			{
				if (inputY == -1)
				{
					return;
				}
				cout << "输入列不满足要求，重新输入" << endl;
				continue;
			}
		}
		x = inputX - 1;
		y = inputY - 1;
		if (data->getPos(x, y) == BLACK || data->getPos(x, y) == WHITE)
		{
			cout << "此位置已被占用" << endl;
			continue;
		}
		 putChess(x, y, nowRole);
		 printChessBoard();
		 switch (test(x,y,nowRole))
		 {
		 case WIN: {
			 player->win(nowRole);
			 cout << "输入1继续,输入-1退出" << endl;
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
			 cout << "输入1继续,输入0返回上一棋子，输入-1退出" << endl;
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


//显示棋盘和棋子界面
void PlayGobang::printChessBoard(void)
{
	board->printBoard(data);
}


//计算经过我的某条线上与我颜色相同的连续棋子数目
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

//我下了x,y这个点后，是否已经胜利
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

//长连禁手
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



//四四禁手
bool PlayGobang::live4forbidden(int x, int y)
{
	int live4Count = 0;
	emptyCount = 0;
	forbiddenLineCount = 1;
	for (size_t i = 0; i < 4; i++)  //分方向遍历周围棋子
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

//三三禁手
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

//禁手
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

//将棋子放入棋盘指定位置，并做禁手等处理
int PlayGobang::putChess(int x, int y, int role)
{

	data->setPos(x, y, role);
	return DEFAULT;
}

bool PlayGobang::live4special(int x,int y)
{
	int live4Count = 0;
	int continueCount = -1;
	for (size_t i = 0; i < 4; i++)  //分方向遍历周围棋子
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

//自动化测试
void PlayGobang::autoTest(void)
{
	int state = DEFAULT;

	//图五用例
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
		cout << "测试四四禁手用例1成功" << endl;
	}
	else
	{
		cout << "测试四四禁手用例1失败" << endl;
	}
	}
	else
	{
		cout << "测试四四禁手用例1失败" << endl;
	}
	system("pause");
	board->resetBoard(data);

	//图六用例
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
		cout << "测试长连禁手成功" << endl;
		system("pause");
	}
	board->resetBoard(data);

	//图三用例
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
			cout << "测试三三禁手用例1成功" << endl;
		}
		else
		{
			cout << "测试三三禁手用例1失败" << endl;
		}
	}
	else
	{
		cout << "测试三三禁手用例1失败" << endl;
	}
	board->resetBoard(data);

	//图一用例
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
			cout << "测试三三禁手用例2成功" << endl;
		}
		else
		{
			cout << "测试三三禁手用例2失败" << endl;
		}
	}
	else
	{
		cout << "测试三三禁手用例2失败" << endl;
	}
	board->resetBoard(data);

	//图二用例
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
			cout << "测试三三禁手用例3成功" << endl;
		}
		else
		{
			cout << "测试三三禁手用例3失败" << endl;
		}
	}
	else
	{
		cout << "测试三三禁手用例3失败" << endl;
	}

	system("pause");
	board->resetBoard(data);

	//图四用例
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
			cout << "测试四四禁手手用例2成功" << endl;
		}
		else
		{
			cout << "测试四四禁手用例2失败" << endl;
		}
	}
	else
	{
		cout << "测试四四禁手用例2失败" << endl;
	}

	system("pause");
	board->resetBoard(data);
	//图四用例
	putChess(2, 3, BLACK);
	putChess(2, 5, BLACK);
	putChess(2, 7, BLACK);
	putChess(2, 9, BLACK);
	printChessBoard();
	system("pause");
	if (FORBIDDEN == test(2, 6, BLACK))
	{
		cout << "测试四四禁手手用例3成功" << endl;
	}
	else
	{
		cout << "测试四四禁手用例3失败" << endl;
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
