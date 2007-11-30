/*
 * Copyright (C) 2007 Bart Massey
 * ALL RIGHTS RESERVED
 * 
 * [This program is licensed under the GPL version 3 or later.]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#define T_INDENT 1
#define T_IDENT 2
#define T_KEYWORD 3
#define T_WORD 4
#define T_LP 5
#define T_RP 6
#define T_NEWLINE 7
#define T_STUFF 8
#define T_STRING 9
#define T_WHITE 10
#define T_DIRECTIVE 100
#define TD_PLAIN 101

extern int nest;
extern char *strval;
extern int intval;
extern int line;
extern char *filename;
extern FILE *outfile;

extern void
error(int cond, char *fmt, ...);

typedef void (*op_t)();

typedef struct {
    int token;
    op_t op;
} xlate_t;

typedef struct {
    char *extension;
    op_t begin, end, bol;
    xlate_t *xlate;
} output_t;

extern output_t output_markdown;
