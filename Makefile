# Copyright (C) 2007 Bart Massey
# ALL RIGHTS RESERVED
# 
# [This program is licensed under the GPL version 3 or later.]
# Please see the file COPYING in the source
# distribution of this software for license terms.

CFLAGS = -g -Wall

pseuf: pseuf.o markdown.o
	$(CC) $(CFLAGS) -o pseuf pseuf.o markdown.o

pseuf.c: pseuf.l
	flex -o pseuf.c pseuf.l

pseuf.o markdown.o: pseuf.h

clean:
	-rm -f markdown.o pseuf.o pseuf pseuf.c
