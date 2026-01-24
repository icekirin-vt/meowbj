#include <stdio.h>
#include "meowbj.h"



int main(void)
{
	printf("hello world \n");  
	meowObj *cube=loadObj("sponza.obj");
	
	if (cube == NULL ){
		printf("loading failed, exit code\n");
		freeObj(cube);
		return 0;
	}
	
	//int faceCount= cube->stats.faces;
	meowStats fileStats=cube->stats;
	printf(" stats: \nv:%zu, n:%zu, t:%zu, faces:%zu\n",fileStats.vert,fileStats.norm,fileStats.texc,fileStats.faces);
	
	freeObj(cube);
	return 0;
}
 
