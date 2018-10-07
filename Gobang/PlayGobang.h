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
  

	//��ʾ���̺����ӽ���
    void printChessBoard(void);

	int test(int x, int y, int role);
	//���㾭���ҵ�ĳ������������ɫ��ͬ������������Ŀ
    void continousChessCount(int x, int y, int direct,int convert,int role,int limitCount);


	//������x,y�������Ƿ��Ѿ�ʤ��
    int success(int x, int y);

	//��������
	bool longForbidden(int x, int y);

	//���Ľ���
    bool live4forbidden(int x, int y);

	//��������
    bool live3forbidden(int x, int y);

	//����
    int forbidden(int x, int y);

	// ��������Ľ���
	bool live4special(int x, int y);

	//�����ӷ�������ָ��λ�ã��������ֵȴ���
	int putChess(int x, int y, int role);

	void oneEmptyCount(int x, int y, int direct, int one);

	//�Զ�������

};

