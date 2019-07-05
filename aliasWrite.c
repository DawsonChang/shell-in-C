#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "helper.h"
//set the index of stack as global variable
int sTop = 0;

void updateAliasFile(char line[]) {
	//get the path by function getPathFromFile() in aliasRead.c
	char dir[1024];
	getPathFromFile(dir);
	
	//put the path with alias.txt in a variable
	char dirForRead[1024];
	snprintf(dirForRead, sizeof(dirForRead), "%s/alias.txt", dir);
	FILE *fp;
	//read the variable of absolute path for "alias.txt"
	fp = fopen(dirForRead, "a+");
	if (!fp) {
		printf("fail to open the file %s\n", dirForRead);
		return;
	}
	//put new command to newline
	fprintf(fp, "\n");
	fprintf(fp, "%s", line);
	fclose(fp);
}

bool checkAliasExist(char nameOri[]) {
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
		return false;
	}
	
	//get the string before '=' save in name[1024]
	char *line = NULL;
	char name[1024];
    size_t len = 0;
    ssize_t read;
	
    while ((read = getline(&line, &len, fp)) != -1) {
		int i = 0;
		for (i = 0; line[i] != '='; i++) {
			name[i] = line[i];
		}
		line[i] = '\0';
		if (strcmp(name, nameOri) == 0) {
			return false;
		}
		line = NULL;
		memset(name, 0, sizeof(name));
    }
	return true;	
}

void PUSH(char checkQuoteStack[], char c) {
	checkQuoteStack[sTop] = c;
	sTop++;
}

void POP(char checkQuoteStack[]) {
	sTop--;
	checkQuoteStack[sTop] = '\0';
}

int checkAliasSyntax(char line[]) {
	//if line is valid, validSyntax = 0
	int validSyntax = 0;
	char checkQuoteStack[1024] = {0};
	char c;
	int i = 0;
	
	//check the string before '=' has symbol below
	do {
		c = line[i];
		if (c == '\'' || c == '\"' || c == '_' || c == '/' || c == '\\' || c == '`' || c == '<' 
		|| c == '>' || c == '(' || c == ')' || c == '|' || c == '$' || c == '#' || c == '&' 
		|| c == '*' || c == ';' || c == ' ') {
			
			validSyntax = 1;
			return validSyntax;
		}
		i++;
	}while(c != '=' && c != EOF);
	//check whether there exists '=' in line[]
	if (c == EOF) {
		validSyntax = 1;
		return validSyntax;	
	}
	//check whether i is the last index of line
	if (i == strlen(line)) {
		validSyntax = 1;
		return validSyntax;
	}
	//check whether the quotations are in pair and on the first and last position of line
	if ((line[i] != '\'' || line[strlen(line)-1] != '\'') && (line[i] != '\"' || line[strlen(line)-1] != '\"')) {
		validSyntax = 1;
		return validSyntax;
	}
	//use a stack to calculate whether the quotation marks is in pairs
	while(i < strlen(line)){
		c = line[i];
		if (c == '\''){
			if (sTop == 0) 
				PUSH(checkQuoteStack, c);
			
			else if(checkQuoteStack[sTop-1] == '\'') 
				POP(checkQuoteStack);
			
			else 
				PUSH(checkQuoteStack, c);
			
		}
		else if (c == '\"'){

			if (sTop == 0) 
				PUSH(checkQuoteStack, c);
			
			else if(checkQuoteStack[sTop-1] == '\"') 
				POP(checkQuoteStack);
			
			else 
				PUSH(checkQuoteStack, c);
			
		}
		i++;
		
	}
	sTop = 0;
	//if stack is empty, the quotation mark is in pairs
	if (checkQuoteStack[0] != '\0') {
		validSyntax = 1;
		return validSyntax;
	}
	return validSyntax;
}
