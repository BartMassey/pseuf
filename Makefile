pcode: pcode.o markdown.o
	gcc -o pcode pcode.o markdown.o

pcode.c: pcode.l
	flex -o pcode.c pcode.l

pcode.o markdown.o: pcode.h
