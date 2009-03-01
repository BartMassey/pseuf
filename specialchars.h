/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

typedef struct {
    int c;
    char *s;
} specialchar_table_t;

extern specialchar_table_t specialchar_blackboard[];

extern char *specialchar_search(specialchar_table_t *, int);
