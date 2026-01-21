#ifndef MEOWBJ_H
#define MEOWBJ_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
  float x,y,z;
}mobj_vec3;

typedef struct{
  float u,v;
}mobj_vec2;

typedef struct{
  size_t verts,normals,uvs,faces;
}mobj_filestats;

char *mobj_loadFile(char path[])
{
  FILE *filePointer=fopen(path, "rb");
  fseek(filePointer, 0L, SEEK_END);    //seeks to end of file to get its size for future malloc
  size_t fileSize=ftell(filePointer);
  rewind(filePointer);

  char *fileContents=malloc((fileSize+1)*sizeof(char));
  fread(fileContents, fileSize, 1,filePointer);
  
  return fileContents;
}

typedef struct{
  mobj_vec3 *verts;
  mobj_vec3 *norms;
  mobj_vec2 *uvs;
  mobj_filestats stats;
}mobj_obj;
  


mobj_filestats mobj_getFileStats(char *file)
{
  
  size_t vertexCount=0;
  size_t normalCount=0;
  size_t uvCount=0;
  size_t faceCount=0;
 
  char *currentLine=malloc(strlen(file)*sizeof(char));
  strcpy(currentLine,file);
  while(currentLine){
    char *nextLine=strchr(currentLine, '\n');
    if(nextLine) *nextLine='\0';
    //printf("currentLine=[%s]\n",currentLine);
    char lineattrib[3];
    strncpy(lineattrib, currentLine, 2);
    if (strcmp(lineattrib,"v ")==0){
      vertexCount++;
    }
    else if (strcmp(lineattrib,"vn")==0){
	normalCount++;
      }
    else if (strcmp(lineattrib,"vt")==0){
      uvCount++;
      }
    else if (strcmp(lineattrib,"f ")==0){
      faceCount++;
      }
    
    if(nextLine) *nextLine='\n';
    currentLine=nextLine ? (nextLine+1) : NULL;
  }
  free(currentLine);
  mobj_filestats result={
    .verts=vertexCount,
    .normals=normalCount,
    .uvs=uvCount,
    .faces=faceCount
  };
  
  return result;
}


mobj_obj loadObj(char path[])
{
  char *fileContents=mobj_loadFile(path);
  mobj_filestats fileStats=mobj_getFileStats(fileContents);

  mobj_vec3 vertexStore[fileStats.verts];
  mobj_vec3 normalStore[fileStats.normals];
  mobj_vec3 uvStore[fileStats.uvs];

  size_t vertexCount=0;
  size_t normalCount=0;
  size_t uvCount=0;
  size_t faceCount=0;
  
  char *currentLine=fileContents;
  while(currentLine){
    char *nextLine=strchr(currentLine, '\n');
    char lineattrib[3];
    strncpy(lineattrib,currentLine,2);
    if (strcmp(lineattrib,"v ")==0){
      //int i=0;
      mobj_vec3 tempVec={0};
      char floatString[29];
      strncpy(floatString,currentLine+(sizeof(char)*2),29);
      printf("floatString: %s\n",floatString);
      //vertexStore[vertexCount];
      vertexCount++;
    }
    else if (strcmp(lineattrib,"vn")==0){
	normalCount++;
      }
    else if (strcmp(lineattrib,"vt")==0){
      uvCount++;
      }
    else if (strcmp(lineattrib,"f ")==0){
      faceCount++;
      }
    
    if(nextLine) *nextLine='\n';
    currentLine=nextLine ? (nextLine+1) : NULL;
  }
  
 
  free(fileContents);
  mobj_obj result={
    .stats=fileStats
  };
  
  return result;
}


#endif
