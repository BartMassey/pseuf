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
    int indent = 0;
    for (i = 0; i < yyleng; i++)
	if (yytext[i] == '\t')
	    indent += 8 - (indent % 8);
        else
            indent++;
    intval = indent;
    return T_INDENT;
}

<STRING>\" {
    strval = string_save();
    BEGIN(0);
    return T_STRING;
}

<STRING>\\. {
    string_add(yytext[1]);
}

<STRING>[ \t]* {
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
    return T_NEWLINE;
}

. {
    BEGIN(STUFF);
    yyless(0);
}

<STUFF>{STUFF}+ {
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
showmsg(char *prefix, char *fmt, va_list ap)
{
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
    va_end(ap);
}


void
error(int cond, char *fmt, ...)
{
    va_list ap;
    if (cond) {
	va_start(ap, fmt);
	showmsg(0, fmt, ap);
	va_end(ap);
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
directive_multiword(char *name, void (*op)(int, char *)) {
    while (1) {
	int idt;
	char *id = get_id(&idt);
	if (idt == T_NEWLINE) {
	    line++;
	    break;
	}
	error(!id, "%%%s directive syntax error", name);
	op(idt, id);
    }
}


static void
d_plain(int idt, char *id) {
    if (idt != T_KEYWORD && idt != T_IDENT) {
	warning("%%plain directive on already plain %s", id);
	return;
    }
    wordtab_clear(keywords, id);
    wordtab_clear(idents, id);
}


static void
d_ident(int idt, char *id) {
    word_t word;
    if (idt == T_IDENT) {
	warning("%%ident directive on already ident %s", id);
	return;
    }
    word.word = id;
    word.data = 0;
    wordtab_clear(keywords, id);
    wordtab_insert(idents, &word);
}


static void
d_keyword(int idt, char *id) {
    word_t word;
    if (idt == T_KEYWORD) {
	warning("%%keyword directive on already keyword %s", id);
	return;
    }
    word.word = id;
    word.data = 0;
    wordtab_clear(idents, id);
    wordtab_insert(keywords, &word);
}


static directive_t directive_data[] = {
    {.name = "plain", .token = TD_PLAIN, .op = d_plain},
    {.name = "ident", .token = TD_IDENT, .op = d_ident},
    {.name = "keyword", .token = TD_KEYWORD, .op = d_keyword},
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
	tab++;
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
format_file(output_t *output) {
    int bol = 1;
    int token;
    xlate_t *xp;
    if (output->begin)
        output->begin();
    while ((token = yylex()) != 0) {
	if (token >= T_DIRECTIVE) {
	    directive_multiword(dirval->name, dirval->op);
	    continue;
	}
        if (bol && output->bol)
            output->bol();
        bol = 0;
	for (xp = output->xlate; xp->token != 0; xp++) {
	    if (token == xp->token) {
                if (token == T_NEWLINE) {
                    if (output->eol)
                        output->eol();
                    if (xp->op)
                        xp->op();
                    line++;
                    bol = 1;
                } else {
                    if (xp->op)
                        xp->op();
                }
		if (strval && xp->token != T_LP && xp->token != T_RP)
		    free(strval);
		strval = 0;
		break;
	    }
	}
	assert(xp->token != 0);
    }
    if (output->end)
        output->end();
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
    output_t *output = &output_markdown;
    init_directives();
    if (argc <= 1) {
	outfile = stdout;
	format_file(output);
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
				 strlen(output->extension) + 1);
	    if (!outfilename) {
		fprintf(stderr, "%s: out of memory\n", filename);
		exit(1);
	    }
	    strcpy(outfilename, s);
	    strcat(outfilename, output->extension);
	    free(s);
	    outfile = fopen(outfilename, "w");
	    if (!outfile) {
		perror(outfilename);
		exit(1);
	    }
	    format_file(output);
	    free(outfilename);
	    fclose(yyin);
	    fclose(outfile);
	}
    }
    return 0;
}
