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

static int outline = 0;

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
    outline++;
    fprintf(outfile, "    ");
}

static void
op_eol(void)
{
    outline++;
    fprintf(outfile, "  ");
}

static void
op_indent(void)
{
    int i;
    for (i = 0; i < intval; i++)
	fprintf(outfile, " ");
}

static void
op_ident(void)
{
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
} specials[] = {
    {"->", "→"},
    {"<-", "←"},
    {">=", "≥"},
    {"<=", "≤"},
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

output_t output_markdown_unicode = {
    .extension = ".mdwn",
    .begin = op_begin,
    .end = op_end,
    .bol = op_bol,
    .eol = op_eol,
    .xlate = xlate
};