#pragma once
#include"Player.h"
class charPlayer:public Player
{
public:
	charPlayer(int therole) { x = 0; y = 0; role = therole; }
	virtual int getPos(int& x, int&y);
	virtual bool lost(void);
private:
	int x;
	int y;
};

