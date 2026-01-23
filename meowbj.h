#ifndef MEOWBJ_H
#define MEOWBJ_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
	float x;
	float y;
	float z;
}meowVec3;

typedef struct{
	float u;
	float v;
}meowVec2;

typedef struct{
	size_t vert;
	size_t norm;
	size_t texc;
	size_t faces;
}meowStats;

char *meowLoadFile(char path[])
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
	meowVec3 *vert;
	meowVec3 *norm;
	meowVec2 *texc;
	meowStats stats;
}meowObj;

void freeObj(meowObj *obj){
	free(obj->vert);
	free(obj->norm);
	free(obj->texc);
	free(obj);
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
			char memberKind[3];

		strncpy(memberKind, lineStart, 2);
		if (strcmp(memberKind,"v ")==0)
			vertexCount++;
		else if (strcmp(memberKind,"vn")==0)
			normalCount++;
		else if (strcmp(memberKind,"vt")==0)
			texCount++;
		else if (strcmp(memberKind,"f ")==0)
			faceCount++;

		if(nextLine) 
			*nextLine='\n';
		lineStart=nextLine ? (nextLine+1) : NULL;
	}

	free(lineStart);
	meowStats result={
		.vert=vertexCount,
		.norm=normalCount,
		.texc=texCount,
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
	meowVec3 *storedValue=malloc(sizeof(meowVec3));

	while(element!= NULL){
		//printf("\t%d:%s\n",counter,element);
		if(counter==0)
			storedValue->x=atof(element);
		else if (counter==1)
			storedValue->y=atof(element);
		else
			storedValue->z=atof(element);

		if(counter<3) 
			counter=counter+1;
		element=strtok(NULL, " ");
	}

	return storedValue;
}


meowVec2 *meowstov2(char floatString[])
{
	char *element;
	element= strtok(floatString, " ");
	int counter= 0;
	meowVec2 *storedValue=malloc(sizeof(meowVec2));

	while(element!= NULL){
		if(counter==0)
			storedValue->u=atof(element);
		else
			storedValue->v=atof(element);

		counter=(counter<2) ? counter+1 : 2;
		element=strtok(NULL, " ");
	}

	return storedValue;
}


void _meowRearrange(char faceString[],meowObj loadedData, meowObj *saveTo, size_t *counter)
{
	char *element;
	element= strtok(faceString, " ");  
	while(element!=NULL){
		char *secondSlashPos=NULL;
		char *firstSlashPos=strchr(element,'/');

		if(firstSlashPos!=NULL){
			*firstSlashPos='\0';
			secondSlashPos=(firstSlashPos!=NULL) ? strchr(firstSlashPos+1,'/') : NULL;
			if(secondSlashPos!=NULL)
				*secondSlashPos='\0';
		}	
		else firstSlashPos=strchr(element,'\0')-1;

		size_t  vertID;
		size_t	normalID;
		size_t	textureID;

		vertID=strtol(element,NULL, 10)-1;
		saveTo->vert[*counter]=loadedData.vert[vertID];

		if((loadedData.stats.texc>0) && (strcmp(firstSlashPos+1,"")!=0)){
			textureID= strtol(firstSlashPos+1,    NULL, 10)-1;
			saveTo->texc[*counter]=loadedData.texc[textureID];
		}

		if((loadedData.stats.norm>0) && (strcmp(secondSlashPos+1,"")!=0)){
			normalID=  strtol(secondSlashPos+1,   NULL, 10)-1;
			saveTo->norm[*counter]=loadedData.norm[normalID];
		}

		*counter=*counter+1;
		element= strtok(NULL," ");
	}
}

void _meowSaveVec3(char line[],size_t *index, meowVec3 *array)
{
	meowVec3 *parsedVec3= meowstov3(line);
	memcpy(&array[*index],parsedVec3,sizeof(meowVec3));
	free(parsedVec3);
	(*index)++;
}


void _meowSaveVec2(char line[],size_t *index, meowVec2 *array)
{
	meowVec2 *parsedVec2= meowstov2(line);
	memcpy(&array[*index],parsedVec2,sizeof(meowVec2));
	free(parsedVec2);
	(*index)++;
}

meowObj *loadObj(char path[])
{
	char *fileContents=meowLoadFile(path);
	if(fileContents==NULL)
		return NULL;

	meowStats fileStats=meowGetFileStats(fileContents);
	meowVec3 vertexStore[fileStats.vert];
	meowVec3 normalStore[fileStats.norm];
	meowVec2 uvStore[fileStats.texc];


	size_t vertexCount=0;
	size_t normalCount=0;
	size_t uvCount=0;
	size_t faceCount=0;

	size_t loadedFaces=fileStats.faces;
	char *startOfLine=fileContents;

	size_t counter=0;
	meowObj parsedPreAssembler={0};
	meowObj *saveObj=malloc(sizeof(meowObj));


	saveObj->vert= malloc(loadedFaces*3*sizeof( meowVec3 ));
	saveObj->norm= malloc(loadedFaces*3*sizeof( meowVec3 ));
	saveObj->texc= malloc(loadedFaces*3*sizeof( meowVec2 ));
	
	while(startOfLine != NULL ){

		char *nextLine=strchr(startOfLine, '\n');
		if(nextLine) 
			*nextLine ='\0';

		char lineattrib[3];
		strncpy(lineattrib,startOfLine,2);
		char thisLine[40];
		strcpy(thisLine,startOfLine);

		if (strcmp(lineattrib,"v ")==0)
			_meowSaveVec3(thisLine+2,&vertexCount,vertexStore); //+2 to skip "v " @ start of line
		else if (strcmp(lineattrib,"vn")==0)
			_meowSaveVec3(thisLine+3,&normalCount,normalStore); //+3 to skip "vn " @ start of line	
		else if (strcmp(lineattrib,"vt")==0) 
			_meowSaveVec2(thisLine+3,&uvCount,uvStore);   // +3 to skip vt @start

		else if (strcmp(lineattrib,"f ")==0){
		if (faceCount==0){
			parsedPreAssembler.vert=vertexStore;
			parsedPreAssembler.norm=normalStore;
			parsedPreAssembler.texc=uvStore;
			parsedPreAssembler.stats=fileStats;
		}


		//printf(" * |%s| \n",thisLine+2);
		_meowRearrange(thisLine+2, parsedPreAssembler, saveObj, &counter);
		faceCount++;
		}


		//printf("\n");
		if(nextLine) *nextLine='\n';
		startOfLine=nextLine ? (nextLine+1) : NULL;
	}


	fileStats.vert=faceCount*3;
	fileStats.norm=faceCount*3;
	fileStats.texc=faceCount*3;
	fileStats.faces=faceCount;
	saveObj->stats=fileStats;
	//resulting=saveObj;
	free(fileContents);
	return saveObj;
}


#endif
