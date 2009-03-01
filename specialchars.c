/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include "specialchars.h"

specialchar_table_t specialchar_blackboard[] = {
  {'A', "𝔸"},
  {'B', "𝔹"},
  {'C', "ℂ"},
  {'D', "𝔻"},
  {'E', "𝔼"},
  {'F', "𝔽"},
  {'G', "𝔾"},
  {'H', "ℍ"},
  {'I', "𝕀"},
  {'J', "𝕁"},
  {'K', "𝕂"},
  {'L', "𝕃"},
  {'M', "𝕄"},
  {'N', "𝕅"},
  {'O', "𝕆"},
  {'P', "𝕇"},
  {'Q', "ℚ"},
  {'R', "𝕉"},
  {'S', "𝕊"},
  {'T', "𝕋"},
  {'U', "𝕌"},
  {'V', "𝕍"},
  {'W', "𝕎"},
  {'X', "𝕏"},
  {'Y', "𝕐"},
  {'Z', "𝕑"},
  {0, 0}
};

char *specialchar_search(specialchar_table_t *t, int c) {
    int i;
    for (i = 0; t[i].s; i++)
        if (t[i].c == c)
            return t[i].s;
    return 0;
}
