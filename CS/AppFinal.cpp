// 필요한 헤더 파일들을 불러온다 
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <mmsystem.h>
#include <conio.h>

#include <time.h>
#include <string.h>

#include <tchar.h>

// 양쪽 벽을 고려해서 홀수로 설정 
#define MAZE_WIDTH 131 // 미로의 가능한 최대 가로 길이 
#define MAZE_HEIGHT 41 // 미로의 가능한 최대 세로 길이 

#define ENTITY_COUNT 2 // 적대적인 객체의 수 

#define SUBMIT		101

// 열거형 선언 
enum { LEFT, TOP, RIGHT, BOTTOM }; // 방향에 대한 열거형 
enum { MWALL, MEMPTY, MEXIT }; // 좌표의 상태
enum unit { EMPTY, WALL, PLAYER, ENTITY, EXIT, ITEMHEAL }; // 타일의 상태에 대한 열거형 
enum entityType { NORMAL, MORERANGE, MOREHEALTH, MOREDAMAGE }; // 적대적 객체의 상태에 대한 열거형 
typedef enum { False, True } _boolean; // bool을 선언 
typedef enum { WHITE, RED, GREEN, LIGHTBLUE, BLUE, ORANGE, PURPLE, BLACK } COLOR; // 색깔에 대한 열거형 
typedef enum { MAINSCREEN, GAMESTART, GAMEEND, PROGRAMEND } _systemCode; // 게임 상태 플래그에 대한 열거형 
typedef enum { MAINSTORY, GAMEOVER, BOSS1, GAMEFINAL } gameStory;

// 구조체 선언
// 플레이어 구조체 
typedef struct _player {
	int x, y; // 플레이어의 위치 
	int health; // 플레이어의 체력 
	// int attackDamage;
	
	_player() { // 초기 위치, 체력 설정 
		x = 1;
		y = 1;
		health = 10;
	}
} Player;

// 적대적인 개체에 관한 구조체 
typedef struct _entity {
	int x, y; // 적대적인 개체의 위치 
	int health; // 적대적인 개체의 체력 
	int attackDamage; // 적대적인 개체의 공격력 
	enum entityType type; // 적대적인 개체의 형식 
	
	_entity() { // 초기 위치 및 상태 지정 
		x = 10;
		y = 10;
		health = 10;
		type = NORMAL;
	}
} Entity;

// 단위 격자 
typedef struct {
	int X, Y;
} MAPUNIT;

// 전역 변수 선언
_systemCode SystemCODE = MAINSCREEN; // 프로그램의 상태를 저장하는 변수 

int maze[MAZE_HEIGHT][MAZE_WIDTH]; // 미로 생성 

Entity entity[ENTITY_COUNT]; // 플레이어에 적대적인 생명체 선언 Entity라는 구조체로 이루어져 있음 

const int direction[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}}; // 방향 설정 

int backScreenBuffer[MAZE_HEIGHT][MAZE_WIDTH]; // 더블 버퍼링을 이용하기 위해 미로를 출력하는데 이용함 

Player playerMain; // 플레이어  

int entityUpdateTime; // 플레이어를 제외한 객체의 속도를 조절 

int timer; // 타이머 변수 
int level; // 레벨 변수 

int previousUnit; // 이전 유닛 변수(아이템 얻기에 이용됨) 

