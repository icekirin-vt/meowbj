#include <stdio.h>
#include "meowbj.h"

int main(void)
{
  printf("hello world \n");
  mobj_obj cube=loadObj("cube.obj");
  mobj_filestats stats=cube.stats;
  printf("Vertex:\t%zu\nFaces: \t%zu\n", stats.verts, stats.faces);
  return 0;
}
