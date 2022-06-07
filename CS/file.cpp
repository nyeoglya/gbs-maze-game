#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#include <windows.h>

#define MAZE_WIDTH		100
#define MAZE_HEIGHT		50

// 전역 변수 선언
FILE* fp;
char location[] = "saves/savedata_";

char preData[9][20] = {"[data]", "author=", "mapname=", "wsize", "hsize", "xstartpoint", "ystartpoint", "", "[content]"};
char readPreData[6][100];

char Data[100][5000]; // [MAZE_WIDTH * MAZE_HEIGHT]

char maze[MAZE_HEIGHT][MAZE_WIDTH];

int wsize, hsize, xstartpoint, ystartpoint;
char* author, mapname;

// 함수 선언 
char* getFilePath(char*);

void saveData(char*, int, int);
int loadData(char*);
void deleteData(char *);
void modifyData();

void saveMazeData();

// 파일 메인 함수 
int main(void) {
	int i = 0;
	
	char data[100];
	
	for(;i<2;i++) {
		scanf("%s", data);
		strcat(preData[i+1], (const char*) data);
	}
	
	wsize = 1000;
	hsize = 1000;
	xstartpoint = 1;
	ystartpoint = 1;
	
	saveData(getFilePath(data), 12, 23); // 임시, 파일명 수정하기 
	// loadData(getFilePath(data));
	
	return 0;
}

// 파일 경로 불러오는 함수 
char* getFilePath(char* filename) {
	char* Ifilename = (char*)calloc(100, sizeof(char));
	
	strcat(Ifilename, location);
	strcat(Ifilename, filename);
	strcat(Ifilename, ".txt");
	
	printf("%s\n\n", Ifilename);
	
	// free(Ifilename);
	
	return Ifilename;	
}

// 새로운 파일을 만들어 데이터를 저장하는 함수 
void saveData(char* filename, int words, int lines) {
	int i = 0;
	
	fp = fopen(filename, "wt");
	
	for(;i<9;i++) {
		fprintf(fp, "%s\n", preData[i]);
	}
	
	saveMazeData();
	
	fclose(fp);
}

void saveMazeData() {
	int i = 0;
	
	for(;i<MAZE_HEIGHT;i++) {
		fprintf(fp, "%s", maze[i]);
	}
}

// 기존의 데이터를 불러오는 함수 
int loadData(char* filename) {
	if((fp = fopen(filename, "rt")) == NULL) {
		printf("파일이 존재하지 않습니다.\n\n");
		
		return -1;
	} else {
		printf("세이브파일을 발견했습니다.\n\n");
	}
	
	int i = 0;
	char buffer[5000];
	
	// fscanf(fp, "%s=%s", readPreData[0], readPreData[1]);
	while(!feof(fp)) {
		fgets(buffer, sizeof(buffer), fp);
		
		strcpy(Data[i], buffer);
		printf("%s", Data[i]);
		i += 1;
	}
	
	fclose(fp);
	return 0;
}

// 세이브 파일을 삭제하는 함수 
void deleteData(char* filename) {
	if((fp = fopen(filename, "rt")) == NULL) {
		printf("파일이 존재하지 않습니다.");
	} else {
		printf("파일 삭제를 진행합니다.");
		fclose(fp);
		remove(filename);
	}
}


