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
		cout << "�ڷ�ʤ��" << endl;
	}
	else if (role == 2)
	{
		cout << "�׷�ʤ��" << endl;
	}
}

bool Player::lost(int role)
{
	if (role == 1)
	{
		cout << "�ڷ�����" << endl;
	}
	return true;;
}
