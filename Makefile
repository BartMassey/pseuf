# Copyright (C) 2007 Bart Massey
# ALL RIGHTS RESERVED
# 
# [This program is licensed under the GPL version 3 or later.]
# Please see the file COPYING in the source
# distribution of this software for license terms.

BACKEND = markdown.o latex.o greek.o
OBJS = pseuf.o wordtab.o $(BACKEND)

CFLAGS = -g -Wall

pseuf: $(OBJS)
	$(CC) $(CFLAGS) -o pseuf $(OBJS)

pseuf.c: pseuf.l
	flex -o pseuf.c pseuf.l

$(OBJS): pseuf.h

$(BACKEND): greek.h

clean:
	-rm -f $(OBJS) pseuf pseuf.c
	-rm -f qsort.aux wrapper.dvi wrapper.aux wrapper.log
	-rm -f qsort.mdwn qsort.tex
	-rm -rf auto
