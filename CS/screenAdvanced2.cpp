#include <stdio.h>

#include <windows.h>
#include <conio.h>

#include <tchar.h>

#define MAZE_WIDTH		50
#define MAZE_HEIGHT		30

#define ENTITY_COUNT	1

// 빨간 안경 
// 치킨(상세 설명: 맛있다)  
// 몬스터

// 열거형 선언 
enum unit { EMPTY, WALL, PLAYER, ENTITY, ITEMTEST1 };
enum entityType { NORMAL, MORERANGE, MOREHEALTH, MOREDAMAGE };
typedef enum { False, True } _boolean;

// 구조체(예제) 선언
typedef struct _player {
	int x, y;
	int health;
	int attackDamage;
	int inventory[3];
	
	_player() {
		x = 1;
		y = 1;
		health = 10;
		inventory[0] = 0;
		inventory[1] = 0;
		inventory[2] = 0;
	}
} Player;

typedef struct _entity {
	int x, y;
	int health;
	int attackDamage;
	enum entityType type;
	
	_entity() {
		x = 10;
		y = 10;
		health = 10;
		type = NORMAL;
	}
} Entity;

// 전역 변수 선언
int maze[MAZE_HEIGHT][MAZE_WIDTH] = {
{WALL, WALL, WALL, WALL, WALL},
{WALL, EMPTY, EMPTY, WALL, WALL},
{WALL, EMPTY, EMPTY, WALL, WALL, ITEMTEST1},
{WALL, WALL, EMPTY, EMPTY, WALL},
{WALL, WALL, WALL, EMPTY, WALL}
};

int page[3][6][30] = {
{
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
},
{
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
},
{
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}
};

Entity entity[ENTITY_COUNT];

int direction[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};

int backScreenBuffer[MAZE_HEIGHT][MAZE_WIDTH];
int tempBuffer[MAZE_HEIGHT][MAZE_WIDTH];

Player playerTest;

int entityUpdateTime;

int previousUnit;
_boolean isOpenInventory = False;

// 함수 선언
int entityUpdate();

void doubleBuffering();
void forceBuffering(int, int, int, int);
void printUnit(int, int, int);
void gotoxy(short, short);
int checkRange(int, int);

void fullScreen();
void backupScreen();
void restoreScreen();
void pageScreen(int);

void textColorW();
void textColorR();
void textColorY();
void textColorB();
void textColorG();

