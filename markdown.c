#include <stdio.h>
#include "pcode.h"

static void
op_indent(void)
{
    int i;
    for (i = 0; i < intval; i++)
	printf(" ");
}

static int start = 1;

static void
print_word(char *fmt, char *word) {
    if (!start)
	printf(" ");
    printf(fmt, word);
    start = 0;
}

static void
op_ident(void)
{
    print_word("*%s*", strval);
}

static void
op_keyword(void)
{
    print_word("**%s**", strval);
}

static void
op_stuff(void)
{
    print_word("%s", strval);
}

static void
op_open_expr(void)
{
    if (nest > 1) {
	print_word("%s", "(");
	start = 1;
    }
}

static void
op_close_expr(void)
{
    if (nest > 0)
	printf(")");
    start = 0;
}

static void
op_newline(void)
{
    printf("\n");
    start = 1;
}

xlate output_markdown[] = {
    {T_INDENT, op_indent},
    {T_IDENT, op_ident},
    {T_KEYWORD, op_keyword},
    {T_WORD, op_stuff},
    {T_LP, op_open_expr},
    {T_RP, op_close_expr},
    {T_NEWLINE, op_newline},
    {T_STUFF, op_stuff},
    {T_STRING, op_stuff},
    {0, 0}
};
