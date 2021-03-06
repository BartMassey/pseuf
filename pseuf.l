/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

%{
#   include <stdio.h>
#   include "pseuf.h"
#   include "symbols.h"

    typedef struct {
	char *name;
	int token;
	op_t op;
    } directive_t;

    static word_t directives[];
    static char *comment_symbol;

    static char *save_string(char *);
    static void string_init(void);
    static void string_add(char);
    static char * string_save(void);

    int line = 1;
    char *filename = "<stdin>";
    FILE *outfile;
    char *strval;
    int intval;
    directive_t *dirval;
%} 
    
%x COMMENT
%x STRING
%x DIRECTIVE
%x STUFF
%option noyywrap

ID [_a-zA-Z][-_a-zA-Z0-9\']*
STUFF [^\"\(\)_a-zA-Z \t\n]

%%
^\%\%.*\n {
    /* comment */
}

^\%[ \t]* {
    BEGIN(DIRECTIVE);
}

<DIRECTIVE>{ID} {
    word_t *dir = wordtab_search(directives, yytext);
    error(dir == 0, "unknown directive %s", yytext);
    dirval = dir->data;
    BEGIN(0);
    return dirval->token;
}

<DIRECTIVE>. {
    error(1, "unexpected character in directive");
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

\" {
    string_init();
    BEGIN(STRING);
}

<STRING>\" {
    strval = string_save();
    BEGIN(0);
    return T_STRING;
}

<STRING>\\. {
    string_add(yytext[1]);
}

<STRING>[ \t\n]* {
    string_add(' ');
}

<STRING>. {
    string_add(yytext[0]);
}

\\[a-z][a-zA-Z] {
    strval = save_string(&yytext[1]);
    return T_SPECIALCHAR;
}

{ID} {
    strval = save_string(yytext);
    if (wordtab_search(keywords, yytext))
	return T_KEYWORD;
    if (wordtab_search(idents, yytext))
	return T_IDENT;
    return T_WORD;
}

"(" {
    strval = "(";
    return T_LP;
}

")" {
    strval = ")";
    return T_RP;
}

"{" {
    strval = "{";
    return T_LC;
}

"}" {
    strval = "}";
    return T_RC;
}

[ \t]+ {
    return T_WHITE;
}

{STUFF} {
    BEGIN(STUFF);
    yyless(0);
}

<STUFF>{STUFF}+ {
    if (comment_symbol) {
        char *start = strstr(yytext, comment_symbol);
        if (start > yytext) {
            char ch = *start;
            *start = '\0';
            strval = save_string(yytext);
            *start = ch;
            yyless(strlen(strval));
            return T_STUFF;
        }
        if (start == yytext) {
            strval = save_string(comment_symbol);
            if (strlen(yytext) == strlen(comment_symbol))
                BEGIN(0);
            yyless(strlen(comment_symbol));
            return T_COMMENT;
        }
    }
    strval = save_string(yytext);
    BEGIN(0);
    return T_STUFF;
}

<STUFF>\n {
    BEGIN(0);
    return T_NEWLINE;
}

"\r\n"|"\n" {
    return T_NEWLINE;
}

%%

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

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


word_t keywords[TABLE_SIZE] = {
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


word_t idents[TABLE_SIZE];

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

static char *
get_stuff(void) {
    int token = yylex();
    if (token == T_WHITE)
	token = yylex();
    if (token != T_STUFF)
        return 0;
    return yytext;
}

static void
directive_comment(char *name, void (*op)(int, char *)) {
    char ch, *stp, *stp1;
    char *stuff = get_stuff();
    error(!stuff, "%%%s directive syntax error", name);
    int token = yylex();
    error(token != T_NEWLINE, "%%%s directive illegal comment symbol", name);
    for (stp = stuff; *stp && !isspace(*stp); stp++)
        /* do nothing */;
    ch = *stp;
    *stp = '\0';
    for (stp1 = stp; *stp1 && isspace(*stp); stp1++)
        /* do nothing */;
    error(*stp1, "%%%s directive whitespace in comment symbol", name);
    op(T_STUFF, stuff);
    *stp = ch;
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

static void
d_comment(int idt, char *str) {
    if (comment_symbol)
        free(comment_symbol);
    comment_symbol = save_string(str);
}

static directive_t directive_data[] = {
    {.name = "plain", .token = TD_PLAIN, .op = d_plain},
    {.name = "ident", .token = TD_IDENT, .op = d_ident},
    {.name = "keyword", .token = TD_KEYWORD, .op = d_keyword},
    {.name = "comment", .token = TD_COMMENT, .op = d_comment},
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
symtab_init(void) {
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
format_file(output_t *output) {
    int bol = 1;
    int token;
    xlate_t *xp;
    symtab_init();
    if (output->init)
        output->init();
    if (output->begin)
        output->begin();
    while ((token = yylex()) != 0) {
	if (token >= T_DIRECTIVE) {
            if (token == TD_COMMENT)
                directive_comment(dirval->name, dirval->op);
            else
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
/* XXX leak like a sieve until bug is tracked down */
#if 0
		if (strval && xp->token != T_LP && xp->token != T_RP)
		    free(strval);
#endif
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

static output_t *outputs[] = {
    &output_markdown,
    &output_latex,
    &output_utf8,
    0
};

void
usage(void) {
    int i;
    fprintf(stderr, "pseuf: usage: pseuf [options] <file>.pseu ...\n");
    for (i = 0; outputs[i]; i++)
        fprintf(stderr, "\t--%s   Format output as %s (%s)\n",
                outputs[i]->name, outputs[i]->name,
                outputs[i]->extension);
}

int
main(int argc, char **argv) {
    int i, j;
    output_t *output = outputs[0];
    init_directives();
    for (i = 1; i < argc; i++) {
        for (j = 0; outputs[j]; j++) {
            if (strlen(argv[i]) > 2 &&
                !strncmp(argv[i], "--", 2) &&
                !strcmp(argv[i] + 2, outputs[j]->name)) {
                output = outputs[j];
                break;
            }
        }
        if (outputs[j])
            continue;
        if (!strcmp(argv[i], "--help")) {
            usage();
            return 0;
        }
        if (argv[i][0] == '-') {
            fprintf(stderr, "option %s unknown\n", argv[i]);
            usage();
            return 1;
        }
        break;
    }
    if (i == argc) {
	outfile = stdout;
	format_file(output);
    } else {
	for (; i < argc; i++) {
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
