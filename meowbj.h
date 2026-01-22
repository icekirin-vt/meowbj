#ifndef MEOWBJ_H
#define MEOWBJ_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

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
  fclose(filePointer);
  return fileContents;
}


typedef struct{
  mobj_vec3 *verts;
  mobj_vec3 *norms;
  mobj_vec2 *uvs;
  mobj_filestats stats;
}mobj_obj;

void freeObj(mobj_obj *objToFree){
  
  free(objToFree->verts);
  free(objToFree->norms);
  free(objToFree->uvs);
}


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



mobj_vec3 *mobj_strtovec3(char floatString[])
{
  char *element;
  //printf("\t%s",floatString);
  element= strtok(floatString, " ");
  int counter= 0;
  mobj_vec3 *storedValue=malloc(sizeof(mobj_vec3));;

  while(element!= NULL){
    //printf("\t%d:%s\n",counter,element);
    if(counter==0)
      storedValue->x=atof(element);
    else if (counter==1)
      storedValue->y=atof(element);
    else
      storedValue->z=atof(element);

    if(counter<3) counter=counter+1;
    element=strtok(NULL, " ");
  }
  
  return storedValue;
}
mobj_vec2 mobj_strtovec2(char floatString[])
{
  char *element;
  //printf("\t%s",floatString);
  element= strtok(floatString, " ");
  int counter= 0;
  mobj_vec2 storedValue={0};

  while(element!= NULL){
    //printf("\t%d:%s\n",counter,elem);
    if(counter==0)
      storedValue.u=atof(element);
    else
      storedValue.v=atof(element);

    if(counter<2) counter=counter+1;
    element=strtok(NULL, " ");
  }
  
  return storedValue;
}


void mobj_objAssembler(char faceString[],mobj_obj loadedData, mobj_obj *saveTo, size_t *counter)
{
  char *element;
  
  element= strtok(faceString, " ");
  size_t facesCount=loadedData.stats.faces;
  



  //mobj_obj result={0};
  mobj_filestats finalStat= { .verts=facesCount*3,
    .normals=facesCount*3,
    .uvs=facesCount*3,
    .faces=facesCount
  };
  
  //printf("   parsed :");
  
  while(element!=NULL){
    //   "1\0 2\0 3\0"
    char *firstSlashPos=strchr(element,'/');
    *firstSlashPos='\0';
    char *secondSlashPos=strchr(firstSlashPos+1,'/');
    *secondSlashPos='\0';
    //printf("%s,%s,%s", element,firstSlashPos+1,secondSlashPos+1);

    size_t  vertID,normalID,textureID;
    
    vertID=    strtol(element,            NULL, 10)-1;
    textureID= strtol(firstSlashPos+1,    NULL, 10);
    normalID=  strtol(secondSlashPos+1,   NULL, 10);

    
    //printf(" faceid: %zu \nParsed: \t v:%zu  t:%zu  n:%zu", *counter,vertID,textureID,normalID);

   
    saveTo->verts[*counter] =loadedData.verts[vertID];
    saveTo->norms[*counter] =loadedData.norms[normalID];
    saveTo->uvs  [*counter]   =loadedData.uvs[textureID];
    
    
    *counter=*counter+1;
    //printf("\n");
    element= strtok(NULL," ");
  }
  //printf("\n");
}

mobj_obj loadObj(char path[])
{
  char *fileContents=mobj_loadFile(path);
  mobj_filestats fileStats=mobj_getFileStats(fileContents);

  mobj_vec3 zeroVec3={.x=0.0,.y=0.0, .z=0.0};
  mobj_vec2 zeroVec2={.u=0.0, .v=0.0};
  mobj_vec3 vertexStore[fileStats.verts];
  mobj_vec3 normalStore[fileStats.normals];
  mobj_vec2 uvStore[fileStats.uvs];

  for(size_t i=0;i<fileStats.verts;i++){
    vertexStore[i]= zeroVec3;
    if(i<fileStats.normals)
      normalStore[i]= zeroVec3;
 }
  for(size_t i=0;i<fileStats.uvs;i++)
    uvStore[i]= zeroVec2;
  
  size_t vertexCount=0;
  size_t normalCount=0;
  size_t uvCount=0;
  size_t faceCount=0;

  faceCount=fileStats.faces;
  
  char *currentLine=fileContents;
  size_t counter=0;
  mobj_obj parsedPreAssembler={0};
  mobj_obj saveObj={0};


  saveObj.verts= malloc(faceCount*3*sizeof( mobj_vec3 ));
  saveObj.norms= malloc(faceCount*3*sizeof( mobj_vec3 ));
  saveObj.uvs=   malloc(faceCount*3*sizeof( mobj_vec2 ));
  
  while(currentLine != NULL ){
    
    char *nextLine=strchr(currentLine, '\n');
    if(nextLine) *nextLine ='\0';
    char lineattrib[3];
    strncpy(lineattrib,currentLine,2);
    //printf("[%s]",lineattrib);
    char actuallyCurrentLine[40];
    
    strcpy(actuallyCurrentLine,currentLine);
    if (strcmp(lineattrib,"v ")==0){
      //vertexStore[vertexCount]
      mobj_vec3 *parsedVec3=mobj_strtovec3(actuallyCurrentLine+2); //+2 just to skip "v " @start
      memcpy(&vertexStore[vertexCount],parsedVec3, sizeof(mobj_vec3));
      free(parsedVec3);
      vertexCount++;
    }
    else if (strcmp(lineattrib,"vn")==0){
      mobj_vec3 *parsedVec3=mobj_strtovec3(actuallyCurrentLine+3);  //+3 for vn @start
      memcpy(&normalStore[normalCount],parsedVec3,sizeof(mobj_vec3));
      free(parsedVec3);
      normalCount++;
    }
    else if (strcmp(lineattrib,"vt")==0){  // +3 for vt @start
      uvStore[uvCount]=mobj_strtovec2(actuallyCurrentLine+3);
      uvCount++;
      if (uvCount==fileStats.uvs){
	parsedPreAssembler.verts=vertexStore;
	parsedPreAssembler.norms=normalStore;
	parsedPreAssembler.uvs=uvStore;
	parsedPreAssembler.stats=fileStats;
      }
      }
    else if (strcmp(lineattrib,"f ")==0){
      mobj_objAssembler(actuallyCurrentLine+2, parsedPreAssembler, &saveObj, &counter);
      faceCount++;
      }
    

    //printf("\n");
    if(nextLine) *nextLine='\n';
    currentLine=nextLine ? (nextLine+1) : NULL;
  }
  
 
  free(fileContents);
  
  saveObj.stats=fileStats;
  
  return saveObj;
}


#endif
