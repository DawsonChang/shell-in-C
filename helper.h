#include <stdbool.h>
//from printf.c
void functionPrintf(int argc, char* argv[], char lineForPrintf[]);
//from aliasRead.c
char* checkAlias(char* result, int init);
//from aliasRead.c
void getPathFromFile(char dir[]);
//from helpCd.c
void helpCd();
//from helpAlias.c
void helpAlias();
//from aliasWrite.c
int checkAliasSyntax(char line[]);
//from aliasWrite.c
bool checkAliasExist(char nameOri[]);
//from aliasWrite.c
void updateAliasFile(char line[]);
