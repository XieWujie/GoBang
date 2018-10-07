#include "Data.h"



Data::Data()
{
	
}


Data::~Data()
{
}

void Data::setPos(int x, int y, int role) {
	board[x][y] = role;
}
int Data::getPos(int x, int y) {
	return board[x][y];
}


void Data::setPosWithoutInput(int x, int y, int role)
{
	board[x][y] = role;
}
