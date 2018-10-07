#include <iostream>
#include "charPlayer.h"


using namespace std;

/* 此围棋选手使用字符界面输入坐标
 * 输入合法坐标返回0，非法坐标返回-1
 */
int charPlayer::getPos(int& x, int& y)
{
	//请补充代码	
	return 0;
}

//选手行号输入0，表示主动认输
bool charPlayer::lost(void)
{
	return x == 0;
}
