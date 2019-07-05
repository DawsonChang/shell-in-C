#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "helper.h"

//read file /tmp/dirName.txt to get the path of alias file
void getPathFromFile(char dir[]) {
	FILE *fdir;
	fdir = fopen("/tmp/dirName.txt", "r");
	if (!fdir) {
		printf("fail to open the file /tmp/dirName.txt\n");
		return;
	}
	char c;
	int i = 0;
	c = fgetc(fdir);
	while(c != EOF){
	
		dir[i] = c;
		c = fgetc(fdir);
		i++;
	}
	dir[i] = '\0';
	fclose(fdir);
}

char* checkAlias(char* result, int init){
	//initialize when the shell starts
	//get current path name for reading "alias.txt"
	if(init == 0){
		char* dir = get_current_dir_name();
		FILE *fw;
		//save the path in "dirName.txt" in /tmp
		fw = fopen("/tmp/dirName.txt", "w");
		if (!fw) {
			printf("fail to open the file /tmp/dirName.txt\n");
			return result;
		}
		fprintf(fw, "%s", dir);
		fclose(fw);
	}
	//get the path by function getPathFromFile()
	char dir[1024];
	getPathFromFile(dir);
	
	//put the path with alias.txt in a variable
	char dirForRead[1024];
	snprintf(dirForRead, sizeof(dirForRead), "%s/alias.txt", dir);
	
	FILE *fp;
	//read the variable of absolute path for "alias.txt"
	fp = fopen(dirForRead, "r");
	if (!fp) {
		printf("fail to open the file %s\n", dirForRead);
		return result;
	}
	char line[1024] = {0};
	char sudoCommand[1024] = {0};
	char realCommand[1024] = {0};
	int realCommandStartIndex = 0;
	int realCommandIndex = 0;
	int i = 0;
	char c;
	//sudoCommand means the name before '='
	//realCommand means the command after '='
	while(1) {
		c = fgetc(fp);
		if (c == '\n' || c == EOF){
			for(int j = 0; j < strlen(line); j++){
				if (line[j] != '='){
					sudoCommand[j] = line[j];
				}
				else{
					realCommandStartIndex = j+1;
					break;
				}
			}
			for(int j = realCommandStartIndex+1; j < strlen(line)-1; j++){
				realCommand[realCommandIndex] = line[j];
				realCommandIndex++;
			}
			//if it is not the last line, the ' on the last index of realCommand should be deleted
			if (c != EOF) {
				if (realCommand[strlen(realCommand)-1] == '\'' || realCommand[strlen(realCommand)-1] == '\"')
					realCommand[strlen(realCommand)-1] = '\0';
				else
					realCommand[strlen(realCommand)] = '\0';
			}
			//if we find sudoCommand is in the alias, prepare to return realCommand
			if(strcmp(sudoCommand, result) == 0){
				result = realCommand;
				break;
			}
			memset(line, 0, sizeof(line));
			memset(sudoCommand, 0, sizeof(sudoCommand));
			memset(realCommand, 0, sizeof(realCommand));
			i = 0;
			realCommandIndex = 0;
		}
		//in the same line
		else{
			line[i] = c;
			i++;
		}
		if(c == EOF)
			break;
	}
	fclose(fp);
	return result;
	
}
