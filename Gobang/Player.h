#pragma once

#include<iostream>
using namespace std;
class Player
{
	public:

		Player();
		~Player();

		int getPos(int& x, int& y);
		 bool lost(int role);
		 void win(int role);

	protected:
		int role;
	};

