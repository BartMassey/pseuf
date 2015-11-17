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
#include "specialchars.h"

static int textstart;

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
op_begin(void)
{
    fprintf(outfile, "<!-- This pseudocode translated from %s by pseuf -->\n",
            filename);
    fprintf(outfile, "\n");
    textstart = 1;
}

static void
op_end(void)
{
    fprintf(outfile, "\n");
    fprintf(outfile, "<!-- End of pseuf translation of %s -->\n", filename);
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
    textstart = 1;
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
        symbol_t *s = w->data;
        fprintf(outfile, "&#%d;", from_utf8(s->to));
        return 1;
    }
    return 0;
}

static char *
subst_prime(char *s) {
    static char *p = "&#x2032;";
    static char b[1024];
    char *t = b;
    while (*s != '\0' && t < b + sizeof(b) - strlen(p) - 1) {
        if (*s == '\'') {
            strcpy(t, p);
            t += strlen(p);
        } else {
            *t++ = *s;
        }
        s++;
    }
    *t = '\0';
    return b;
}

static void
op_ident(void)
{
    textstart = 0;
    if (ident_special())
        return;
    fprintf(outfile, "*%s*", subst_prime(strval));
}

static void
op_keyword(void)
{
    textstart = 0;
    fprintf(outfile, "**%s**", strval);
}

static void
op_stuff(void)
{
    textstart = 0;
    fprintf(outfile, "%s", strval);
}


static struct {
    char *from, *to;
} special_ops[] = {
    {"<-", "&#8592;"},   /* LEFTWARDS ARROW */
    {"->", "&#8594;"},   /* RIGHTWARDS ARROW */
    {"<->", "&#8596;"},    /* LEFT RIGHT ARROW */
    {"<-->", "&#10321;"},   /* LONG LEFT RIGHT ARROW */
    {"=/=", "&#8800;"},  /* NOT EQUAL TO */
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
    textstart = 0;
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
    fprintf(outfile, "&nbsp;");
}

static void
op_specialchar(void)
{
    textstart = 0;
    int c = strval[1];
    switch (strval[0]) {
    case 'b':
        if ('A' <= c && c <= 'Z') {
            char *bchar = specialchar_search(specialchar_blackboard, c);
            if (bchar) {
                fprintf(outfile, "&#%d;", from_utf8(bchar));
                return;
            }
        } else if ('a' <= c && c <= 'z') {
            int bchar = 0x1d552;  /* MATHEMATICAL DOUBLE-STRUCK SMALL A */
            fprintf(outfile, "&#%d;", bchar + c - 'a');
            return;
        }
        break;
    case 'f':
        if ('A' <= c && c <= 'Z') {
            int fchar = 0x1d56c;  /* MATHEMATICAL BOLD FRAKTUR CAPITAL A */
            fprintf(outfile, "&#%d;", fchar + c - 'A');
            return;
        } else if ('a' <= c && c <= 'z') {
            int fchar = 0x1d586;  /* MATHEMATICAL BOLD FRAKTUR SMALL A */
            fprintf(outfile, "&#%d;", fchar + c - 'a');
            return;
        }
        break;
    case 's':
        if ('A' <= c && c <= 'Z') {
            int schar = 0x1d4d0;  /* MATHEMATICAL BOLD SCRIPT CAPITAL A */
            fprintf(outfile, "&#%d;", schar + c - 'A');
            return;
        } else if ('a' <= c && c <= 'z') {
            int schar = 0x1d4ea;  /* MATHEMATICAL BOLD SCRIPT SMALL A */
            fprintf(outfile, "&#%d;", schar + c - 'a');
            return;
        }
        break;
    }
    warning("unknown entity for special character \\%s\n", strval);
    fprintf(outfile, "&#%d;", 0xfffd);  /* REPLACEMENT CHARACTER */
}

static void
op_comment(void)
{
    if (!textstart)
        fprintf(outfile, "&nbsp;&nbsp;&nbsp;");
    fprintf(outfile, "%s", strval);
    textstart = 0;
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
    {T_COMMENT, op_comment},
    {0, 0}
};

output_t output_markdown = {
    .name = "markdown",
    .extension = ".mdwn",
    .init = op_init,
    .begin = op_begin,
    .end = op_end,
    .bol = op_bol,
    .eol = op_eol,
    .xlate = xlate
};
