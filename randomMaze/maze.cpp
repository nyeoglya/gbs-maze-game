#include <stdio.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define _size	19 // 홀수 권장 

#define LEFT	1
#define RIGHT	2
#define TOP		3
#define BOTTOM	4
#define FILLED	5
#define EMPTY	6

typedef struct {
	int x, y;
} point;

int maze[_size][_size];
int mazeStack[(_size*_size)/4]; // 넉넉하게 잡음 
// 1: left, 2: right, 3: top, 4: bottom
// 5: 확정됨, 6: 미확정됨 

void printMaze() {
	int i, j;
	
	// 임시 출력 
	for (i=0;i<_size;i++) {
		for (j=0;j<_size;j++) {
			if (maze[j][i] == LEFT) printf("L");
			else if (maze[j][i] == RIGHT) printf("R");
			else if (maze[j][i] == TOP) printf("T");
			else if (maze[j][i] == BOTTOM) printf("B");
			else if (maze[j][i] == EMPTY) printf(" ");
			else if (maze[j][i] == FILLED) printf("F");
			
			// printf("%s", (maze[j][i]==5)?"∮":" ");
		}
		puts("");
	}
}

int main(void) {
	
	int i, j;
	int random;
	
	srand(time(NULL));
	
	for (i=0;i<_size;i++) {
		for (j=0;j<_size;j++) {
			if (i == 0 || i == _size-1 || j == 0 || j == _size-1) {
				maze[i][j] = 5;
			} else {
				maze[i][j] = 6;
			}
		}
	}
	
	i = 1; // 좌표의 x값 (오른쪽이 +)
	j = 1; // 좌표의 y값 (아래가 +)
	
	int count = 0;
	int stackCount = 0;
	int blockCount = 0;
	
	while (1) {
		// 만약 사면이 다 둘러싸였다면
		if (i+2 < _size) {
			if (maze[i+2][j] != EMPTY) {
				blockCount++;
			}
		} else {
			blockCount++;
		}
		
		if (j+2 < _size) {
			if (maze[i][j+2] != EMPTY) {
				blockCount++;
			}
		} else {
			blockCount++;
		}
		
		if (i-2 < 0) {
			if (maze[i-2][j] != EMPTY) {
				blockCount++;
			}
		} else {
			blockCount++;
		}
		
		if (j-2 < 0) {
			if (maze[i][j-2] != EMPTY) {
				blockCount++;
			}
		} else {
			blockCount++;
		}
		
		if (blockCount == 4) {
			maze[i][j] = FILLED;
			stackCount--;
			
			// printf("%d, %d\n", stackCount, mazeStack[stackCount]);
			
			switch(mazeStack[stackCount]) { // 미리 표시한 값을 이용한다. 
				case LEFT:
					i += 2;
					break;
				case RIGHT:
					i -= 2;
					break;
				case TOP:
					j += 2;
					break;
				case BOTTOM:
					j -= 2;
					break;
				default:
					break;
			}
		} else {
			blockCount = 0;
			stackCount++;
		}
		
		/*
		if (( || i+2 >= _size) // 어차피 값을 불러오기 때문에 의미가 없음 
			&& (maze[i-2][j] != EMPTY || i-2 < 0)
			&& (maze[i][j+2] != EMPTY || j+2 >= _size)
			&& (maze[i][j-2] != EMPTY || j-2 < 0)) {
			
			
			}
		}
		*/
		
		random = rand()%4 + 1; // 랜덤한 방향 설정
		
		switch(random) { // 방향에 대한 값 
			case LEFT: // 만약 방향이 왼쪽이라면 
				if (i-2 >= 0 && maze[i-2][j] == EMPTY) { // 가려는 곳이 비어있고 배열 오류가 나지 않는다면 
					maze[i][j] = LEFT; // 현재 위치에 방향을 기록하기 
					maze[i-1][j] = FILLED;
					i -= 2; // 다음 위치로 커서를 이동 
					mazeStack[stackCount] = LEFT; // 다음 위치에 이전의 값을 임시로 저장하기 
				} else {
					stackCount--;
				}
				break;
			case RIGHT: // 만약 방향이 오른쪽이라면
				if (i+2 < _size && maze[i+2][j] == EMPTY) {
					maze[i][j] = RIGHT;
					maze[i+1][j] = FILLED;
					i += 2;
					mazeStack[stackCount] = RIGHT;
				} else {
					stackCount--;
				}
				break;
			case TOP: // 방향 위쪽
				if (j-2 < _size && maze[i][j-2] == EMPTY) {
					maze[i][j] = TOP;
					maze[i][j-1] = FILLED;
					j -= 2;
					mazeStack[stackCount] = TOP;
				} else {
					stackCount--;
				}
				break;
			case BOTTOM: // 방향 아래쪽
				if (j+2 < _size && maze[i][j+2] == EMPTY) {
					maze[i][j] = BOTTOM;
					maze[i][j+1] = FILLED;
					j += 2; 
					mazeStack[stackCount] = BOTTOM;
				} else {
					stackCount--;
				}
				break;
		}
		
		if (stackCount < 0) break;
		
		// Sleep(100);
		
		count = 0;
		system("cls");
		printMaze();
		
		printf("%d: %d (%d, %d)\n", stackCount, mazeStack[stackCount], i, j);
		
		Sleep(25);
		
		while (1) {
			if (GetAsyncKeyState(VK_RETURN)) break;
		}
		
		count += 1;
		if (count == 100) {
			count = 0;
			system("cls");
			printMaze();
		}
	}
	
	printf("끝남");
	
	return 0;
}

/*
int main(void) {
	
	int i, j;
	int random;
	
	srand(time(NULL));
	
	// VERSION 1.0
	
	for (i=0;i<_size;i++) {
		for (j=0;j<_size;j++) {
			maze[i][j] = 1;
			
			if ((i+1)%2 == 0 && (j+1)%2 == 0) {
				maze[i][j] = 0;
			}
		}
	}
	
	for (i=1;i<_size;i+=2) {
		for (j=1;j<_size;j+=2) {
			if (rand()%2 == 1) {
				maze[i][j+1] = 0;
			} else {
				maze[i+1][j] = 0;
			}
		}
	}

	// VERSION 1.0
	
	for (i=0;i<_size;i++) {
		for (j=0;j<_size;j++) {
			printf("%s", (maze[i][j]==1)?"∮":" ");
		}
		puts("");
	}
	
	return 0;
}
*/
