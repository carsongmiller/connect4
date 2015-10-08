#include <iostream>
#include <windows.h>

using namespace std;

const int w_ = 7;
const int h_ = 6;

int main()
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);

	int BLACK = 0;
	int BLUE = 1;
	int GREEN = 2;
	int CYAN = 3;
	int RED = 4;
	int MAGENTA = 5;
	int BROWN = 6;
	int LIGHTGRAY = 7;
	int DARKGRAY = 8;
	int LIGHTBLUE = 9;
	int LIGHTGREEN = 10;
	int LIGHTCYAN = 11;
	int LIGHTRED = 12;
	int LIGHTMAGENTA = 13;
	int YELLOW = 14;
	int WHITE = 15;


	for(int i = 0; i < 16; i++)
	{
		SetConsoleTextAttribute(H, i);
		cout << "TEXT\n";
	}
	return 0;
}

