# Copyright (C) 2007 Bart Massey
# ALL RIGHTS RESERVED
# 
# [This program is licensed under the GPL version 3 or later.]
# Please see the file COPYING in the source
# distribution of this software for license terms.

OBJS = pseuf.o markdown.o wordtab.o

CFLAGS = -g -Wall

pseuf: $(OBJS)
	$(CC) $(CFLAGS) -o pseuf $(OBJS)

pseuf.c: pseuf.l
	flex -o pseuf.c pseuf.l

$(OBJS): pseuf.h

clean:
	-rm -f $(OBJS) pseuf pseuf.c
