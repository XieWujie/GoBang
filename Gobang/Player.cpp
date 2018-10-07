#include "Player.h"


Player::Player()
{

}


Player::~Player()
{
}

int Player::getPos(int& x, int& y)
{
	return -1;
}

void Player::win(int role)
{
	if (role == 1)
	{
		cout << "黑方胜利" << endl;
	}
	else if (role == 2)
	{
		cout << "白方胜利" << endl;
	}
}

bool Player::lost(int role)
{
	if (role == 1)
	{
		cout << "黑方禁手" << endl;
	}
	return true;;
}
