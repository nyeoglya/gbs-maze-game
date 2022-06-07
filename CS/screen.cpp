// https://codevang.tistory.com/39

#include <stdio.h>

#include <windows.h>
#include <conio.h>

#include <tchar.h>

#define MAZE_WIDTH		50
#define MAZE_HEIGHT		30

// 구조체(예제) 선언
typedef struct {
	int x, y;
} POINTING; 

// 전역 변수 선언
int maze[MAZE_WIDTH][MAZE_HEIGHT] = {{1,0,0,1,1}, {1,0,0,1}, {1,0}};
// int backScreenBuffer[MAZE_WIDTH][MAZE_HEIGHT];

POINTING playerTest = {1, 1};

// 함수 선언
void printBuffer();
void printUNIT(int, int, int);
void gotoxy(short, short);

int main(void) {
	
	int i, j;
	
	int playerFormal = 0;
		
	/*
	for (;i<MAZE_WIDTH;i++) {
		for (;j<MAZE_HEIGHT;j++) {
			maze[i][j] = 1;
			printf("B");
		}
		
		puts("");
	}
	*/
	
	for (i=0;i<MAZE_HEIGHT;i++) {
		for (j=0;j<MAZE_WIDTH;j++) {
			printf("%d", maze[j][i]);
			/*
			if (maze[i][j] == 0) printf(" "); 
			else if (maze[i][j] == 1) printf("▒"); 
			else if (maze[i][j] == 2) printf("A"); 
			*/
		}
		puts("");
	}
	
	gotoxy(1, 1);
	
	// Sleep(6000);
	
	while(1) {
		Sleep(50);
		
		gotoxy(playerTest.x, playerTest.y);
		printf(" ");
		
		if (GetAsyncKeyState(VK_RIGHT)) {
			playerFormal = playerTest.x;
			
			if (maze[playerTest.x+1][playerTest.y] != 1) {		
				playerTest.x += 1;
				
				if (playerTest.x >= MAZE_WIDTH) {
					playerTest.x -= MAZE_WIDTH;
				}
				
				maze[playerTest.x][playerTest.y] = 2;
				maze[playerFormal][playerTest.y] = 0;
				
				gotoxy(playerFormal, playerTest.y);
				printf("0");
			}
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			playerFormal = playerTest.x;
			
			if (maze[playerTest.x-1][playerTest.y] != 1) {		
				playerTest.x -= 1;
				
				if (playerTest.x < 0) {
					playerTest.x += MAZE_WIDTH;
				}
				
				maze[playerTest.x][playerTest.y] = 2;
				maze[playerFormal][playerTest.y] = 0;
				
				
				gotoxy(playerFormal, playerTest.y);
				printf("0");
			}
		}
		if (GetAsyncKeyState(VK_UP)) {
			playerFormal = playerTest.y;
			
			if (maze[playerTest.x][playerTest.y-1] != 1) {		
				playerTest.y -= 1;
				
				if (playerTest.y < 0) {
					playerTest.y += MAZE_HEIGHT;
				}
				
				maze[playerTest.x][playerTest.y] = 2;
				maze[playerTest.x][playerFormal] = 0;
				
				gotoxy(playerTest.x, playerFormal);
				printf("0");
			}
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			playerFormal = playerTest.y;
			
			if (maze[playerTest.x][playerTest.y+1] != 1) {		
				playerTest.y += 1;
				
				if (playerTest.y >= MAZE_HEIGHT) {
					playerTest.y -= MAZE_HEIGHT;
				}
				
				maze[playerTest.x][playerTest.y] = 2;
				maze[playerTest.x][playerFormal] = 0;
				
				gotoxy(playerTest.x, playerFormal);
				printf("0");
			}
		}
		
		gotoxy(playerTest.x, playerTest.y);
		printf("A");
		
		// printBuffer();
		
	}
	
	return 0;
}

/*
void printBuffer() {
	int i, j;
	
	for (i=0;i<MAZE_HEIGHT;i++) {
		for (j=0;j<MAZE_WIDTH;j++) {
			// printf("(%d %d)", maze[i][j], backScreenBuffer[i][j]);
			// printf("%s", maze[i][j]==2?"A":"▒");
			
			if (maze[i][j] != backScreenBuffer[i][j]) {
				gotoxy(playerTest.x, playerTest.y);
				printf("%s", maze[i][j]==2?"A":"▒");
				
				backScreenBuffer[i][j] = maze[i][j];
			}
		}
		puts("");
	}
}

void printUNIT(int x, int y, int chara) {
	gotoxy(x, y);
	
	switch(chara) {
		case 2:
			printf("A");
		default:
			printf("D");
	}
}
*/

void gotoxy(short x, short y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


