#include <stdio.h>

#include <windows.h>

void gotoxy(short x, short y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main(void) {
	gotoxy(4, 5);
	
	printf("asdf");
	
	return 0;
}

