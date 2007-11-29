/*
 * Copyright (C) 2007 Bart Massey
 * ALL RIGHTS RESERVED
 * 
 * [This program is licensed under the GPL version 3 or later.]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

%{
#   include "pseuf.h"

    static char *save_string(char *);
    static int is_keyword(char *);
    static void string_init(void);
    static void string_add(char);
    static char * string_save(void);
    int nest = 0;
    char *strval = 0;
    int intval;
%} 
    
%x STRING
%option noyywrap

%%

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

[_a-zA-Z][-_a-zA-Z0-9]* {
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
    return T_NEWLINE;
}

[^\"\(\) \t\n]+ {
    strval = save_string(yytext);
    return T_STUFF;
}

%%

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char *
save_string(char *string) {
    char *save = malloc(strlen(string) + 1);
    assert(save);
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
    assert(savep < save + SAVELEN - 1);
    *savep++ = c;
}

static char *
string_save(void) {
    assert(savep < save + SAVELEN - 1);
    *savep++ = '\0';
    return save_string(save);
}

static char *keywords[] = {
    "to", "if", "else", "do", "in", "for", "while", 0
};

static int
is_keyword(char *word) {
    char **kw;
    for (kw = keywords; *kw; kw++)
	if (!strcmp(*kw, word))
	    return 1;
    return 0;
}


int
main(int argc, char **argv) {
    int token;
    xlate_t *xp;
    output_markdown.begin();
    output_markdown.bol();
    while ((token = yylex()) != 0) {
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
    return 0;
}
