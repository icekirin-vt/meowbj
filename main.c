#include <stdio.h>
#include "meowbj.h"

int main(void)
{
  printf("hello world \n");
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  mobj_obj cube=loadObj("cube.obj");
  gettimeofday(&stop, NULL);
  printf("loading a cube model took  %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
  for (int i=0;i<36;i++){
    printf("v:%d  %f,%f,%f with normal: %f,%f,%f and uv: %f,%f\n", i, cube.verts[i].x, cube.verts[i].y, cube.verts[i].z, cube.norms[i].x, cube.norms[i].y, cube.norms[i].z, cube.uvs[i].u,cube.uvs[i].v);
  }
  //float xCoord=cube.verts[29].z;
  //printf("Vertex nr 33 is at %f", xCoord);

  
  freeObj(&cube);
  return 0;
}

