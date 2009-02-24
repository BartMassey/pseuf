/*
 * Copyright (C) 2007 Bart Massey
 * ALL RIGHTS RESERVED
 * 
 * [This program is licensed under the GPL version 3 or later.]
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
        entry.data = greek_idents[i].to;
        wordtab_insert(idents, &entry);
    }
}

static void
op_begin(void)
{
    fprintf(outfile, "\n");
}

static void
op_end(void)
{
    fprintf(outfile, "\n");
}

static void
op_bol(void)
{
    fprintf(outfile, "> ");
}

static void
op_eol(void)
{
    fprintf(outfile, "  ");
}

static void
op_indent(void)
{
    int i;
    for (i = 0; i < intval; i++)
	fprintf(outfile, "&nbsp;");
}

static int from_utf8(char *s) {
    int i = 1;
    int r = (unsigned char)s[0];
    int m;
    for (m = 0xc0; (r & m) == m; m <<= 5)
        r = ((r & ~(m << 1)) << 6) | ((unsigned char)s[i++] & 0x3f);
    return r & ~(m << 1);
}



/* XXX needs to use a symbol table */
static int
ident_special(void)
{
    word_t *w = wordtab_search(idents, strval);
    if (w && w->data) {
        fprintf(outfile, "&#%d;", from_utf8(w->data));
        return 1;
    }
    return 0;
}

static void
op_ident(void)
{
    if (ident_special())
        return;
    fprintf(outfile, "*%s*", strval);
}

static void
op_keyword(void)
{
    fprintf(outfile, "**%s**", strval);
}

static void
op_stuff(void)
{
    fprintf(outfile, "%s", strval);
}


static struct {
    char *from, *to;
} special_ops[] = {
    {"<-", "&#8592;"},   /* LEFTWARDS ARROW */
    {"->", "&#8594;"},   /* RIGHTWARDS ARROW */
    {"<=", "&#8804;"},   /* LESS-THAN OR EQUAL TO */
    {">=", "&#8805;"},   /* GREATER-THAN OR EQUAL TO */
    {">", "&gt;"},
    {"<", "&lt;"},
    {"&", "&amp;"},
    {0, 0}
};

/* XXX needs to use a symbol table */
static int
op_special(void)
{
    int i;
    for (i = 0; special_ops[i].from != 0; i++) {
	if (!strcmp(strval, special_ops[i].from)) {
	    fprintf(outfile, "%s", special_ops[i].to);
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
    fprintf(outfile, "%s", strval);
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

output_t output_markdown = {
    .extension = ".mdwn",
    .init = op_init,
    .begin = op_begin,
    .end = op_end,
    .bol = op_bol,
    .eol = op_eol,
    .xlate = xlate
};