const int Rdirect[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // 상, 하, 좌, 우를 의미하며 벽을 고려하여 2칸씩 이동한다. 
int _maze[MAZE_HEIGHT][MAZE_WIDTH]; // 미로를 생성할 때 임시로 저장해놓는 변수 

int mazeWidth, mazeHeight; // 미로의 가로, 세로 크기 

// 파일 관리 
FILE* fp;
char location[] = "saves/savedata_"; // 파일의 기본 경로 
char location2[] = "saves/scenedata_"; // 파일의 기본 경로2

char fileList[][30] = {"princess", "princess", "missedprincess", "mawang", "king", "soldiers", "nights", "title"};
char fileList2[][30] = {"", "", "", "", "", "", "", ""};
char sceneList[][30] = {"intro_", "boss1_"};

/* 만든 함수 목록 */
// 게임 상태 처리 
int gameMain(); // 게임이 실질적으로 실행되면 실행하는 함수 
int gameSetting();
void gameEnd(); // 게임이 종료되면 실행하는 함수 

// 객체 처리 
int entityUpdate(); // 플레이어를 제외한 객체의 상태를 갱신하는 함수 

// 스토리 출력
void story(gameStory);
void printText(const char*, int);
void nextDelay(int);

// 화면 처리
void doubleBuffering(); // Double Buffering 기법을 이용하는 함수 
void forceBuffering(int, int, int, int); // 강제로 특정 화면을 갱신하는 함수 
void printUnit(int, int, int); // 화면의 격자에 대해 지정한 타일을 출력하는 함수 
void gotoxy(short, short); // 화면의 지정된 격자로 이동하는 함수 
int checkRange(int, int); // 범위 제한 
void backgroundColor(COLOR, _boolean); // 화면 색 설정

// 창 처리 
void fullScreen(); // 전체 화면을 위한 함수 

// 미로 생성 
void mazeGen(MAPUNIT); // 미로 생성에 대한 함수, 미로 만들기를 시작할 위치를 지정할 수 있음 
void mazeGenA(int, int); // 재귀적으로 미로를 생성하기 위한 함수 
int checkRange(int, int); // 범위를 확인하는 함수(미로 생성, 플레이어, 객체의 움직임의 제한에 이용됨) 
void mixDirection(int*, int); // 미로를 생성할 때, 랜덤하게 나아가기 위한 함수 

// 파일 관리 
char* getFilePath(char*); // 파일의 경로를 불러오는 함수, 파일의 존재성을 검사함 
int loadIMGData(char*); // 파일의 이미지 데이터를 불러오는 함수 
int loadTEXTData(char*, int); // 파일의 텍스트 데이터를 불러오는 함수 

// 이벤트 처리 
int eventHandler();
void bossFight(int);

// 문자열 처리
char* textMerge(char*, int);

// 메인화면 출력 및 관리 
int mainTitle();
int keyControl();
void init(char*);
int titleDraw(); 
int drawMenu(int);
int menuCode();
void infoDraw();

int main(void) {
	
	fullScreen(); // 우선 화면을 전체화면으로 만든다 
	
	story(MAINSTORY);
	
startProgram: // 프로그램 종료 메시지에서 아니오를 누르면 다시 돌아오기 위한 레이블 
	SystemCODE = MAINSCREEN; // 전체적인 시스템을 관리하기 위한 플래그 
	
	for(;SystemCODE != PROGRAMEND;) { // 만약 플래그가 프로그램을 끝내는 방향이 아니면 
		mainTitle(); // 게임 시작 전 메인화면을 계속 보여준다. 
	}
	
	// 게임 종료를 선택시에 나오는 확인 메시지 문구
	
	int message = MessageBox(NULL, "게임을 종료하시겠습니까?", "알림", MB_OKCANCEL | MB_TOPMOST | MB_ICONQUESTION); 
	if (message == IDCANCEL) goto startProgram; // 만약 게임을 종료하고 싶지 않다면 
	
	return 0; // 프로그램 종료 
}

int gameMain() {
	system("cls"); // 게임을 시작할 떄 우선 기본적으로 이전의 화면을 초기화함 
	
	level = 0; // 레벨을 초기화함 
	
	int i; // 임시 변수를 선언함 
	
	backgroundColor(WHITE, True); // 배경화면을 설정함 
	 
	playerMain.health = 10; // 게임을 재시작하였을 때를 고려하여 플레이어의 기본 체력을 재설정한다
	playerMain.x = 1; // 게임을 재시작하였을 때를 고려하여 플레이어의 기본 위치를 재설정한다 
	playerMain.y = 1; // 게임을 재시작하였을 때를 고려하여 플레이어의 기본 위치를 재설정한다
	
	mazeWidth = 15; // 게임을 재시작하였을 때를 고려하여 미로의 초기 크기를 재설정한다 
	mazeHeight = 15; // 게임을 재시작하였을 때를 고려하여 미로의 초기 크기를 재설정한다
	
	timer = 600;
	
	forceBuffering(0, 0, mazeWidth, mazeHeight); // 강제로 화면을 한번 초기화한다. 
	
	memset(maze, WALL, sizeof(maze)); // string.h
	memset(backScreenBuffer, WALL, sizeof(backScreenBuffer));
	
	mazeGen({1,1}); // 미로를 생성한다 
	
	maze[2][2] = ITEMHEAL; // 테스트, 아이템
	
	doubleBuffering(); // 더블 버퍼링으로 미로를 갱신한다. 
	
	Sleep(500); // 잠간의 시간지연을 둔다. 
	
	while(1) { // 게임오버가 되지 않으면 계속 실행 
		Sleep(50); // 게임의 속도를 조절하기 위한 시간지연 함수 
		
		// 만약 오른쪽 키나 D키가 눌리고 범위 안에 플레이어가 들어왔다면 
		if (((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState(0x44) & 0x8000)) && checkRange(playerMain.y, playerMain.x+1)) {
			if (maze[playerMain.y][playerMain.x+1] != WALL) { // 만약 가려는 위치에 벽이 없다면 
				maze[playerMain.y][playerMain.x] = EMPTY; // 원래 위치를 비우고 
				playerMain.x += 1; // 가려는 방향(오른쪽)으로 좌표를 이동 
				previousUnit = maze[playerMain.y][playerMain.x]; // 이전 타일을 저장(아이템을 먹기 위함) 
				maze[playerMain.y][playerMain.x] = PLAYER; // 새로운 위치에 플레이어를 표시하기 위해 미로에 갱신함 
			}
		}
		
		// 만약 왼쪽 키나 A키가 눌리고 범위 안에 플레이어가 들어왔다면 
		if (((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(0x41)) & 0x8000) && checkRange(playerMain.y, playerMain.x-1)) {
			if (maze[playerMain.y][playerMain.x-1] != WALL) { // 만약 가려는 위치에 벽이 없다면 
				maze[playerMain.y][playerMain.x] = EMPTY; // 원래 위치를 비우고 
				playerMain.x -= 1; // 가려는 방향(왼쪽)으로 좌표를 이동 
				previousUnit = maze[playerMain.y][playerMain.x]; // 이전 타일을 저장(아이템을 먹기 위함) 
				maze[playerMain.y][playerMain.x] = PLAYER; // 새로운 위치에 플레이어를 표시하기 위해 미로에 갱신함 
			}
		}
		
		// 만약 위쪽 키나 W키가 눌리고 범위 안에 플레이어가 들어왔다면 
		if (((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState(0x57) & 0x8000)) && checkRange(playerMain.y-1, playerMain.x)) {
			if (maze[playerMain.y-1][playerMain.x] != WALL) { // 만약 가려는 위치에 벽이 없다면 
				maze[playerMain.y][playerMain.x] = EMPTY; // 원래 위치를 비우고 
				playerMain.y -= 1; // 가려는 방향(위쪽)으로 좌표를 이동 
				previousUnit = maze[playerMain.y][playerMain.x]; // 이전 타일을 저장(아이템을 먹기 위함) 
				maze[playerMain.y][playerMain.x] = PLAYER; // 새로운 위치에 플레이어를 표시하기 위해 미로에 갱신함 
			}
		}
		
		// 만약 아래쪽 키나 S키가 눌리고 범위 안에 플레이어가 들어왔다면 
		if (((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState(0x53) & 0x8000)) && checkRange(playerMain.y+1, playerMain.x)) {
			if (maze[playerMain.y+1][playerMain.x] != WALL) { // 만약 가려는 위치에 벽이 없다면 
				maze[playerMain.y][playerMain.x] = EMPTY; // 원래 위치를 비우고 
				playerMain.y += 1; // 가려는 방향(아래쪽)으로 좌표를 이동 
				previousUnit = maze[playerMain.y][playerMain.x]; // 이전 타일을 저장(아이템을 먹기 위함) 
				maze[playerMain.y][playerMain.x] = PLAYER; // 새로운 위치에 플레이어를 표시하기 위해 미로에 갱신함 
			}
		}
		
		// 만약 플레이어가 도착지에 도착했다면 
		if ((playerMain.x == mazeWidth-2) && (playerMain.y == mazeHeight-2) && level%4 == 0) {
			level += 2; // 레벨을 증가시킨다 
			mazeGen({1,1}); // 미로를 새로 만든다 
			backgroundColor(WHITE, True); // 배경화면 색을 설정한다 
			playerMain.x = mazeWidth-2; // 플레이어의 위치를 재설정한다 
			playerMain.y = mazeHeight-2; // 플레이어의 위치를 재설정한다
			timer += 200 + level*20;
			Sleep(10); // 잠깐 쉰다
			eventHandler();
		} else if ((playerMain.x == 1) && (playerMain.y == 1) && level%4 == 2) {
			level += 2;
			mazeGen({mazeWidth-2, mazeHeight-2});
			backgroundColor(WHITE, True);
			timer += 200 + level*20;
			Sleep(10);
			eventHandler();
		}
		
		// 만약 R키가 눌린다면(테스트를 위한 함수)
		if (GetAsyncKeyState(0x52) & 0x8000) {
			level += 2;
			
			eventHandler();
			
			if (level%4 == 0) {
				mazeGen({1,1});
				
				playerMain.x = 1;
				playerMain.y = 1;
			} else {
				mazeGen({mazeWidth-2, mazeHeight-2});
				
				playerMain.x = mazeWidth-2;
				playerMain.y = mazeHeight-2;
			}
			
			timer = 600;
		}
		
		// 만약 ESC가 눌린다면 
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			goto gameEND; // 게임을 끝내기 
		}
		
		entityUpdateTime += 1; // 객체 갱신 시간 
		
		if (entityUpdateTime%3 == 0) { // 객체는 플레이어를 3번 갱신할 때 1번 갱신한다 
			entityUpdate(); // 객체 갱신 함수 
			entityUpdateTime = 0; // 갱신 시간 초기화 
		}
		
		// 만약 적대적 객체와 플레이어가 접촉하였다면 
		for (i=0;i<ENTITY_COUNT;i++) {
			if (((playerMain.x-entity[i].x)*(playerMain.x-entity[i].x)
			+ (playerMain.y-entity[i].y) * (playerMain.y-entity[i].y)) <= 1.1) {
				playerMain.health -= 1;
			}
		}
		
		// 만약 플레이어의 체력이 0이하라면(죽음) 
		if (playerMain.health <= 0) {
			goto gameEND; // 게임을 끝내기 
		}
		
		// 이전 타일이 아이템인지, 어떤 아이템인지 검사 
		switch (previousUnit) {
			case ITEMHEAL: // 피 회복 아이템 
				playerMain.health += 5; // 피를 일정량만큼 채운다 
				break;
			default: // 아이템 목록에 없으면 그냥 넘기기 
				break;
		}
		
		previousUnit = -1; // 이전 유닛을 -1로 만들어서 아이템이 중복해서 들어오는 버그를 수정 
		
		timer -= 1; // 타이머 변수에서 1을 빼기 
		
		if (timer < 0) { // 만약 시간 초과되었다면? 
			goto gameEND; // 게임을 끝내기 
		}
		
		doubleBuffering(); // 더블 버퍼링, 갱신한 화면을 실제로 적용하기 
		
		// 정보들을 보여주기(임시)
		backgroundColor(WHITE, False);
		gotoxy(0, MAZE_HEIGHT+2); 
		printf("Location: (%03d, %03d)\n\n", playerMain.x, playerMain.y);
		printf("Health: %03d\n", playerMain.health);
		printf("level: %03d\n", (int)(level/2));
		printf("time left: %03d\n", (int)(timer/20));
	}

gameEND: // 게임 끝남 
	
	gameEnd(); // 게임을 끝내는 함수 
	
	return 0;
}

