/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pseuf.h"

word_t *
wordtab_search(word_t *table, char *word) {
    word_t *w;
    for (w = table; w->word; w++)
	if (!strcmp(w->word, word))
	    return w;
    return 0;
}

void
wordtab_clear(word_t *table, char *word) {
    word_t *toclear, *last;
    for (toclear = table; toclear->word; toclear++) {
	if (!strcmp(toclear->word, word)) {
	    last = toclear;
            while ((last + 1)->word)
	        last++;
	    *toclear = *last;
	    last->word = 0;
	    continue;
	}
    }
}

void
wordtab_insert(word_t *table, word_t *word) {
    word_t *last = table;
    while (last - table < TABLE_SIZE - 1 &&
	   last->word &&
	   strcmp(last->word, word->word))
	last++;
    if (last - table >= TABLE_SIZE - 1) {
	fprintf(stderr, "wordtab: out of memory\n");
	exit(1);
    }
    if (last->word) {
	last->data = word->data;
    } else {
	*last = *word;
	last++;
	last->word = 0;
    }
}

