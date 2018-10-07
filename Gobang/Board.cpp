#include "Board.h"



Board::Board()
{
}


Board::~Board()
{
}
void Board::printBoard(Data * data) 
{
	
	for (int i = -1; i < 15; i++)
	{
		for (int j = -1; j < 15; j++)
		{

			int setW = 5;
			cout.setf(ios::left);
		
			if (i == -1)
			{
				if (j > 8)
				{
					cout << (j + 1) << " ";
				}
				else {
					cout << (j + 1) << "  ";
				}
				
			}

			if (i >= 0)
			{
				if (j == -1)
				{
					if (i > 8)
					{
						cout << (i + 1) <<" ";
					}
					else {
						cout << (i + 1) << "  ";
					}
				}
				else
				{
					int role = data->getPos(i, j);
					if (role == 1)
					{
						cout << "#  ";
					}
					else if (role == 2)
					{
						cout << "@  ";
					}
					else
					{
						cout << "+  ";
					}
				}
			}
		}
		cout << endl;
	}
}

void Board::resetBoard(Data * data)
{
	for (size_t i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			data->setPosWithoutInput(i, j, 0);
		}
	}
}

void Board::remove(int x, int y,Data * data)
{
	data->setPosWithoutInput(x, y, 0);
}