// 객체 갱신 함수 
int entityUpdate() {
	int i, j; // 변수들을 선언 
	int random = 0; // 랜덤 변수 선언(방향을 정하기 위함) 
	
	for (i=0;i<ENTITY_COUNT;i++) {
		random = rand()%4; // 랜덤한 값을 대입한다.
		
		// 만약 가려는 방향에 출구, 아이템, 벽이 있거나 미로의 범위를 넘어간다면 
		if (!checkRange(entity[i].y+direction[random][1], entity[i].x+direction[random][0])) {
			continue; // 움직이지 않는다. 
		} else if (maze[entity[i].y+direction[random][1]][entity[i].x+direction[random][0]] == WALL) {
			continue; // 움직이지 않는다.
		} else if (maze[entity[i].y+direction[random][1]][entity[i].x+direction[random][0]] == EXIT) {
			continue; // 움직이지 않는다.
		} else if (maze[entity[i].y+direction[random][1]][entity[i].x+direction[random][0]] == ITEMHEAL) {
			continue; // 움직이지 않는다.
		}
		
		// 위의 조건에 만족하지 않는다면 다음을 실행한다. 
		maze[entity[i].y][entity[i].x] = EMPTY; // 원래 위치에서의 객체를 없앤다 
		
		entity[i].x += direction[random][0]; // 좌표를 갱신한다 
		entity[i].y += direction[random][1]; // 좌표를 갱신한다
		
		maze[entity[i].y][entity[i].x] = ENTITY; // 새로운 위치에 객체를 추가한다
	}
}

