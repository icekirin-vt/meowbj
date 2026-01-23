#include <stdio.h>
#include "meowbj.h"



int main(void)
{
	printf("hello world \n");  
	meowObj *cube=loadObj("pathHere.obj");
	
	if (cube == NULL ){
		fclose(debugFile);
		printf("loading failed, exit code\n");
		freeObj(cube);
		return 0;
	}
	
	int faceCount= cube->stats.faces;
f	meowStats fileStats=cube->stats;
	printf(" stats: \nv:%zu, n:%zu, t:%zu, faces:%zu\n",fileStats.vert,fileStats.norm,fileStats.texc,fileStats.faces);
	
	(debugFile);
	freeObj(cube);
	return 0;
}
 
