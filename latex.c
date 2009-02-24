/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <stdio.h>
#include <string.h>
#include "pseuf.h"
#include "greek.h"


static void
op_init(void) {
    int i;
    for (i = 0; greek_idents[i].from != 0; i++) {
        word_t entry;
        entry.word = greek_idents[i].from;
        entry.data = &greek_idents[i];
        wordtab_insert(idents, &entry);
    }
}

static void
op_begin(void) {
    fprintf(outfile, "\\begin{tabbing}\n");
}

static void
op_end(void) {
    fprintf(outfile, "\\end{tabbing}\n");
}

static void
op_eol(void)
{
    fprintf(outfile, "\\\\");
}

static void
op_indent(void)
{
    int i;
    fprintf(outfile, "$");
    for (i = 0; i < intval; i++)
	fprintf(outfile, "~");
    fprintf(outfile, "$");
}

/* XXX needs to use a symbol table */
static int
ident_special(void)
{
    word_t *w = wordtab_search(idents, strval);
    if (w && w->data) {
        struct greek_map *greek = w->data;
        if (greek->latex)
            return 0;
        fprintf(outfile, "$\\%s$", w->word);
        return 1;
    }
    return 0;
}

static void
op_ident(void)
{
    if (ident_special())
        return;
    fprintf(outfile, "{\\em %s}", strval);
}

static void
op_keyword(void)
{
    fprintf(outfile, "{\\bf %s}", strval);
}

static void
op_stuff(void)
{
    fprintf(outfile, "%s", strval);
}


static struct {
    char *from, *to;
} specials[] = {
    {"<-", "$\\leftarrow$"},
    {"->", "$\\rightarrow$"},
    {"<=", "$\\le$"},
    {">=", "$\\ge$"},
    {">", "$>$"},
    {"<", "$<$"},
    {"&", "\\&"},
    {0, 0}
};

static int
op_special(void)
{
    int i;
    for (i = 0; specials[i].from != 0; i++) {
	if (!strcmp(strval, specials[i].from)) {
	    fprintf(outfile, "%s", specials[i].to);
	    return 1;
	}
    }
    return 0;
}

static void
op_op(void)
{
    if (op_special())
	return;
    fprintf(outfile, "$%s$", strval);
}

static void
op_newline(void)
{
    fprintf(outfile, "\n");
}

static void
op_white(void)
{
    fprintf(outfile, " ");
}

static xlate_t xlate[] = {
    {T_INDENT, op_indent},
    {T_IDENT, op_ident},
    {T_KEYWORD, op_keyword},
    {T_WORD, op_stuff},
    {T_LP, op_stuff},
    {T_RP, op_stuff},
    {T_NEWLINE, op_newline},
    {T_STUFF, op_op},
    {T_STRING, op_stuff},
    {T_WHITE, op_white},
    {0, 0}
};

output_t output_latex = {
    .extension = ".tex",
    .init = op_init,
    .begin = op_begin,
    .end = op_end,
    .bol = 0,
    .eol = op_eol,
    .xlate = xlate
};
