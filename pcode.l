%{
#   include "pcode.h"

    static char *save_string(char *);
    static int is_keyword(char *);
    static void string_init(void);
    static void string_add(char);
    static char * string_save(void);
    int nest = 0;
    int white = 1;
    char *strval;
    int intval;
%} 
    
%x STRING
%s EXPR
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

[ \t]+ {
    /* do nothing */;
}

[_a-zA-Z][-_a-zA-Z0-9]* {
    strval = save_string(yytext);
    if (YY_START == EXPR)
	return T_IDENT;
    if (is_keyword(yytext))
	return T_KEYWORD;
    return T_WORD;
}

"(" {
    BEGIN(EXPR);
    nest++;
    return T_LP;
}

")" {
    if (nest > 0)
	--nest;
    if (nest == 0)
	BEGIN(0);
    return T_RP;
}

"\"" {
    BEGIN(STRING);
    string_init();
    white = 1;
}

<STRING>"\\\"" {
    string_add('"');
    white = 0;
}

<STRING>"\\"[ \t\n] {
    string_add(yytext[0]);
}

<STRING>"\\\\" {
    string_add('\\');
    white = 0;
}

<STRING>[ \t\n] {
    if (!white) {
        string_add(' ');
        white = 1;
    }
}

<STRING>"\"" {
    strval = string_save();
    BEGIN(0);
    return T_STRING;
}

<STRING>. {
    string_add(yytext[0]);
    white = 0;
}

\n {
    return T_NEWLINE;
}

[^ \t\n]+ {
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
    char *save = malloc(strlen(string));
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
    xlate *xp;
    while ((token = yylex()) != 0) {
	for (xp = output_markdown; xp->token != 0; xp++) {
	    if (token == xp->token) {
		xp->op();
		break;
	    }
	}
	assert(xp->token != 0);
    }
    return 0;
}