int eventHandler() {
	int num = (int)(level/2);
	
	switch (num) {
		case 10:
			bossFight(0);
			break;
		default:
			playerMain.health += 5;
			break;
	}
}

void bossFight(int bossLevel) {
	// system("cls");
	
	memset(maze, EMPTY, sizeof(maze));
	
	doubleBuffering();
	
	mazeWidth = 101;
	mazeHeight = 31;
	
	switch(bossLevel) {
		case 0:
			// exit(-1);
			break;
		default:
			break;
	}
	
	// TESTBOSS1
	backgroundColor(WHITE, False);
	// gotoxy(,);
	story(BOSS1);
	printText("", 30);
	
	while(1) {
		 
	}
}

// 게임이 끝났을 경우에 대한 함수 
void gameEnd() {
	
	int i, j; // 변수를 선언한다. 
	
	// 화면 색을 설정한다. 
	backgroundColor(BLACK, True);
	backgroundColor(WHITE, False);
	gotoxy(3, mazeWidth/2-4); // 특정 위치로 이동한다
	for(i=0;i<50;i++) { // '='를 50개 출력한다 
		printf("=");
	}
	
	for(j=0;j<5;j++) { // 빈칸을 여러 줄에 걸쳐 출력한다 
		gotoxy(3, mazeWidth/2-3+j);
		
		for(i=0;i<50;i++) {
			printf(" ");
		}
	}
	
	gotoxy(3, mazeWidth/2+2); // 빈칸 아래줄로 이동한다 
	for(i=0;i<50;i++) { // '='를 50개 출력한다 
		printf("=");
	}
	
	// 색을 새로 설정한다. 
	backgroundColor(WHITE, True);
	backgroundColor(WHITE, False);
	gotoxy(10, mazeWidth/2-1);
	printf("용사는 오렌지 병으로 쓰러졌다\n"); // 사망 문구를 보여준다. 
	
	// 초당 한번씩 문구를 갱신한다. 
	int secTimer = 2;
	int milsecTimer = 9;
	
	// 시간 
	while (secTimer >= 0) {
		Sleep(99);
		
		gotoxy(10, mazeWidth/2);
		printf("▶ %d.%d초 뒤에 메인화면으로 나갑니다", secTimer, milsecTimer);
		milsecTimer -= 1;
		
		if (milsecTimer < 0) {
			secTimer -= 1;
			milsecTimer = 9;
		}
	}
	
	fflush(stdin);
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

// Double Buffering을 위한 함수 
void doubleBuffering() {
	int i, j; // 변수를 선언한다 
	
	// 미로의 각 격자에 대해 다음 작업을 수행한다 
	for (i=0;i<mazeHeight;i++) {
		for (j=0;j<mazeWidth;j++) {
			if (maze[i][j] != backScreenBuffer[i][j]) { // 만약 이전 상태의 칸과 현재 상태의 칸이 다르면 
				printUnit(j, i, maze[i][j]); // 다른 칸을 출력한다 
				backScreenBuffer[i][j] = maze[i][j]; // 그 칸을 갱신한다 
			}
		}
	}
}

// 강제로 화면 갱신을 위한 함수 
void forceBuffering(int x, int y, int dx, int dy) {
	int i, j;
	
	// 입력받은 범위에 대해 다음 작업을 수행한다 
	for (i=y;i<y+dy;i++) {
		for (j=x;j<x+dx;j++) {
			printUnit(j, i, maze[i][j]); // 조건 없이 칸을 출력한다 
			backScreenBuffer[i][j] = maze[i][j]; // 그 칸을 갱신한다 
		}
	}
}

// 칸을 출력하기 위한 함수 
void printUnit(int x, int y, int chara) {
	gotoxy(x+1, y+1); // 화면 출력을 살짝 이동함 
	
	// backgroundColor(BLACK, True);
	
	switch(chara) { // 출력해야 할 칸을 파악한다 
		case PLAYER: // 만약 출력해야 하는 칸이 플레이어 칸이라면 
			backgroundColor(ORANGE, True); // 배경색을 오렌지색으로 설정한다 
			break;
		case WALL: // 만약 출력해야 하는 칸이 벽이 있는 칸이라면
			backgroundColor(WHITE, True); // 배경색을 흰색으로 설정한다 
			break;
		case EMPTY: // 만약 출력해야 하는 칸이 비어있는 칸이라면
			backgroundColor(BLACK, True); // 배경색을 검정색으로 설정한다 
			break;
		case ENTITY: // 만약 출력해야 하는 칸이 객체에 대한 칸이라면
			backgroundColor(RED, True); // 배경색을 빨간색으로 설정한다 
			break;
		case EXIT: // 만약 출력해야 하는 칸이 출구 칸이라면 
			backgroundColor(LIGHTBLUE, True); // 배경색을 하늘색으로 설정한다 
			break;
		case ITEMHEAL: // 만약 출력해야 하는 칸이 아이템이라면 
			backgroundColor(GREEN, True); // 배경색을 초록색으로 설정한다 
			break;
		default: // 만약 출력해야 하는 칸이 정의되지 않은 칸이라면 
			exit(-1); // 프로그램을 강제로 종료한다 
			break;
	}
	
	printf(" ");
}

// 미로 생성에 대한 함수 
void mazeGen(MAPUNIT startPoint) {
	int i, j; // 변수를 선언한다 
	
	srand((unsigned int)time(NULL)); // 랜덤 함수 시드 초기화 
	// 원하는 값(M_WALL)로 map의 사이즈만큼 map에 메모리 할당, 즉 map을 전부 벽으로 설정 
	memset(_maze, MWALL, sizeof(_maze)); // string.h

	// 미로 생성(재귀적) 
	mazeGenA(startPoint.Y, startPoint.X);
	
	// 출구 만들기
	if (level%4 == 0) _maze[mazeHeight-2][mazeWidth-2] = MEXIT;
	else _maze[1][1] = MEXIT;
	
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
				case MEXIT:
					maze[i][j] = EXIT;
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
	
	int temp, i; // 임시 변수들을 선언한다 
	int randomNum; // 랜덤한 수를 담을 변수 정의 
	
	for (i=0;i<(num-1);i++) { // num-1번에 대해 순서를 섞는다 
		randomNum = rand()%(num-i)+i; // 랜덤한 수를 하나 정의한다 
		// 두 칸의 숫자를 바꾼다 
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
	
	mazeWidth = level+15<MAZE_WIDTH?(15 + level):MAZE_WIDTH; // mazeWidth를 갱신한다 
	mazeHeight = level+15<MAZE_HEIGHT?(15 + level):MAZE_HEIGHT; // mazeHeight를 갱신한다
	// 단, 주어진 변수를 갱신할 떄, 최대 가능한 칸을 넘어가지 않기 위해 삼항 연산자를 이용한다 
	
	return (x < mazeWidth - 1 && x > 0) && (y < mazeHeight - 1 && y > 0); // 만약 입력받은 칸이 범위 내라면 1을 반환 
}

// 미로 생성 알고리즘 (Recursive Backtracking 방법)
void mazeGenA(int y, int x) {
	int i = 0; // 변수를 선언한다 
	int nx, ny; // 이전 칸에서 앞으로 나아갈 새로운 칸 
	int direction[4] = { LEFT, RIGHT, TOP, BOTTOM }; // 방향을 선언한다 
	
	_maze[y][x] = MEMPTY; // 현재 있는 위치를 빈칸으로 만든다
	
	// 가려는 방향을 섞는다
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

// 파일 경로 불러오는 함수(문자열을 반환하기 위해 char*이다)
char* getFilePath(char* filename) {
	char* Ifilename = (char*)calloc(100, sizeof(char)); // 파일 이름을 저장할 변수를 선언한다 
	
	strcat(Ifilename, location); // 파일 기본 경로를 변수에 합친다 
	strcat(Ifilename, filename); // 파일의 실제 이름을 변수에 합친다 
	strcat(Ifilename, ".txt"); // 확장자를 변수에 합친다 
	
	// printf("%s\n\n", Ifilename);
	// free(Ifilename);
	
	return Ifilename; // 경로를 포함한 전체 이름을 반환한다
}

// 데이터를 불러오는 함수 
int loadIMGData(char* filename) {
	if((fp = fopen(filename, "rt")) == NULL) { // 만약 파일이 존재하지 않으면 
		return -1; // -1을 반환하며 함수를 끝낸다 
	}
	
	// 파일이 있으면 다음으로 진행한다 
	int i = 0; // 변수를 선언한다 
	char buffer[100]; // 파일의 한 줄을 받아올 변수이다 
	
	while(!feof(fp)) { // 더 이상 받아올 데이터가 없을 때까지 아래의 내용을 실행한다 
		fgets(buffer, sizeof(buffer), fp); // buffer의 크기만큼 한줄씩 불러온다 
		
		printf("%s", buffer);
		// Data[i] = buffer; // 받아온 값을 Data에 저장한다 
		// printf("%s", Data[i]); // 임시(출력하여 잘 받아와졌는지 확인하기 위한 용도) 
		i += 1; // Data의 다음 행에 저장하기 위해 임시 변수를 하나 더해준다 
	}
	
	fclose(fp); // 사용된 fp를 닫아서 메모리 누수를 막는다 
	return i;
}

int loadTEXTData(char* filename, int delay) {
	char* Ifilename = (char*)calloc(100, sizeof(char)); // 파일 이름을 저장할 변수를 선언한다 
	
	strcat(Ifilename, location2); // 파일 기본 경로를 변수에 합친다 
	strcat(Ifilename, filename); // 파일의 실제 이름을 변수에 합친다 
	strcat(Ifilename, ".txt"); // 확장자를 변수에 합친다 
	
	if((fp = fopen(Ifilename, "rt")) == NULL) { // 만약 파일이 존재하지 않으면 
		return -1; // -1을 반환하며 함수를 끝낸다 
	}
	
	// 파일이 있으면 다음으로 진행한다 
	int i = 0; // 변수를 선언한다 
	char buffer[100]; // 파일의 한 줄을 받아올 변수이다 
	
	while(!feof(fp)) { // 더 이상 받아올 데이터가 없을 때까지 아래의 내용을 실행한다 
		fgets(buffer, sizeof(buffer), fp); // buffer의 크기만큼 한줄씩 불러온다 
		
		printText(buffer, delay);
		// Data[i] = buffer; // 받아온 값을 Data에 저장한다 
		// printf("%s", Data[i]); // 임시(출력하여 잘 받아와졌는지 확인하기 위한 용도) 
		i += 1; // Data의 다음 행에 저장하기 위해 임시 변수를 하나 더해준다  
	}
	
	fclose(fp); // 사용된 fp를 닫아서 메모리 누수를 막는다 
	return i;
}

// 입력받은 격자로 이동하기 위한 함수(Windows Api를 이용함) 
void gotoxy(short x, short y) {
	COORD pos = {x, y}; // 위치를 받는다 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); // 커서의 위치를 변경하는 함수를 이용한다 
}

// 전체화면을 만드는 함수 
void fullScreen() {
    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0); // 콘솔의 출력창의 모드를 변경하여 전체화면으로 만든다 
}

// 배경색을 설정하는 함수 
void backgroundColor(COLOR c, _boolean isBackground) {
	WORD colorType; // 색의 타입을 설정한다 
	
	// 설정하려는 것이 배경색이라면 
	if (isBackground == True) {
		goto background; // 배경색을 설정하는 명령으로 이동한다 
	}
	
	switch(c) { // 설정하려는 색을 파악한다 
		case WHITE:
			colorType = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;
		case RED:
			colorType = FOREGROUND_RED;
			break;
		case ORANGE:
			colorType = FOREGROUND_RED | FOREGROUND_GREEN;
			break;
		case GREEN:
			colorType = FOREGROUND_GREEN;
			break;
		case LIGHTBLUE:
			colorType = FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;
		case BLUE:
			colorType = FOREGROUND_BLUE;
			break;
		case PURPLE:
			colorType = FOREGROUND_RED | FOREGROUND_BLUE;
			break;
		default:
			colorType = NULL;
			break;
	}
	
	goto paintcolor; // 색을 실제로 설정하기 위한 곳으로 이동한다 
	
background:
	switch(c) { // 설정하려는 색을 파악한다 
		case WHITE:
			colorType = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
			break;
		case RED:
			colorType = BACKGROUND_RED;
			break;
		case ORANGE:
			colorType = BACKGROUND_RED | BACKGROUND_GREEN;
			break;
		case GREEN:
			colorType = BACKGROUND_GREEN;
			break;
		case LIGHTBLUE:
			colorType = BACKGROUND_GREEN | BACKGROUND_BLUE;
			break;
		case BLUE:
			colorType = BACKGROUND_BLUE;
			break;
		case PURPLE:
			colorType = BACKGROUND_RED | BACKGROUND_BLUE;
			break;
		default:
			colorType = NULL;
			break;
	}
	
paintcolor:
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorType); // 콘솔의 텍스트 속성을 변경하여 색을 변경한다 
}

char* textMerge(char* text, int num) {
	char strBuffer[30];
	sprintf(strBuffer, "%s%d", text, num);
	return strBuffer;
}

void story(gameStory type) {
	int sceneNum;
	
	switch(type) {
		case MAINSTORY:
			goto _mainstory;
			break;
		case BOSS1:
			goto _boss1;
			break;
		case GAMEOVER:
			break;
		case GAMEFINAL:
			break;
		default:
			break;
	}
	
_mainstory:
	for(sceneNum=0;sceneNum<8;sceneNum++) {
		system("cls");
		int imageLine = loadIMGData(getFilePath(fileList[sceneNum]));
		
		gotoxy(0, imageLine+1);
		loadTEXTData(textMerge(sceneList[0],(sceneNum+1)), 10);
		
		nextDelay(300);
		
		for(;;) {
			if(GetAsyncKeyState(VK_RETURN) & 0x8000) {
				break;
			} else if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				goto _endStory;
			}
		}
	}
	
	goto _endStory;
	
_boss1:
	for(sceneNum=0;sceneNum<10;sceneNum++) {
		int imageLine = loadIMGData(getFilePath(fileList2[sceneNum]));
		
		gotoxy(0, imageLine+1);
		loadTEXTData(sceneList[1], 10);
		
		nextDelay(300);
		
		for(;;) {
			if(GetAsyncKeyState(VK_RETURN) & 0x8000) {
				break;
			} else if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				goto _endStory;
			}
		}
	}

