/*
 * Copyright © 2010 Bart Massey
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
#include "specialchars.h"

/* XXX returns a pointer to static data */
static char *
to_utf8(int c) {
    static char s[5];
    int nbytes = 0;
    if (c <= 0x7f) {
        s[0] = c;
        s[1] = '\0';
        return s;
    }
    while(c > 0x3f) {
        s[nbytes] = 0x80 | (c & 0x3f);
        c >>= 6;
        nbytes++;
    }
    s[nbytes++] = ~0 << (8 - nbytes) | c;
    s[nbytes] = '\0';
    return s;
}

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
op_indent(void)
{
    int i;
    for (i = 0; i < intval; i++)
	fprintf(outfile, "  ");
}


/* XXX needs to use a symbol table */
static int
ident_special(void)
{
    word_t *w = wordtab_search(idents, strval);
    if (w && w->data) {
        symbol_t *s = w->data;
        fprintf(outfile, "%s", s->to);
        return 1;
    }
    return 0;
}

static void
op_ident(void)
{
    if (ident_special())
        return;
    fprintf(outfile, "%s", strval);
}

static void
op_keyword(void)
{
    fprintf(outfile, "%s", strval);
}

static void
op_stuff(void)
{
    fprintf(outfile, "%s", strval);
}


static struct {
    char *from, *to;
} special_ops[] = {
    {"<-", "←"},   /* LEFTWARDS ARROW */
    {"->", "→"},   /* RIGHTWARDS ARROW */
    {"=/=", "≠"},  /* NOT EQUAL TO */
    {"<=", "≤"},   /* LESS-THAN OR EQUAL TO */
    {">=", "≥"},   /* GREATER-THAN OR EQUAL TO */
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

static void
op_specialchar(void)
{
    int c = strval[1];
    switch (strval[0]) {
    case 'b':
        if ('A' <= c && c <= 'Z') {
            char *bchar = specialchar_search(specialchar_blackboard, c);
            if (bchar) {
                fprintf(outfile, "%s", bchar);
                return;
            }
        } else if ('a' <= c && c <= 'z') {
            int bchar = 0x1d552;  /* MATHEMATICAL DOUBLE-STRUCK SMALL A */
            fprintf(outfile, "%s", to_utf8(bchar + c - 'a'));
            return;
        }
        break;
    case 'f':
        if ('A' <= c && c <= 'Z') {
            int fchar = 0x1d56c;  /* MATHEMATICAL BOLD FRAKTUR CAPITAL A */
            fprintf(outfile, "%s", to_utf8(fchar + c - 'A'));
            return;
        } else if ('a' <= c && c <= 'z') {
            int fchar = 0x1d586;  /* MATHEMATICAL BOLD FRAKTUR SMALL A */
            fprintf(outfile, "%s", to_utf8(fchar + c - 'a'));
            return;
        }
        break;
    case 's':
        if ('A' <= c && c <= 'Z') {
            int schar = 0x1d4d0;  /* MATHEMATICAL BOLD SCRIPT CAPITAL A */
            fprintf(outfile, "%s", to_utf8(schar + c - 'A'));
            return;
        } else if ('a' <= c && c <= 'z') {
            int schar = 0x1d4ea;  /* MATHEMATICAL BOLD SCRIPT SMALL A */
            fprintf(outfile, "%s", to_utf8(schar + c - 'a'));
            return;
        }
        break;
    }
    warning("unknown special character \\%s\n", strval);
    fprintf(outfile, "%s", to_utf8(0xfffd));  /* REPLACEMENT CHARACTER */
}

static xlate_t xlate[] = {
    {T_INDENT, op_indent},
    {T_IDENT, op_ident},
    {T_KEYWORD, op_stuff},
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

output_t output_utf8 = {
    .name = "utf8",
    .extension = ".txt",
    .init = op_init,
    .begin = 0,
    .end = 0,
    .bol = 0,
    .eol = 0,
    .xlate = xlate
};
