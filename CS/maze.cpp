#include <stdio.h>

#include <time.h>
#include <string.h>

#include <windows.h>

// 양쪽 벽을 고려해서 홀수로 설정 
#define MAZE_WIDTH 111
#define MAZE_HEIGHT 29

// 값을 나열하기 위해 열거형 선언
enum { LEFT, TOP, RIGHT, BOTTOM }; // 방향
enum { WALL, EMPTY }; // 좌표의 상태 

// 단위 격자 
typedef struct {
	int X, Y;
} MAPUNIT;

// 전역변수 
const int Rdirect[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // 상, 하, 좌, 우를 의미하며 벽을 고려하여 2칸씩 이동한다. 
int _maze[MAZE_WIDTH][MAZE_HEIGHT]; // 이거 [MAZE_HEIGHT][MAZE_WIDTH] 로 수정하기 

// 함수 선언 
void mazeGen(int, int);
int checkRange(int, int);
void mixDirection(int*, int);

int main(void) {
	MAPUNIT startPoint = {1,1}; // 시작점 정의(임의로 변경 가능) 
	
	srand((unsigned int)time(NULL)); // 랜덤 함수 시드 초기화 
	// 원하는 값(M_WALL)로 map의 사이즈만큼 map에 메모리 할당, 즉 map을 전부 벽으로 설정 
	memset(_maze, WALL, sizeof(_maze)); // string.h
	// 미로 생성(재귀적) 
	mazeGen(startPoint.X, startPoint.Y);
	
	// 벽에 구멍 뚫기
	
	
	// 미로 출력 
	for (int i = 0;i < MAZE_HEIGHT;++i) {
		for (int j = 0;j < MAZE_WIDTH;++j) {
			if (_maze[j][i] == WALL) printf("W");
			else printf(" ");
		}
		puts(""); 
	}

	return 0;
}

// 진행하는 방향의 순서를 섞는 함수
void mixDirection(int* arr, int num) {
	// srand(time(NULL));
	
	int temp, i = 0;
	int randomNum; // 랜덤한 수를 담을 변수 정의 
	
	for (;i<(num-1);i++) {
		randomNum = rand()%(num-i)+i;
		temp = arr[i];
		arr[i] = arr[randomNum];
		arr[randomNum] = temp;
	}
}

// 범위의 내부에 있는지 체크하는 함수 
int checkRange(int x, int y) {
	/*
	1. y가 전체 높이보다 작고 0보다 크며 x가 전체 높이보다 작고 0보다 클 때, 1을 반환
	2. 그 외에는 0을 반환
	*/
	return (x < MAZE_WIDTH - 1 && x > 0) && (y < MAZE_HEIGHT - 1 && y > 0);
}

// 미로 생성 알고리즘(Recursive Backtracking 방법)
void mazeGen(int x, int y) {
	int i = 0;
	int nx, ny;
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM };
	
	_maze[x][y] = EMPTY; // 현재 있는 위치를 빈칸으로 만든다. 
	
	// 가려는 방향을 섞는다. 
	mixDirection(direction, 4);
	
	// 총 4 방향에 대해 재귀적으로 실행한다. 
	for (;i<4;i++) {
		// 다음 갈 방향을 구한다. 
		nx = x+Rdirect[direction[i]][0];
		ny = y+Rdirect[direction[i]][1];

		// 범위를 벗어나지 않으며 가려는 방향이 아직 벽(비어있음)일 때 
		if (checkRange(nx, ny) && _maze[nx][ny] == WALL) {
			mazeGen(nx, ny);
			
			// 가려는 위치 사이에 구멍을 뚫는다. 
			if (nx!=x) _maze[(nx+x)/2][y] = EMPTY;// 세로 축 이동인 경우
			else _maze[x][(y+ny)/2] = EMPTY; // 가로 축 이동인 경우
			
			// 가려는 위치에 구멍을 뚫는다. 
			_maze[nx][ny] = EMPTY;
		}
	}
}

void nextRoad() {
	int i, random = 0;
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM };
	
	for (i=0;i<4;i++) {
		
	}
}