_endStory:
	return;
}

void nextDelay(int delayTime) {
	CONSOLE_SCREEN_BUFFER_INFO cursorData;
	
	Sleep(delayTime);
	
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorData);
	
	gotoxy(1, cursorData.dwCursorPosition.Y+2);
	printText("▷ Enter키를 눌러 진행", 10);
	gotoxy(1, cursorData.dwCursorPosition.Y+3);
	printText("▷ Esc키를 눌러 스킵", 10);
}

void printText(const char* text, int time) {
	int i;
	
	for(i=0;i<strlen(text);i++) {
		Sleep(time);
		printf("%c", text[i]);
	}
}

int gameSetting() {
	system("cls");
	
	int difficulty = 0;
	_boolean isUpdate;
	
	gotoxy(5, 5);
	backgroundColor(WHITE, False);
	printText("난이도: ", 5);
	backgroundColor(GREEN, False);
	printText("EASY", 5);
	
	gotoxy(5, 7);
	backgroundColor(WHITE, False);
	printText("▷ 엔터를 눌러서 진행", 5);
	gotoxy(5, 8);
	printText("▷ ", 5);
	backgroundColor(LIGHTBLUE, False);
	printText("← →", 5);
	backgroundColor(WHITE, False);
	printText("를 눌러서 난이도 설정", 5);
	
	Sleep(500);
	
	while(1) {
		if(GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (difficulty > 0) {
				difficulty -= 1;
				isUpdate = True;
			}
		} else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (difficulty < 2) {
				difficulty += 1;
				isUpdate = True;
			}
		} else if(GetAsyncKeyState(VK_RETURN) & 0x8000) {
			break;
		}
		
		if (isUpdate == True) {
			gotoxy(5, 5);
			backgroundColor(WHITE, False);
			printf("난이도: ");
			
			switch (difficulty) {
				case 0:
					backgroundColor(GREEN, False);
					printText("EASY  ", 5);
					break;
				case 1:
					backgroundColor(ORANGE, False);
					printText("NORMAL", 5);
					break;
				case 2:
					backgroundColor(RED, False);
					printText("HARD  ", 5);
					break;
				default:
					break;
			}
			
			Sleep(100);
			
			isUpdate = False;
		}
	}
	
	return difficulty;
}

