#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#define MAZE_WIDTH		100
#define MAZE_HEIGHT		50

char* getFilePath(char*);
int loadData(char*);

// 전역 변수 선언
FILE* fp;
char location[] = "saves/savedata_";

char Data[100][5000]; // [MAZE_WIDTH * MAZE_HEIGHT]

char maze[MAZE_HEIGHT][MAZE_WIDTH];

int main(void) {
	
	loadData(getFilePath("nights"));
	
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

