edit:mysh clean
mysh:mysh.o printf.o aliasRead.o aliasWrite.o helpCd.o helpAlias.o
	gcc mysh.o printf.o aliasRead.o aliasWrite.o helpCd.o helpAlias.o -o mysh
mysh.o:mysh.c helper.h
	gcc -c mysh.c
printf.o:printf.c helper.h
	gcc -c printf.c
aliasRead.o:aliasRead.c helper.h
	gcc -c aliasRead.c
aliasWrite.o:aliasWrite.c helper.h
	gcc -c aliasWrite.c
helpCd.o:helpCd.c helper.h
	gcc -c helpCd.c
helpAlias.o:helpAlias.c helper.h
	gcc -c helpAlias.c
clean:
	rm -f *.o
