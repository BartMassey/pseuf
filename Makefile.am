# Copyright © 2009 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "3-clause ('new') BSD License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.

BACKEND = markdown.o latex.o symbols.o

pseuf_OBJS = pseuf.o wordtab.o $(BACKEND)

bin_PROGRAMS = pseuf

pseuf_SOURCES = latex.c markdown.c pseuf.h pseuf.l symbols.c \
                symbols.h wordtab.c

pseuf.c: pseuf.l
	flex -o pseuf.c pseuf.l

$(OBJS): pseuf.h

$(BACKEND): symbols.h

clean-local:
	-rm -f pseuf.c
	-rm -f qsort.mdwn qsort.tex
	-rm -f qsort.aux wrapper.dvi wrapper.aux wrapper.log wrapper.pdf
	-rm -rf auto
