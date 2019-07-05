#include <stdio.h>
#include "helper.h"

void helpCd(){
	printf("cd: cd [-L|[-P [-e]] [-@]] [dir]\n \
    Change the shell working directory.\n \
	\n\
    Change the current directory to DIR.  The default DIR is the value of the\n \
    HOME shell variable.\n \
	\n\
    The variable CDPATH defines the search path for the directory containing\n \
    DIR.  Alternative directory names in CDPATH are separated by a colon (:).\n \
    A null directory name is the same as the current directory.  If DIR begins\n \
    with a slash (/), then CDPATH is not used.\n \
	\n\
    If the directory is not found, and the shell option `cdable_vars' is set,\n \
    the word is assumed to be  a variable name.  If that variable has a value,\n \
    its value is used for DIR.\n \
	\n\
    Options:\n \
      -L        force symbolic links to be followed: resolve symbolic\n \
                links in DIR after processing instances of `..'\n \
      -P        use the physical directory structure without following\n \
                symbolic links: resolve symbolic links in DIR before\n \
                processing instances of `..'\n \
      -e        if the -P option is supplied, and the current working\n \
                directory cannot be determined successfully, exit with\n \
                a non-zero status\n \
      -@        on systems that support it, present a file with extended\n \
                attributes as a directory containing the file attributes\n \
	\n\
    The default is to follow symbolic links, as if `-L' were specified.\n \
    `..' is processed by removing the immediately previous pathname component\n \
    back to a slash or the beginning of DIR.\n \
	\n\
    Exit Status:\n \
    Returns 0 if the directory is changed, and if $PWD is set successfully when\n \
    -P is used; non-zero otherwise.\n");
}
