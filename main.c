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
  //int vertsCount=cube.stats.verts;
  //float xCoord=cube.verts[29].z;
  //for(int i=0; i<vertsCount;i++){
    //printf(" v: %f,%f,%f   \n",cube.verts[i].x,cube.verts[i].y,cube.verts[i].z);
  //}
  
  freeObj(&cube);
  return 0;
}