/*
이혜원이 만든 코드 
*/

// 게임 시작 전 메인 화면 
int mainTitle() {
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	system("cls"); // 화면을 한번 초기화한다(다른 화면에서 돌아올 때를 위함) 
    init("장씨 구출기"); // 창의 이름 등의 기본 설정을 진행한다 
    
	while(1) {
    	int titleLength = titleDraw();
	    int menuCode = drawMenu(titleLength);
	    
	    switch(menuCode) {
	    	case 0:
	    		gameSetting();
	    		gameMain();
				break;
	    	case 1:
	    		infoDraw();
				break;
	    	case 2:
	    		SystemCODE = PROGRAMEND;
	    		break;
	    	default:
	    		break;
		}
	    
	    system("cls");
	}
	
	return 0;
}

void infoDraw() {
	system("cls");
	printf("\n\n");
	printf("\t\t\t[ 조작법 ]\n\n");
	printf("\t\t이동: W, A, S, D\n");
	printf("\t\t선택: 스페이스바\n\n\n\n\n\n\n");
	printf("\t개발자: 공현성, 유호창, 이혜원\n\n");
	printf("\t스페이스바를 누르면 메인화면으로 이동합니다.");
	
	while(1) {
		if(keyControl() == SUBMIT){
			break;
		}
	}
} 

