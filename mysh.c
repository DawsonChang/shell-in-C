#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "helper.h"

void functionPrompt(){
	char* username;
	char hostname[1024];
	username = getenv("USER");
	gethostname(hostname, 1024);
	printf("\033[96m[{%s}@{%s}\033[0m\033[93m{%s}]\033[0m $ ", username, hostname, get_current_dir_name());
}

char* functionCd(char* argv[]){
	char *buf = NULL;
	buf = get_current_dir_name();
	buf = strcat(buf, "/");
	buf = strcat(buf, argv[1]);
	return buf;
}

int functionExit(char* argv[]){
	if (argv[1] == NULL)
		return 0;
	else
		return atoi(argv[1]);	
}

int main() {
    char *line = NULL;
    size_t len;
    ssize_t nread;
    int status = 0;
	unsigned int init = 0;

    while (1) {
        // prompt
        /*if (status != 0 && WIFEXITED(status))
            printf("(\033[31m%d\033[0m) ", WEXITSTATUS(status));*/
		
		//display [{username}@{hostname} {pwd}] $ in prompt
		functionPrompt(); //from mysh.c

        // read the line and strip newline
        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;
        if (line[nread-1] == '\n')
            line[nread-1] = '\0';
		
		//if you press enter, the shell will continue instead get error
		if(line[0] == 0)
			continue;

		//check whether the input sudocommand is in alias txt file
		//function input is "line", if the sudocommand match alias.txt, return new command
		//if not, return the original line
		char *newline = NULL;
		newline = checkAlias(line, init); //from alaisRead.c
		//if it is the first loop, init=0
		init++;
		
		//if get empty from alias, continue
		if(newline[0] == 0)
			continue;
		
		//get the command line argument after command(for "printf")
		char lineForPrintf[1024];
		for (int j = 0; j < strlen(newline); j++)
			lineForPrintf[j] = newline[j+7];
		
		//get the command line argument after command(for "alias")
		char lineForAlias[1024];
		for (int j = 0; j < strlen(newline); j++)
			lineForAlias[j] = newline[j+6];
		
		
        // change spaces into zero bytes
        // and initialize argv
		int a = 0;
		int argc = 0;
		while (a < strlen(newline))
		{
			if (strlen(newline) == 1) {
				argc++;
				break;
			}
			char c = newline[a];
			if (c == ' ' && newline[a-1] != ' ')
				argc++;
			a++;
			if (a == strlen(newline)-1 && newline[strlen(newline)-1] != ' ')
				argc++;
		}
        char *argv[100] = { 0 };
        char *p = newline;
        int i = 0;
        while (*p) {
            argv[i] = p;
            ++i;
            while (*p && *p != ' ')
                ++p;
            if (*p == ' ')
                *p = '\0';
            ++p;
        }

		//check if it is the form for environment variable
		int numOfEq = 0;
		char vname[1024] = {0};
		char vvalue[1024] = {0};
		
		if (argc == 1) {	
			for (int j = 0; j < strlen(argv[0]); j++) {
				if (argv[0][j] == '=') {
					numOfEq++;
				}
			}
			int k = 0;
			// split name=value into vname and vvalue for setenv
			while(argv[0][k] != '=' && k < strlen(argv[0])) {
				vname[k] = argv[0][k];
				k++;
			}
			int newIndex = 0;
			vname[k] = '\0';

			k++;
			while(k < strlen(argv[0])) {
				vvalue[newIndex] = argv[0][k];
				k++;
				newIndex++;
			}
			vvalue[newIndex] = '\0';
		}

		if(argc == 1 && numOfEq == 1) {
			//put the argument as environment variable
			//parameter 1 means replace previous environment variable
			setenv(vname, vvalue, 1);
			status = 0;
			continue;
		}
		//if get the name of environment variable
		if(getenv(vname)){
			printf("%s\n", getenv(vname));
			status = 0;
			continue;
		}
		
		//if get command line argument "-V", change it to "--version"(it is executable)
		if(argc > 1){
			if(strcmp(argv[1], "-V") == 0) {
				strcpy(argv[1], "--version");
			}
		}

		//if get command "cd"
		if(strcmp(argv[0], "cd") == 0){
			if(argc > 1 && strcmp(argv[1], "--help") != 0)
				chdir(functionCd(argv)); //form mysh.c
			else if(argc > 1 && strcmp(argv[1], "--help") == 0)
				helpCd(); //from helpCd.c
			else if (argc == 1)
				chdir(getenv("HOME"));
			else
				printf("cd: usage: cd [dir]\n");
			status = 0;
			continue;
		}
		//if get command "exit"
		else if(strcmp(argv[0], "exit") == 0){
			return functionExit(argv); //from mysh.c
		}
		//if get command "help"
		else if(strcmp(argv[0], "help") == 0){
			char* command = argv[1];
			if(argc > 1){
				if(strcmp(argv[1], "cd") == 0)
					helpCd(); //from helpCd.c
				else if(strcmp(argv[1], "pwd") == 0)
					system("/bin/pwd --help");
				else if(strcmp(argv[1], "echo") == 0)
					system("/bin/echo --help");
				else if(strcmp(argv[1], "alias") == 0)
					helpAlias(); //from helpAlias.c			
				else{
					strcat(command, " ");
					strcat(command, "--help");
					system(command);
				}
			}
			else
				system("bash -c help");
			
			status = 0;
			continue;
		}
		
		//if get command "printf"
		else if(strcmp(argv[0], "printf") == 0) {
			functionPrintf(argc, argv, lineForPrintf); //functionPrintf() is from printf.c
			status = 0;
			continue;
		}
		
		else if(strcmp(argv[0], "alias") == 0) {
			if (argc == 1) {
				//get the path by function
				char dir[1024];
				getPathFromFile(dir); //from aliasRead.c

				//put the path with alias.txt in a variable
				char dirForRead[1024];
				snprintf(dirForRead, sizeof(dirForRead), "%s/alias.txt", dir);
				FILE *fp;
				//read the variable of absolute path for "alias.txt"
				fp = fopen(dirForRead, "r");
				char c = fgetc(fp);
				while(c != EOF) {
					printf("%c", c);
					c = fgetc(fp);
				}
				printf("\n");
				fclose(fp);
			}
			if (argc > 1) {
				int validSyntax = checkAliasSyntax(lineForAlias); //from aliasWrite.c
				if (validSyntax == 1) {
					printf("alias: usage: alias [name]=\"[command]\"\n");
				}
				else {
					//get the name before '='
					char nameOri[1024];
					int j = 0;
					for (j = 0; lineForAlias[j] != '='; j++) {
						nameOri[j] = lineForAlias[j];
					}
					nameOri[j] = '\0';
					//check whether the name is stored in alias before
					if (checkAliasExist(nameOri))
						//if the name is not exist before, store it in the file
						updateAliasFile(lineForAlias); //from aliasWrite.c
					else
						printf("-bash: alias: the alias name has existed\n");
				}
			}
			status = 0;
			continue;
		}
			
        if (fork()) {
            // parent
            wait(&status);
        } else {
            //child
            execvp(argv[0], argv);
            printf("%s: command not found\n", argv[0]);
            return 1;
        }
    }
    return 0;
}
