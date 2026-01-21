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
  while(currentLine != NULL){
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

mobj_vec3 mobj_strtovec3(char floatString[])
{
  char *element;
  printf("\t%s",floatString);
  element= strtok(floatString, " ");
  int counter= 0;
  mobj_vec3 storedValue={0};

  while(element!= NULL){
    //printf("\t%d:%s\n",counter,elem);
    switch(counter){
    case 0:
      storedValue.x=atof(element);
      break;
    case 1:
      storedValue.y=atof(element);
      break;
    default:
      storedValue.z=atof(element);
    }
    if(counter<3) counter=counter+1;
    element=strtok(NULL, " ");
  }
  
  return storedValue;
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
  while(currentLine != NULL ){
    
    char *nextLine=strchr(currentLine, '\n');
    char lineattrib[3];
    strncpy(lineattrib,currentLine,2);
    printf("[%s]",lineattrib);
    char actuallyCurrentLine[40];
    
    strncpy(actuallyCurrentLine,currentLine, nextLine-currentLine);
    if (strcmp(lineattrib,"v ")==0){
      vertexStore[vertexCount]=mobj_strtovec3(actuallyCurrentLine+2); //+2 just to skip "v " @start
      vertexCount++;
    }
    else if (strcmp(lineattrib,"vn")==0){
      normalStore[normalCount]=mobj_strtovec3(actuallyCurrentLine+3);  //+3 for vn @start
      normalCount++;
    }
    else if (strcmp(lineattrib,"vt")==0){  //TODO: UV inport
      uvCount++;
      }
    else if (strcmp(lineattrib,"f ")==0){
      faceCount++;
      }

    printf("\n");
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
