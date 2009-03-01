/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pseuf.h"
#include "symbols.h"


static void
op_init(void) {
    int i;
    for (i = 0; symbols[i].from != 0; i++) {
        word_t *e = malloc(sizeof *e);
        assert(e);
        e->word = symbols[i].from;
        e->data = &symbols[i];
        wordtab_insert(idents, e);
    }
}

static void
op_begin(void) {
    fprintf(outfile, "%% This pseudocode translated from %s by pseuf\n",
            filename);
    fprintf(outfile, "\\begin{tabbing}\n");
}

static void
op_end(void) {
    fprintf(outfile, "\\end{tabbing}\n");
    fprintf(outfile, "%% End of pseuf translation of %s\n", filename);
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

static int
ident_special(void)
{
    word_t *w = wordtab_search(idents, strval);
    if (w && w->data) {
        symbol_t *s = w->data;
        if (s->latex)
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
    {"=/=", "$\\ne$"},
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

static void
op_specialchar(void)
{
    abort();
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
    {T_SPECIALCHAR, op_specialchar},
    {0, 0}
};

output_t output_latex = {
    .name = "latex",
    .extension = ".tex",
    .init = op_init,
    .begin = op_begin,
    .end = op_end,
    .bol = 0,
    .eol = op_eol,
    .xlate = xlate
};
