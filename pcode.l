%{
#define T_INDENT 1
#define T_WHITESPACE 2
#define T_IDENT 3
#define T_KEYWORD 3
#define T_WORD 4
#define T_LP 5
#define T_RP 6
#define T_NEWLINE 7
#define T_STUFF 8

    extern char *save_string(char *);
    extern int is_keyword(char *);
    extern void string_init(void);
    extern void string_add(char);
    extern void string_save(void);

    static char *strval;
    static int intval;
%} 
    
%x STRING
%s EXPR

%%

^[ \t]* {
    int i;
    int indent = yylen;
    for (i = 0; i < yylen; i++)
	if (yytext[i] == '\t')
	    indent += 7;
    intval = indent;
    return T_INDENT;
}

[ \t]+ {
    /* do nothing */;
}

<EXPR> [_a-zA-Z][-_a-zA-Z0-9]* {
    strval = save_string(yytext);
    if (YYSTATE == EXPR)
	return T_IDENT;
    if (is_keyword(yytext))
	return T_KEYWORD;
    return T_WORD;
}

"(" {
    BEGIN(EXPR);
    indent++;
    return T_LP;
}

")" {
    if (indent > 0)
	--indent;
    if (indent == 0)
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
    string_add('\');
    white = 0;
}

<STRING>[ \t\n] {
    if (!white) {
        string_add(' ');
        white = 1;
    }
}

<STRING>"\"" {
    string_save();
    BEGIN(0);
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
