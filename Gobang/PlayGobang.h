#pragma once
#include"Board.h"
#include"Data.h"
#include"Player.h"
class PlayGobang
{
public:

	PlayGobang(Board * board,Data * data,Player * player);
	~PlayGobang();
	void start();
	void resetBoard();
	void autoTest(void);
private:
	Board * board;
	Data * data;
	Player * player;
  

	//显示棋盘和棋子界面
    void printChessBoard(void);

	int test(int x, int y, int role);
	//计算经过我的某条线上与我颜色相同的连续棋子数目
    void continousChessCount(int x, int y, int direct,int convert,int role,int limitCount);


	//我下了x,y这个点后，是否已经胜利
    int success(int x, int y);

	//长连禁手
	bool longForbidden(int x, int y);

	//四四禁手
    bool live4forbidden(int x, int y);

	//三三禁手
    bool live3forbidden(int x, int y);

	//禁手
    int forbidden(int x, int y);

	// 特殊的四四禁手
	bool live4special(int x, int y);

	//将棋子放入棋盘指定位置，并做禁手等处理
	int putChess(int x, int y, int role);

	void oneEmptyCount(int x, int y, int direct, int one);

	//自动化测试

};

