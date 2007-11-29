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

extern int nest;
extern char *strval;
extern int intval;

typedef void (*op_t)();

typedef struct {
    int token;
    op_t op;
} xlate_t;

typedef struct {
    op_t begin;
    op_t end;
    xlate_t *xlate;
} output_t;

extern output_t output_markdown;