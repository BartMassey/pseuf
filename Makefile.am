# Copyright © 2009 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "3-clause ('new') BSD License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.

AM_CFLAGS = -g -Wall -Wno-unused-function

BACKEND = markdown.o latex.o utf8.o symbols.o specialchars.o

pseuf_OBJS = pseuf.o wordtab.o $(BACKEND)

bin_PROGRAMS = pseuf

pseuf_SOURCES = markdown.c latex.c utf8.c pseuf.h pseuf.l symbols.c \
                symbols.h wordtab.c specialchars.c specialchars.h

pseuf.c: pseuf.l
	flex -o pseuf.c pseuf.l

$(OBJS): pseuf.h

$(BACKEND): symbols.h

clean-local:
	-rm -f pseuf.c
	-rm -rf auto
