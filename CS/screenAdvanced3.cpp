#include <stdio.h>

#include <windows.h>
#include <conio.h>

#include <time.h>
#include <string.h>

#include <tchar.h>

// 양쪽 벽을 고려해서 홀수로 설정 
#define MAZE_WIDTH 111
#define MAZE_HEIGHT 41

#define ENTITY_COUNT 1

// 빨간 안경 
// 치킨(상세 설명: 맛있다)  
// 몬스터

// 열거형 선언 
enum { LEFT, TOP, RIGHT, BOTTOM };
enum { MWALL, MEMPTY }; // 좌표의 상태
enum unit { EMPTY, WALL, PLAYER, ENTITY, ITEMHEAL };
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
		x = 50;
		y = 30;
		health = 10;
		type = NORMAL;
	}
} Entity;

// 단위 격자 
typedef struct {
	int X, Y;
} MAPUNIT;

// 전역 변수 선언
int maze[MAZE_HEIGHT][MAZE_WIDTH] = {/*
{WALL, WALL, WALL, WALL, WALL},
{WALL, EMPTY, EMPTY, WALL, WALL},
{WALL, EMPTY, EMPTY, WALL, WALL, ITEMHEAL},
{WALL, WALL, EMPTY, EMPTY, WALL},
{WALL, WALL, WALL, EMPTY, WALL}*/
};
/*
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
*/

Entity entity[ENTITY_COUNT];

const int direction[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};

int backScreenBuffer[MAZE_HEIGHT][MAZE_WIDTH];
int tempBuffer[MAZE_HEIGHT][MAZE_WIDTH];

Player playerTest;

int entityUpdateTime;

int previousUnit;
_boolean isOpenInventory = False;

const int Rdirect[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // 상, 하, 좌, 우를 의미하며 벽을 고려하여 2칸씩 이동한다. 
int _maze[MAZE_HEIGHT][MAZE_WIDTH]; 

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
// void pageScreen(int);

void mazeGen();
void mazeGenA(int, int);
int checkRange(int, int);
void mixDirection(int*, int);

void textColorW();
void textColorR();
void textColorY();
void textColorB();
void textColorG();
void backgroundColorW();
void backgroundColorY();
void backgroundColorG();
void backgroundColorR();
void backgroundColorBl();

int main(void) {
	
	backgroundColorW();
	
	fullScreen();
	mazeGen();
	
	maze[2][2] = ITEMHEAL;
	
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
		
		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) && checkRange(playerTest.y, playerTest.x+1)) {
			if (maze[playerTest.y][playerTest.x+1] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.x += 1;
				previousUnit = maze[playerTest.y][playerTest.x];
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) && checkRange(playerTest.y, playerTest.x-1)) {
			if (maze[playerTest.y][playerTest.x-1] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.x -= 1;
				previousUnit = maze[playerTest.y][playerTest.x];
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) && checkRange(playerTest.y-1, playerTest.x)) {
			if (maze[playerTest.y-1][playerTest.x] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.y -= 1;
				previousUnit = maze[playerTest.y][playerTest.x];
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		if ((GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) && checkRange(playerTest.y+1, playerTest.x)) {
			if (maze[playerTest.y+1][playerTest.x] != WALL) {
				maze[playerTest.y][playerTest.x] = EMPTY;
				playerTest.y += 1;
				previousUnit = maze[playerTest.y][playerTest.x];
				maze[playerTest.y][playerTest.x] = PLAYER;
			}
		}
		
		/*
		if (GetAsyncKeyState(0x45)) { // e
			isOpenInventory = True;
			backupScreen();
			
			// pageScreen(0);
			
			gotoxy(((MAZE_WIDTH - 30) / 2) + 5, ((MAZE_HEIGHT - 6) / 2) + 2);
			printf("ITEM1: %02d ITEM2: %02d", playerTest.inventory[0], playerTest.inventory[1]);
			
			gotoxy(((MAZE_WIDTH - 30) / 2) + 5, ((MAZE_HEIGHT - 6) / 2) + 3);
			printf("ITEM3: %02d", playerTest.inventory[2]);
			
		} else if (GetAsyncKeyState(VK_ESCAPE)) { // e와 esc가 동시에 눌리면 안됨 
			exit(-1);
		}
		*/
		
		if (GetAsyncKeyState(0x52)) {
			mazeGen();
			
			playerTest.x = 1;
			playerTest.y = 1;
		}
		
		if (GetAsyncKeyState(VK_ESCAPE)) {
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
			case ITEMHEAL:
				// playerTest.inventory[0] += 1;
				playerTest.health += 5;
				break;
			default:
				break;
		}
		
		previousUnit = -1;
		
		doubleBuffering();
		
		textColorW();
		gotoxy(0, MAZE_HEIGHT+1);
		printf("\n(%02d, %02d)\n\n", playerTest.x, playerTest.y);
		printf("Health: %03d\n", playerTest.health);
	}

gameEND:
	printf("사망\n 사유: 와샌즈");
	
	return 0;
}

