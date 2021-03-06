/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
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
#define T_SPECIALCHAR 11
#define T_COMMENT 12
#define T_LC 13
#define T_RC 14
#define T_DIRECTIVE 100
#define TD_PLAIN 101
#define TD_IDENT 102
#define TD_KEYWORD 103
#define TD_COMMENT 104

extern int nest;
extern char *strval;
extern int intval;
extern int line;
extern char *filename;
extern FILE *outfile;

extern void
warning(char *fmt, ...);

extern void
error(int cond, char *fmt, ...);

typedef void (*op_t)();

typedef struct {
    int token;
    op_t op;
} xlate_t;

typedef struct {
    char *name;
    char *extension;
    op_t init, begin, end, bol, eol;
    xlate_t *xlate;
} output_t;

extern output_t output_markdown;
extern output_t output_latex;
extern output_t output_utf8;

typedef struct {
    char *word;
    void *data;
} word_t;

#define TABLE_SIZE 1024

extern word_t keywords[], idents[];

extern word_t *
wordtab_search(word_t *table, char *word);

extern void
wordtab_clear(word_t *table, char *word);

extern void
wordtab_insert(word_t *table, word_t *word);
