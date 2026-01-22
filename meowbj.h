#ifndef MEOWBJ_H
#define MEOWBJ_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct{
  float x,y,z;
}meowVec3;

typedef struct{
  float u,v;
}meowVec2;

typedef struct{
  size_t vert,norm,tex,faces;
}meowStats;

char *mobj_loadFile(char path[])
{
  FILE *filePointer=fopen(path, "rb");

  if( filePointer == NULL){
    printf("ERROR \t PATH:\n'%s' does not exist!\n",path );
    return NULL;
  }

  fseek(filePointer, 0L, SEEK_END);
  size_t fileSize=ftell(filePointer);
  rewind(filePointer);

  char *fileContents=malloc((fileSize+1)*sizeof(char));
  fread(fileContents, fileSize, 1,filePointer);
  fclose(filePointer);
  return fileContents;
}


typedef struct{
  meowVec3 *verts;
  meowVec3 *norms;
  meowVec2 *uvs;
  meowStats stats;
}meowObj;

void freeObj(meowObj *obj){
  
  free(obj->verts);
  free(obj->norms);
  free(obj->uvs);
}


meowStats meowGetFileStats(char *file)
{
  size_t vertexCount=0;
  size_t normalCount=0;
  size_t texCount=0;
  size_t faceCount=0;
 
  char *lineStart=malloc(strlen(file)*sizeof(char));
  strcpy(lineStart,file);
  while(lineStart!=NULL){
    char *nextLine=strchr(lineStart, '\n');
    if(nextLine) *nextLine='\0';
    //printf("lineStarts at=[%s]\n",lineStart);
    char memberKind[3];
    strncpy(memberKind, currentLine, 2);
    if (strcmp(memberKind,"v ")==0){
      vertexCount++;
    }
    else if (strcmp(memberKind,"vn")==0){
	normalCount++;
      }
    else if (strcmp(memberKind,"vt")==0){
      texCount++;
      }
    else if (strcmp(memberKind,"f ")==0){
      faceCount++;
      }
    
    if(nextLine) *nextLine='\n';
    lineStart=nextLine ? (nextLine+1) : NULL;
  }
  free(lineStart);
  meowStats result={
    .vert=vertexCount,
    .norm=normalCount,
    .tex=texCount,
    .faces=faceCount
  };
  
  return result;
}



meowVec3 *meowstov3(char floatString[])
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
  //printf("   parsed :");
  while(element!=NULL){
    //   "1\0 2\0 3\0"
    char *secondSlashPos=NULL;
    char *firstSlashPos=strchr(element,'/');
    if(firstSlashPos!=NULL){
      *firstSlashPos='\0';
      secondSlashPos=strchr(firstSlashPos+1,'/');
      if(secondSlashPos!=NULL){
	*secondSlashPos='\0';
      } else secondSlashPos=firstSlashPos;
    }
    else firstSlashPos=strchr(element,'\0')-1;

    
    //printf("%s,%s,%s", element,firstSlashPos+1,secondSlashPos+1);

    size_t  vertID,normalID,textureID;
    vertID=    strtol(element,            NULL, 10)-1;
    saveTo->verts[*counter] =loadedData.verts[vertID];

    if((loadedData.stats.uvs>0) && (strcmp(firstSlashPos+1,"")!=0)){
    textureID= strtol(firstSlashPos+1,    NULL, 10)-1;
    saveTo->uvs  [*counter]   =loadedData.uvs[textureID];
    
    }
    
    if((loadedData.stats.normals>0) && (strcmp(secondSlashPos+1,"")!=0)){
    normalID=  strtol(secondSlashPos+1,   NULL, 10)-1;
    saveTo->norms[*counter] =loadedData.norms[normalID];
    }
    
    //printf(" faceid: %zu \nParsed: \t v:%zu  t:%zu  n:%zu", *counter,vertID,textureID,normalID);

    
    *counter=*counter+1;
    //printf("\n");
    element= strtok(NULL," ");
  }
  //printf("\n");
}

int loadObj(char path[],mobj_obj *resulting)
{
  char *fileContents=mobj_loadFile(path);
  if(fileContents==NULL){
    return -1;
  }
  mobj_filestats fileStats=mobj_getFileStats(fileContents);
  mobj_vec3 vertexStore[fileStats.verts];
  mobj_vec3 normalStore[fileStats.normals];
  mobj_vec2 uvStore[fileStats.uvs];

  
  size_t vertexCount=0;
  size_t normalCount=0;
  size_t uvCount=0;
  size_t faceCount=0;

  size_t loadedFaces=fileStats.faces;
  //faceCount=fileStats.faces;
  
  char *currentLine=fileContents;
  size_t counter=0;
  mobj_obj parsedPreAssembler={0};
  mobj_obj saveObj={0};


  saveObj.verts= malloc(loadedFaces*3*sizeof( mobj_vec3 ));
  saveObj.norms= malloc(loadedFaces*3*sizeof( mobj_vec3 ));
  saveObj.uvs=   malloc(loadedFaces*3*sizeof( mobj_vec2 ));
  
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
    }
    else if (strcmp(lineattrib,"f ")==0){
      if (faceCount==0){
	//printf("saving all stores");
	parsedPreAssembler.verts=vertexStore;
	parsedPreAssembler.norms=normalStore;
	parsedPreAssembler.uvs=uvStore;
	parsedPreAssembler.stats=fileStats;
      }

      
      //printf(" * |%s| \n",actuallyCurrentLine+2);
      mobj_objAssembler(actuallyCurrentLine+2, parsedPreAssembler, &saveObj, &counter);
      faceCount++;
      }
    

    //printf("\n");
    if(nextLine) *nextLine='\n';
    currentLine=nextLine ? (nextLine+1) : NULL;
  }
  
 
  free(fileContents);
  
  saveObj.stats=fileStats;
  
  *resulting=saveObj;
  return 0;
}


#endif
