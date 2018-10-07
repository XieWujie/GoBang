#pragma once

#include"Data.h"
#include<iostream>
using namespace std;
class Data
{
public:
	Data();
	~Data();
	int board[15][15];
	void setPos(int x, int y,int role);
	int getPos(int x, int y);
	void setPosWithoutInput(int x, int y, int role);

private:
	int x;
	int y;
	
};

