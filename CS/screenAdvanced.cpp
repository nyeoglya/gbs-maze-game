#include <stdio.h>

#include <windows.h>
#include <conio.h>

#include <tchar.h>

#define MAZE_WIDTH		50
#define MAZE_HEIGHT		30

// 열거형 선언 
enum { EMPTY, WALL, PLAYER };

// 구조체(예제) 선언
typedef struct {
	int x, y;
} POINTING;

// 전역 변수 선언
int maze[MAZE_HEIGHT][MAZE_WIDTH] = {
{WALL, WALL, WALL, WALL, WALL},
{WALL, EMPTY, EMPTY, WALL, WALL},
{WALL, EMPTY, EMPTY, WALL, WALL},
{WALL, WALL, EMPTY, EMPTY, WALL},
{WALL, WALL, WALL, EMPTY, WALL}
};

int backScreenBuffer[MAZE_HEIGHT][MAZE_WIDTH];

POINTING playerTest = {1, 1};

// 함수 선언
void doubleBuffering();
void printUnit(int, int, int);
void gotoxy(short, short);
int checkRange(int, int);
void fullScreen();

void textColorW();
void textColorY();

int main(void) {
	
	fullScreen();
	
	int i, j;
	
	for(i=0;i<MAZE_HEIGHT;i++) {
		for(j=0;j<MAZE_WIDTH;j++) {
			backScreenBuffer[i][j] = EMPTY;
		}
	}
	
	doubleBuffering();
	
	Sleep(1500);
	
	while(1) {
		Sleep(50);
		
		if (GetAsyncKeyState(VK_RIGHT) && checkRange(playerTest.x+1, playerTest.y)) {
			if (maze[playerTest.y][playerTest.x+1] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.x += 1;
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if (GetAsyncKeyState(VK_LEFT) && checkRange(playerTest.x-1, playerTest.y)) {
			if (maze[playerTest.y][playerTest.x-1] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.x -= 1;
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if (GetAsyncKeyState(VK_UP) && checkRange(playerTest.x, playerTest.y-1)) {
			if (maze[playerTest.y-1][playerTest.x] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.y -= 1;
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if (GetAsyncKeyState(VK_DOWN) && checkRange(playerTest.x, playerTest.y+1)) {
			if (maze[playerTest.y+1][playerTest.x] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.y += 1;
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		doubleBuffering();
		
		textColorW();
		gotoxy(0, 40);
		printf("(%02d, %02d)", playerTest.x, playerTest.y);
	}
	
	return 0;
}
 
int checkRange(int x, int y) {
	return (x < MAZE_WIDTH - 1 && x > 0) && (y < MAZE_HEIGHT - 1 && y > 0);
}

void doubleBuffering() {
	int i, j;
	
	for (i=0;i<MAZE_HEIGHT;i++) {
		for (j=0;j<MAZE_WIDTH;j++) {
			if (maze[i][j] != backScreenBuffer[i][j]) {
				printUnit(j, i, maze[i][j]);
				backScreenBuffer[i][j] = maze[i][j];
			}
		}
	}
}

void printUnit(int x, int y, int chara) {
	gotoxy(x+1, y+1); // 화면 출력을 살짝 이동함 
	
	switch(chara) {
		case PLAYER:
			textColorY();
			printf("P");
			break;
		case WALL:
			printf("W");
			break;
		case EMPTY:
			printf(" ");
			break;
		default:
			exit(-1);
			break;
	}
}

void gotoxy(short x, short y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void fullScreen() {
    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
}

void textColorY() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
}

void textColorW() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

