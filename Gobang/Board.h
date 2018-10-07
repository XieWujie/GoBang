#pragma once
#include"Data.h"
#include<iostream>
#include<iomanip>
using namespace std;
class Board
{
public:
	Board();
	~Board();
	void printBoard(Data * data);
	void resetBoard(Data * data);
	void remove(int x, int y,Data * data);
};

