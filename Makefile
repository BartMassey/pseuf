pseuf: pseuf.o markdown.o
	$(CC) $(CFLAGS) -o pseuf pseuf.o markdown.o

pseuf.c: pseuf.l
	flex -o pseuf.c pseuf.l

pseuf.o markdown.o: pseuf.h

clean:
	-rm -f markdown.o pseuf.o pseuf pseuf.c