int entityUpdate() {
	int i, j;
	int random = 0;
	
	for (i=0;i<ENTITY_COUNT;i++) {
		random = rand()%4;
		
		if (!checkRange(entity[i].y+direction[random][1], entity[i].x+direction[random][0])) {
			continue;
		}
		
		if (maze[entity[i].x+direction[random][0]][entity[i].y+direction[random][1]] == WALL) {
			continue;
		}
		
		maze[entity[i].y][entity[i].x] = EMPTY;
		
		entity[i].x += direction[random][0];
		entity[i].y += direction[random][1];
		
		maze[entity[i].y][entity[i].x] = ENTITY;
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

/*
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
*/

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
	
	backgroundColorW();
	
	switch(chara) {
		case PLAYER:
			// textColorY();
			backgroundColorY();
			printf(" ");
			textColorW();
			break;
		case WALL:
			backgroundColorBl();
			printf(" ");
			break;
		case EMPTY:
			printf(" ");
			break;
		case ENTITY:
			backgroundColorR();
			// textColorR();
			printf(" ");
			textColorW();
			break;
		case ITEMHEAL:
			// textColorG();
			backgroundColorG();
			printf(" ");
			textColorW();
			break;
		default:
			exit(-1);
			break;
	}
}

void mazeGen() {
	int i, j;

	MAPUNIT startPoint = {1,1}; // 시작점 정의(임의로 변경 가능) 
	
	srand((unsigned int)time(NULL)); // 랜덤 함수 시드 초기화 
	// 원하는 값(M_WALL)로 map의 사이즈만큼 map에 메모리 할당, 즉 map을 전부 벽으로 설정 
	memset(_maze, MWALL, sizeof(_maze)); // string.h

	// 미로 생성(재귀적) 
	mazeGenA(startPoint.Y, startPoint.X);
	
	// TODO: 벽에 구멍 뚫기
	
	// 미로 복사하기 
	for (i=0;i<MAZE_HEIGHT;i++) {
		for (j=0;j<MAZE_WIDTH;j++) {
			switch (_maze[i][j]) {
				case MWALL:
					maze[i][j] = WALL;
					break;
				case MEMPTY:
					maze[i][j] = EMPTY;
					break;
				default:
					break;
			}
		}
	}
}

// 진행하는 방향의 순서를 섞는 함수
void mixDirection(int* arr, int num) {
	// srand(time(NULL));
	
	int temp, i;
	int randomNum; // 랜덤한 수를 담을 변수 정의 
	
	for (i=0;i<(num-1);i++) {
		randomNum = rand()%(num-i)+i;
		temp = arr[i];
		arr[i] = arr[randomNum];
		arr[randomNum] = temp;
	}
}

// 범위의 내부에 있는지 체크하는 함수 
int checkRange(int y, int x) {
	/*
	1. y가 전체 높이보다 작고 0보다 크며 x가 전체 높이보다 작고 0보다 클 때, 1을 반환
	2. 그 외에는 0을 반환
	*/
	return (x < MAZE_WIDTH - 1 && x > 0) && (y < MAZE_HEIGHT - 1 && y > 0);
}

// 미로 생성 알고리즘(Recursive Backtracking 방법)
void mazeGenA(int y, int x) {
	int i = 0;
	int nx, ny;
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM };
	
	_maze[y][x] = MEMPTY; // 현재 있는 위치를 빈칸으로 만든다. 
	
	// 가려는 방향을 섞는다. 
	mixDirection(direction, 4);
	
	// 총 4 방향에 대해 재귀적으로 실행한다. 
	for (;i<4;i++) {
		// 다음 갈 방향을 구한다. 
		nx = x+Rdirect[direction[i]][0];
		ny = y+Rdirect[direction[i]][1];

		// 범위를 벗어나지 않으며 가려는 방향이 아직 벽(비어있음)일 때 
		if (checkRange(ny, nx) && _maze[ny][nx] == MWALL) {
			mazeGenA(ny, nx);
			
			// 가려는 위치 사이에 구멍을 뚫는다. 
			if (ny!=y) _maze[(ny+y)/2][x] = MEMPTY;// 세로 축 이동인 경우
			else _maze[y][(x+nx)/2] = MEMPTY; // 가로 축 이동인 경우
			
			// 가려는 위치에 구멍을 뚫는다. 
			_maze[ny][nx] = MEMPTY;
		}
	}
}

void nextRoad() {
	int i, random = 0;
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM };
	
	for (i=0;i<4;i++) {
		
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

void backgroundColorW() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
}
void backgroundColorBl() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NULL);
}
void backgroundColorR() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
}
void backgroundColorG() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
}
void backgroundColorY() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN);
}
