#include"PlayGobang.h"
#include <string.h>

int main(int argc, char** argv)
{
	Data data;
	Board board;
	Player player;
	PlayGobang gobang(&board,&data,&player);
	if (argc == 2 && strcmp(argv[1], "-test") == 0) {
		gobang.autoTest();
		return 0;
	}
	gobang.start();
	return 1;
 }
