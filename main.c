#include <stdio.h>
#include "meowbj.h"



int main(void)
{
	printf("hello world \n");  
	meowObj cube;
	int success=loadObj("teapot.obj", &cube);
	printf("load success:%d \n",success);

	if (success!=0){
		printf("loading failed, exit code :%d",success);
		freeObj(&cube);
		return 0;
	}

	printf("");
	int vertsCount=cube.stats.vert;
	for(int i=0; i<vertsCount;i++){
		printf(" v: %f,%f,%f   \n",cube.vert[i].x,cube.vert[i].y,cube.vert[i].z);
	}

	freeObj(&cube);
	return 0;
}

