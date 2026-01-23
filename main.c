#include <stdio.h>
#include "meowbj.h"



int main(void)
{
	printf("hello world \n");  
	meowObj *cube=loadObj("../../plane.obj");
	
	FILE *debugFile;
	debugFile=fopen("debugFile.obj", "wb");

	if (cube == NULL ){
		fclose(debugFile);
		printf("loading failed, exit code\n");
		freeObj(cube);
		return 0;
	}
	
	int faceCount= cube->stats.faces;
	for(int i=0; i<faceCount*3;i++){
		fprintf(debugFile," v %f,%f,%f   \n",cube->vert[i].x,cube->vert[i].y,cube->vert[i].z);

	}
	meowStats fileStats=cube->stats;
	printf(" stats: \nv:%zu, n:%zu, t:%zu, faces:%zu\n",fileStats.vert,fileStats.norm,fileStats.texc,fileStats.faces);
	
	fclose(debugFile);
	freeObj(cube);
	return 0;
}
 
