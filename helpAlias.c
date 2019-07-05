#include <stdio.h>
#include "helper.h"

void helpAlias() {
	printf("alias: alias [-p] [name[=value] ... ]\n\
    Define or display aliases.\n\
	\n\
    Without arguments, `alias' prints the list of aliases in the reusable\n\
    form `alias NAME=VALUE' on standard output.\n\
	\n\
    Otherwise, an alias is defined for each NAME whose VALUE is given.\n\
    A trailing space in VALUE causes the next word to be checked for\n\
    alias substitution when the alias is expanded.\n\
	\n\
    Options:\n\
      -p        print all defined aliases in a reusable format\n\
	\n\
    Exit Status:\n\
    alias returns true unless a NAME is supplied for which no alias has been\n\
    defined.\n");
}
