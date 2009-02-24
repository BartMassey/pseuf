/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

/* XXX Neither emacs nor vi handles UTF-8 outside the BMP
   properly, so we have to edit the greek -> unicode table
   with gedit.  So we include this rather than inlining it.
   We then share it with the LaTeX backend, which needs a
   subset of the information. */

#include "greek.h"

struct greek_map greek_idents[] = {
  {"alpha", "𝛼", 0},
  {"beta", "𝛽", 0},
  {"gamma", "𝛾", 0},
  {"delta", "𝛿", 0},
  {"epsilon", "𝜖", 0},
  {"varepsilon", "𝜀", 0},
  {"zeta", "𝜁", 0},
  {"eta", "𝜂", 0},
  {"theta", "𝜃", 0},
  {"vartheta", "𝜗", 0},
  {"iota", "𝜄", 0},
  {"kappa", "𝜅", 0},
  {"lambda", "𝜆", 0},
  {"mu", "𝜇", 0},
  {"nu", "𝜈", 0},
  {"xi", "𝜉", 0},
  {"omicron", "𝜊", 0},
  {"pi", "𝜋", 0},
  {"varpi", "𝜛", 0},
  {"rho", "𝜌", 0},
  {"varrho", "𝜚", 0},
  {"varsigma", "𝜍", 0},
  {"sigma", "𝜎", 0},
  {"varsigma", "𝜍", 0},
  {"tau", "𝜏", 0},
  {"upsilon", "𝜐", 0},
  {"phi", "𝜙", 0},
  {"varphi", "𝜑", 0},
  {"chi", "𝜒", 0},
  {"psi", "𝜓", 0},
  {"omega", "𝜔", 0},
  {"Alpha", "𝛢", "A"},
  {"Beta", "𝛣", "B"},
  {"Gamma", "𝛤", 0},
  {"Delta", "𝛥", 0},
  {"Epsilon", "𝛦", "E"},
  {"Zeta", "𝛧", "Z"},
  {"Eta", "𝛨", "H"},
  {"Theta", "𝛩", 0},
  {"VarTheta", "𝛳", "T"},
  {"Iota", "𝛪", "I"},
  {"Kappa", "𝛫", "K"},
  {"Lambda", "𝛬", 0},
  {"Mu", "𝛭", "M"},
  {"Nu", "𝛮", "N"},
  {"Xi", "𝛯", 0},
  {"Omicron", "𝛰", "O"},
  {"Pi", "𝛱", 0},
  {"Rho", "𝛲", "P"},
  {"Sigma", "𝛴", 0},
  {"Tau", "𝛵", "T"},
  {"Upsilon", "ϒ", 0},
  {"VarUpsilon", "𝛶" "Y"},
  {"Phi", "𝛷", 0},
  {"Chi", "𝛸", "X"},
  {"Psi", "𝛹", 0},
  {"Omega", "𝛺", 0},
  {0, 0, 0}
};
