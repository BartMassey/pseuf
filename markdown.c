/*
 * Copyright (C) 2007 Bart Massey
 * ALL RIGHTS RESERVED
 * 
 * [This program is licensed under the GPL version 3 or later.]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <stdio.h>
#include "pseuf.h"

static int out_line = 0;

static void
op_begin(void)
{
    printf("\n");
}

static void
op_end(void)
{
    printf("\n");
}

static void
op_bol(void)
{
    out_line++;
    printf("    ");
}

static void
op_indent(void)
{
    int i;
    for (i = 0; i < intval; i++)
	printf(" ");
}

static void
op_ident(void)
{
    printf("*%s*", strval);
}

static void
op_keyword(void)
{
    printf("**%s**", strval);
}

static void
op_stuff(void)
{
    printf("%s", strval);
}

static void
op_open_expr(void)
{
    if (nest > 1)
	printf("(");
}

static void
op_close_expr(void)
{
    if (nest > 0)
	printf(")");
}

static void
op_newline(void)
{
    printf("\n");
}

static void
op_white(void)
{
    printf(" ");
}

static xlate_t xlate[] = {
    {T_INDENT, op_indent},
    {T_IDENT, op_ident},
    {T_KEYWORD, op_keyword},
    {T_WORD, op_stuff},
    {T_LP, op_open_expr},
    {T_RP, op_close_expr},
    {T_NEWLINE, op_newline},
    {T_STUFF, op_stuff},
    {T_STRING, op_stuff},
    {T_WHITE, op_white},
    {0, 0}
};

output_t output_markdown = {
    .extension = ".mdwn",
    .begin = op_begin,
    .end = op_end,
    .bol = op_bol,
    .xlate = xlate
};
