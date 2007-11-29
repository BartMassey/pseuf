#define T_INDENT 1
#define T_IDENT 2
#define T_KEYWORD 3
#define T_WORD 4
#define T_LP 5
#define T_RP 6
#define T_NEWLINE 7
#define T_STUFF 8
#define T_STRING 9

extern int nest;
extern int white;
extern char *strval;
extern int intval;

typedef struct {
    int token;
    void (*op)();
} xlate;

extern xlate output_markdown[];
