#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "helper.h"

char checkEscapeC(char c){
	if(c == 'n')
		return '\n';
	
	else if(c == 'r')
		return '\r';
	
	else if(c == 't')
		return '\t';
	
	else if(c == 'b')
		return '\b';
	
	else if(c == 'f')
		return '\f';

	else if(c == '\\')
		return '\\';
	else
		return c;
	
}

int snprintfFormat(char temp[], char c, char* argv){
	char formated_temp[256] = {0};
	if(c == 'd' || c == 'i'){
		snprintf(formated_temp, sizeof(formated_temp), temp, atoi(argv));
		printf("%s", formated_temp);
		return 0;
	}
	else if(c == 'f' || c == 'F'){
		snprintf(formated_temp, sizeof(formated_temp), temp, strtod(argv,NULL));
		printf("%s", formated_temp);
		return 0;
	}
	else if(c == 'c'){
		snprintf(formated_temp, sizeof(formated_temp), temp, argv[0]);
		printf("%s", formated_temp);
		return 0;
	}
	else if(c == 's'){
		snprintf(formated_temp, sizeof(formated_temp), temp, argv);
		printf("%s", formated_temp);
		return 0;
	}

	return 1;
}

void functionPrintf(int argc, char* argv[], char lineForPrintf[]){
	int formatPair = 0;
	char format[1024] = {0};
	
	if (argc == 1)
		printf("printf: usage: printf format [arguments]\n");
	
	else{
		//if the first character is not double quotation mark
		if (lineForPrintf[0] != '\"'){
			printf("printf: usage: need \" in first of format\n");
			return;
		}
		
		//calculate how many format pairs are in argument such as "%d","%s", except "%%"
		for (int j = 0; j < strlen(lineForPrintf); j++){
			if (lineForPrintf[j] == '%'){
				if(lineForPrintf[j+1] == '%')
					j++;
				else
					formatPair++;
			}
		}
		
		//put the format form together in array "format"
		for (int j = 1; j < argc-formatPair; j++){
			//printf("%s\n", argv[j]);
			strcat(format,argv[j]);
			if (j < argc-formatPair-1) {
				strcat(format, " ");
			}
		}
		
		//check the last character in "format" is double quotation mark
		if(format[strlen(format)-1] != '\"'){
			if (argc == 2)
				printf("printf: usage: need argument after format\n");
			else if (argc > 2)
				printf("printf: usage: error in format\n");
			return;
		}
		int formatIndex = 1;
		int tempIndex = 0;
		int k = 0;
		int isFormat = 0;
		char temp[256] = {0};
		char formated_temp[256] = {0};
		int check = 0;
		
		//traversal the array "format"
		while(formatIndex < strlen(format)-1){
		
			//handle the escape characters
			if (format[formatIndex] == '\\'){
				char escape = checkEscapeC(format[formatIndex + 1]);
				printf("%c", escape);
				formatIndex+=2;
			}
			else{
				//handle the formating output
				if (format[formatIndex] == '%')
					isFormat = 1;
				
				if (isFormat == 1){
					temp[tempIndex] = format[formatIndex];
					tempIndex++;	
				}
				//if it is a normal character, just print it
				else
					printf("%c", format[formatIndex]);
				
				//if get the first alphabet, change it to formating output
				if(isFormat == 1 && (isalpha(format[formatIndex]))){
					//put formating output in "temp"
					check = snprintfFormat(temp, format[formatIndex], argv[argc-formatPair+k]);
					k++;
					memset(temp, 0, sizeof(temp));
					memset(formated_temp, 0, sizeof(formated_temp));
					tempIndex = 0;
					isFormat = 0;
					
					if(check == 1){
						printf("printf: usage: error in format\n");
						return;
					}
				}
				//if get "%%" 
				else if (isFormat == 1 && format[formatIndex+1] == '%'){
					printf("%%");
					memset(temp, 0, sizeof(temp));
					tempIndex = 0;
					formatIndex++;
					isFormat = 0;
				}
				//if get other character after '%'
				else if ((isFormat == 1 && format[formatIndex+1] != '%') && format[formatIndex] != '%' && !(isalpha(format[formatIndex]))){
					printf("printf: usage: error in format\n");
					return;
				}
				/*else{
					printf("printf: usage: error in format\n");
					return;
				}*/
				formatIndex++;
			}
		}
			
	}
}