int keyControl() {
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	Sleep(200);
	
	while (1) {
		
		// 만약 오른쪽 키나 D키가 눌린다면 
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState(0x44) & 0x8000)) {
			return RIGHT;
		}
		// 만약 왼쪽 키나 A키가 눌린다면
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(0x41) & 0x8000)) {
			return LEFT;
		}
		// 만약 엔터 키나 스페이스 키가 눌린다면 
		if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000)) {
			return SUBMIT;
		}
	}
}

void init(char* gameTitle) {
	SetConsoleTitle(gameTitle);
	PlaySoundA((LPCSTR)"saves/test1.mp3", NULL, SND_FILENAME | SND_ASYNC);
}

int titleDraw() {
	printf("\n\n");
	return loadIMGData(getFilePath("title"));
}

int drawMenu(int titleLength) {
	
	titleLength += 2;
	int menuPointer = 0;
	
	while(1) {
		switch (menuPointer) {
			case 0:
				gotoxy(15, titleLength);
				backgroundColor(GREEN, False);
				printf("▷ 게임시작  ");
				backgroundColor(WHITE, False);
				gotoxy(45, titleLength);
				printf("   게임정보  ");
				gotoxy(75, titleLength);
				printf("   종료      ");
				break;
			case 1:
				gotoxy(15, titleLength);
				printf("   게임시작  ");
				gotoxy(45, titleLength);
				backgroundColor(GREEN, False);
				printf("▷ 게임정보  ");
				backgroundColor(WHITE, False);
				gotoxy(75, titleLength);
				printf("   종료      ");
				break;
			case 2:
				gotoxy(15, titleLength);
				printf("   게임시작  ");
				gotoxy(45, titleLength);
				printf("   게임정보  ");
				gotoxy(75, titleLength);
				backgroundColor(GREEN, False);
				printf("▷ 종료      ");
				backgroundColor(WHITE, False);
				break;
			default:
				break;
		}
		
		gotoxy(2, titleLength+3);
		backgroundColor(WHITE, False);
		printf("▷ Space키를 눌러서 진행");
		gotoxy(2, titleLength+4);
		printf("▷ ");
		backgroundColor(LIGHTBLUE, False);
		printf("← →");
		backgroundColor(WHITE, False);
		printf("를 눌러서 커서 이동");
		
		switch(keyControl()) {
			case LEFT:
				if (menuPointer > 0) menuPointer -= 1;
				break;
			case RIGHT:
				if (menuPointer < 2) menuPointer += 1;
				break;
		    case SUBMIT:
			    return menuPointer;
			default:
				break;
		}
	}
}

