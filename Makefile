pcode: pcode.o markdown.o
	$(CC) $(CFLAGS) -o pcode pcode.o markdown.o

pcode.c: pcode.l
	flex -o pcode.c pcode.l

pcode.o markdown.o: pcode.h

clean:
	-rm -f markdown.o pcode.o pcode pcode.c
