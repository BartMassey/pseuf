/* XXX Neither emacs nor vi handles UTF-8 outside the BMP
   properly, so we have to edit the greek -> unicode table
   with gedit.  So we include this rather than inlining it.
   We then share it with the LaTeX backend, which needs a
   subset of the information. */

#include "greek.h"

struct greek_map greek_idents[] = {
  {"alpha", "𝛼"},
  {"beta", "𝛽"},
  {"gamma", "𝛾"},
  {"delta", "𝛿"},
  {"epsilon", "𝜖"},
  {"varepsilon", "𝜀"},
  {"zeta", "𝜁"},
  {"eta", "𝜂"},
  {"theta", "𝜃"},
  {"vartheta", "𝜗"},
  {"iota", "𝜄"},
  {"kappa", "𝜅"},
  {"lambda", "𝜆"},
  {"mu", "𝜇"},
  {"nu", "𝜈"},
  {"xi", "𝜉"},
  {"omicron", "𝜊"},
  {"pi", "𝜋"},
  {"varpi", "𝜛"},
  {"rho", "𝜌"},
  {"varrho", "𝜚"},
  {"varsigma", "𝜍"},
  {"sigma", "𝜎"},
  {"varsigma", "𝜍"},
  {"tau", "𝜏"},
  {"upsilon", "𝜐"},
  {"phi", "𝜙"},
  {"varphi", "𝜑"},
  {"chi", "𝜒"},
  {"psi", "𝜓"},
  {"omega", "𝜔"},
  {"Alpha", "𝛢"},
  {"Beta", "𝛣"},
  {"Gamma", "𝛤"},
  {"Delta", "𝛥"},
  {"Epsilon", "𝛦"},
  {"Zeta", "𝛧"},
  {"Eta", "𝛨"},
  {"Theta", "𝛩"},
  {"Iota", "𝛪"},
  {"Kappa", "𝛫"},
  {"Lambda", "𝛬"},
  {"Mu", "𝛭"},
  {"Nu", "𝛮"},
  {"Xi", "𝛯"},
  {"Omicron", "𝛰"},
  {"Pi", "𝛱"},
  {"Rho", "𝛲"},
  {"Theta", "𝛳"},
  {"Sigma", "𝛴"},
  {"Tau", "𝛵"},
  {"Upsilon", "𝛶"},
  {"Phi", "𝛷"},
  {"Chi", "𝛸"},
  {"Psi", "𝛹"},
  {"Omega", "𝛺"},
  {0, 0}
};