int main(void) {
	
	fullScreen();
	
	int i, j;
	
	for(i=0;i<MAZE_HEIGHT;i++) {
		for(j=0;j<MAZE_WIDTH;j++) {
			backScreenBuffer[i][j] = EMPTY;
		}
	}
	
	doubleBuffering();
	
	Sleep(500);
	
	while(1) {
		Sleep(50);
		
		if (isOpenInventory == True) {
			if (GetAsyncKeyState(VK_ESCAPE)) {
				isOpenInventory = False;
				restoreScreen();
				forceBuffering(((MAZE_WIDTH - 30) / 2)+2, ((MAZE_HEIGHT - 6) / 2)-2, 30, 6);
				Sleep(1000); // 너무 빨리 esc가 눌려서 종료되는 것을 막음 
			}
			
			continue;
		}
		
		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) && checkRange(playerTest.x+1, playerTest.y)) {
			if (maze[playerTest.y][playerTest.x+1] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.x += 1;
				previousUnit = maze[playerTest.y][playerTest.x];
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) && checkRange(playerTest.x-1, playerTest.y)) {
			if (maze[playerTest.y][playerTest.x-1] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.x -= 1;
				previousUnit = maze[playerTest.y][playerTest.x];
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) && checkRange(playerTest.x, playerTest.y-1)) {
			if (maze[playerTest.y-1][playerTest.x] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.y -= 1;
				previousUnit = maze[playerTest.y][playerTest.x];
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if ((GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) && checkRange(playerTest.x, playerTest.y+1)) {
			if (maze[playerTest.y+1][playerTest.x] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.y += 1;
				previousUnit = maze[playerTest.y][playerTest.x];
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if (GetAsyncKeyState(0x45)) { // e
			isOpenInventory = True;
			backupScreen();
			
			pageScreen(0);
			
			gotoxy(((MAZE_WIDTH - 30) / 2) + 5, ((MAZE_HEIGHT - 6) / 2) + 2);
			printf("ITEM1: %02d ITEM2: %02d", playerTest.inventory[0], playerTest.inventory[1]);
			
			gotoxy(((MAZE_WIDTH - 30) / 2) + 5, ((MAZE_HEIGHT - 6) / 2) + 3);
			printf("ITEM3: %02d", playerTest.inventory[2]);
			
		} else if (GetAsyncKeyState(VK_ESCAPE)) { // e와 esc가 동시에 눌리면 안됨 
			exit(-1);
		}
		
		entityUpdateTime += 1;
		
		if (entityUpdateTime%3 == 0) {
			entityUpdate();
			entityUpdateTime = 0;
		}
		
		for (i=0;i<ENTITY_COUNT;i++) {
			if (((playerTest.x-entity[i].x)*(playerTest.x-entity[i].x)
			+ (playerTest.y-entity[i].y) * (playerTest.y-entity[i].y)) <= 1.1) {
				playerTest.health -= 1;
			}
		}
		
		if (playerTest.health <= 0) {
			goto gameEND;
		}
		
		switch (previousUnit) {
			case ITEMTEST1:
				playerTest.inventory[0] += 1;
				break;
			default:
				break;
		}
		
		previousUnit = -1;
		
		doubleBuffering();
		
		textColorW();
		gotoxy(0, 40);
		printf("(%02d, %02d)\n\n", playerTest.x, playerTest.y);
		printf("Health: %03d\n", playerTest.health);
		printf("\n\n%d", maze[playerTest.y][playerTest.x]);
	}

gameEND:
	printf("사망\n 사유: 처맞음");
	
	return 0;
}

int entityUpdate() {
	int i, j;
	int random = 0;
	
	for (i=0;i<ENTITY_COUNT;i++) {
		random = rand()%4;
		
		if (!checkRange(entity[i].x+direction[random][0], entity[i].y+direction[random][1])) {
			continue;
		}
		
		if (maze[entity[i].x+direction[random][0]][entity[i].y+direction[random][1]] != WALL ||
		maze[entity[i].x+direction[random][0]][entity[i].y+direction[random][1]] != ENTITY) {
			maze[entity[i].y][entity[i].x] = EMPTY;
			
			entity[i].x += direction[random][0];
			entity[i].y += direction[random][1];
			
			maze[entity[i].y][entity[i].x] = ENTITY;
		}
	}
}

void backupScreen() {
	int i, j;
	for(i=0;i<MAZE_HEIGHT;i++) {
		for(j=0;j<MAZE_WIDTH;j++) {
			tempBuffer[i][j] = maze[i][j];
		}
	}
}

void restoreScreen() {
	int i, j;
	for(i=0;i<MAZE_HEIGHT;i++) {
		for(j=0;j<MAZE_WIDTH;j++) {
			maze[i][j] = tempBuffer[i][j];
		}
	}
}

void pageScreen(int num) {
	int deltaX = (MAZE_WIDTH - 30) / 2;
	int deltaY = (MAZE_HEIGHT - 6) / 2;
	
	int i, j;
	for(i=0;i<6;i++) {
		for(j=0;j<30;j++) {
			maze[i+deltaX][j+deltaY] = page[num][i][j];
		}
	}
	
	switch(num) {
		case 0:
			gotoxy(deltaX+13, deltaY);
			printf("inventory");
			break;
		default:
			break;
	}
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

void forceBuffering(int x, int y, int dx, int dy) {
	int i, j;
	
	for (i=y;i<y+dy;i++) {
		for (j=x;j<x+dx;j++) {
			printUnit(j, i, maze[i][j]);
			backScreenBuffer[i][j] = maze[i][j];
		}
	}
}

void printUnit(int x, int y, int chara) {
	gotoxy(x+1, y+1); // 화면 출력을 살짝 이동함 
	
	switch(chara) {
		case PLAYER:
			textColorY();
			printf("P");
			textColorW();
			break;
		case WALL:
			printf("W");
			break;
		case EMPTY:
			printf(" ");
			break;
		case ENTITY:
			textColorR();
			printf("E");
			textColorW();
			break;
		case ITEMTEST1:
			textColorG();
			printf("I");
			textColorW();
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

void textColorR() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
}

void textColorB() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
}

void textColorG() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
}

void textColorW() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

