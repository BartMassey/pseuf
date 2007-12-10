/*
 * Copyright (C) 2007 Bart Massey
 * ALL RIGHTS RESERVED
 * 
 * [This program is licensed under the GPL version 3 or later.]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

%{
#   include <stdio.h>
#   include "pseuf.h"

    typedef struct {
	char *name;
	int token;
	op_t op;
    } directive_t;

    static word_t directives[], keywords[], idents[];

    static char *save_string(char *);
    static void string_init(void);
    static void string_add(char);
    static char * string_save(void);
    int line = 1;
    char *filename = "<stdin>";
    FILE *outfile;
    char *strval = 0;
    int intval;
    directive_t *dirval;
%} 
    
%x STRING
%x DIRECTIVE
%x STUFF
%option noyywrap

ID [_a-zA-Z][-_a-zA-Z0-9]*
STUFF [^\"\(\)_a-zA-Z \t\n]

%%

^\% {
    BEGIN(DIRECTIVE);
}

<DIRECTIVE>[ \t]+   /* do nothing */;

<DIRECTIVE>{ID} {
    word_t *dir = wordtab_search(directives, yytext);
    error(dir == 0, "unknown directive %s", yytext);
    dirval = dir->data;
    BEGIN(0);
    return dirval->token;
}

^[ \t]* {
    int i;
    int indent = yyleng;
    for (i = 0; i < yyleng; i++)
	if (yytext[i] == '\t')
	    indent += 7;
    intval = indent;
    return T_INDENT;
}

<STRING>\" {
    strval = string_save();
    BEGIN(0);
    return T_STRING;
}

<STRING>\\. {
    string_add(yytext[0]);
}

<STRING>[ \t\n]* {
    string_add(' ');
}

<STRING>. {
    string_add(yytext[0]);
}

{ID} {
    strval = save_string(yytext);
    if (wordtab_search(keywords, yytext))
	return T_KEYWORD;
    if (wordtab_search(idents, yytext))
	return T_IDENT;
    return T_WORD;
}

\" {
    string_init();
    BEGIN(STRING);
}

"(" {
    strval = "(";
    return T_LP;
}

")" {
    strval = ")";
    return T_RP;
}

[ \t]+ {
    return T_WHITE;
}

\n {
    line++;
    return T_NEWLINE;
}

. {
    BEGIN(STUFF);
    yyless(0);
}

<STUFF>.{STUFF}* {
    strval = save_string(yytext);
    BEGIN(0);
    return T_STUFF;
}

%%

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

static void
showmsg(char *prefix, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%s:%d: ", filename, line);
    if (prefix)
	fprintf(stderr, "%s: ", prefix);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}
    
void
warning(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    showmsg("warning", fmt, ap);
}


void
error(int cond, char *fmt, ...)
{
    if (cond) {
	va_list ap;
	va_start(ap, fmt);
	showmsg(0, fmt, ap);
	exit(1);
    }
}


static word_t keywords[TABLE_SIZE] = {
    {"to", 0},
    {"if", 0},
    {"then", 0},
    {"else", 0},
    {"return", 0},
    {"do", 0},
    {"in", 0},
    {"for", 0},
    {"while", 0},
    {0, 0}
};

static word_t idents[TABLE_SIZE];

static char *
get_id(int *token) {
    *token = yylex();
    if (*token == T_WHITE)
	*token = yylex();
    switch (*token) {
    case T_KEYWORD:
    case T_IDENT:
    case T_WORD:
	return strval;
    }
    return 0;
}


static void
d_plain(void) {
    int idt;
    char *id = get_id(&idt);
    int nl = yylex();
    error(!id || nl != T_NEWLINE, "%%plain directive syntax error");
    if (idt != T_KEYWORD) {
	warning("%%plain directive on non-keyword %s", id);
	return;
    }
    wordtab_clear(keywords, id);
    free(id);
}

static directive_t directive_data[] = {
    {.name = "plain", .token = TD_PLAIN, .op = d_plain},
    {0, 0, 0}
};

static word_t directives[TABLE_SIZE];

static void
init_directives(void) {
    word_t *tab = directives;
    directive_t *dp;
    for (dp = directive_data; dp->name; dp++) {
	tab->word = dp->name;
	tab->data = dp;
    }
}

static char *
save_string(char *string) {
    char *save = malloc(strlen(string) + 1);
    error(!save, "out of memory");
    strcpy(save, string);
    return save;
}

/* XXX fixme --- this should be dynamic */
#define SAVELEN (16*1024)

static char save[SAVELEN];
static char *savep;

static void
string_init(void) {
    savep = save;
}

static void
string_add(char c) {
    error(savep >= save + SAVELEN - 1, "out of memory");
    *savep++ = c;
}

static char *
string_save(void) {
    error(savep >= save + SAVELEN - 1, "out of memory");
    *savep++ = '\0';
    return save_string(save);
}

static void
format_file(void) {
    int token;
    xlate_t *xp;
    output_markdown.begin();
    output_markdown.bol();
    while ((token = yylex()) != 0) {
	if (token >= T_DIRECTIVE) {
	    dirval->op();
	    continue;
	}
	for (xp = output_markdown.xlate; xp->token != 0; xp++) {
	    if (token == xp->token) {
		xp->op();
		if (strval && xp->token != T_LP && xp->token != T_RP)
		    free(strval);
		strval = 0;
		break;
	    }
	}
	assert(xp->token != 0);
	if (token == T_NEWLINE)
	    output_markdown.bol();
    }
    output_markdown.end();
}


static char *
basename(char *name, char *extension) {
    char *suffp = rindex(name, '.');
    if (!suffp)
	return name;
    if (!strcmp(suffp + 1, extension)) {
	char *result = save_string(name);
	result[strlen(result) - strlen(extension) - 1] = '\0';
	return result;
    }
    return 0;
}

int
main(int argc, char **argv) {
    init_directives();
    if (argc <= 1) {
	outfile = stdout;
	format_file();
    } else {
	int i;
	for (i = 1; i < argc; i++) {
	    char *outfilename;
	    char *s;
	    filename = argv[i];
	    s = basename(filename, "pseu");
	    if (!s) {
		fprintf(stderr, "%s: bad file extension\n", filename);
		exit(1);
	    }
	    yyin = fopen(filename, "r");
	    if (!yyin) {
		perror(filename);
		exit(1);
	    }
	    outfilename = malloc(strlen(s) +
				 strlen(output_markdown.extension) + 1);
	    if (!outfilename) {
		fprintf(stderr, "%s: out of memory\n", filename);
		exit(1);
	    }
	    strcpy(outfilename, s);
	    strcat(outfilename, output_markdown.extension);
	    free(s);
	    outfile = fopen(outfilename, "w");
	    if (!outfile) {
		perror(outfilename);
		exit(1);
	    }
	    format_file();
	    free(outfilename);
	    fclose(yyin);
	    fclose(outfile);
	}
    }
    return 0;
}
