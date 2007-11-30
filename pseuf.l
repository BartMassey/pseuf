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

    static char *save_string(char *);
    static int is_keyword(char *);
    static void string_init(void);
    static void string_add(char);
    static char * string_save(void);
    static int lookup_directive(char *);
    int line = 1;
    char *filename = "<stdin>";
    FILE *outfile;
    int nest = 0;
    char *strval = 0;
    int intval;
    op_t opval;
%} 
    
%x STRING
%x DIRECTIVE
%x STUFF
%option noyywrap

ID [_a-zA-Z][-_a-zA-Z0-9]*
STUFF [^\"\(\) \t\n]

%%

^\% {
    BEGIN(DIRECTIVE);
}

<DIRECTIVE>[ \t]+   /* do nothing */;

<DIRECTIVE>{ID} {
    int token = lookup_directive(yytext);
    error(token == 0, "unknown directive %s", yytext);
    BEGIN(0);
    return token;
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
    if (nest > 0)
	return T_IDENT;
    if (is_keyword(yytext))
	return T_KEYWORD;
    return T_WORD;
}

\" {
    string_init();
    BEGIN(STRING);
}

"(" {
    nest++;
    return T_LP;
}

")" {
    if (nest > 0)
	--nest;
    return T_RP;
}

[ \t]+ {
    return T_WHITE;
}

\n {
    line++;
    return T_NEWLINE;
}

{STUFF} {
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


static char *keywords[] = {
    "to", "if", "else", "do", "in", "for", "while", 0
};


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


typedef struct {
    char *name;
    int token;
    op_t op;
} directive_t;

static void
d_plain(void) {
    int idt;
    char *id = get_id(&idt);
    int nl = yylex();
    char **toclear, **last;
    error(!id || nl != T_NEWLINE, "%%plain directive syntax error");
    if (idt != T_KEYWORD) {
	warning("%%plain directive on non-keyword %s", id);
	return;
    }
    for (toclear = keywords; *toclear; toclear++) {
	if (!strcmp(*toclear, id)) {
	    for (last = keywords; *last && *(last + 1); last++)
		/* do nothing */;
	    *toclear = *last;
	    *last = 0;
	    continue;
	}
    }
    free(id);
}


directive_t directives[] = {
    {.name = "plain", .token = TD_PLAIN, .op = d_plain},
    {0, 0, 0}
};

static int
lookup_directive(char *name) {
    directive_t *dp;
    for (dp = directives; dp->name; dp++) {
	if (!strcmp(name, dp->name)) {
	    opval = dp->op;
	    return dp->token;
	}
    }
    return 0;
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

static int
is_keyword(char *word) {
    char **kw;
    for (kw = keywords; *kw; kw++)
	if (!strcmp(*kw, word))
	    return 1;
    return 0;
}


static void
format_file(void) {
    int token;
    xlate_t *xp;
    output_markdown.begin();
    output_markdown.bol();
    while ((token = yylex()) != 0) {
	if (token >= T_DIRECTIVE) {
	    opval();
	    continue;
	}
	for (xp = output_markdown.xlate; xp->token != 0; xp++) {
	    if (token == xp->token) {
		xp->op();
		if (strval)
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
