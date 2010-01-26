/*
 * Copyright © 2009 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "3-clause ('new') BSD License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

/* XXX Emacs does not handle UTF-8 outside the BMP
   properly, so we have to edit this unicode table
   with something else. */

#include "symbols.h"

symbol_t symbols[] = {
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
  /*
   * UTF-8 encodings of LaTeX symbols
   * from Jos van den Oever
   * http://marc.info/?l=vim&m=101923104802786&w=2
   */
  {"leq", "≤", 0},
  {"le", "≤", 0},
  {"ll", "≪", 0},
  {"muchlt", "≪", "ll"},
  {"prec", "≺", 0},
  {"ordlt", "≺", "prec"},
  {"varpreceq", "≼", "preceq"},
  {"precle", "≼", 0},
  {"subset", "⊂", 0},
  {"subseteq", "⊆", 0},
  {"sqsubset", "⊏", 0},
  {"sqsubseteq", "⊑", 0},
  {"varin", "∈", "in"},
  {"inset", "∈", "in"},
  {"vdash", "⊢", 0},
  {"turnstyle", "⊢", "vdash"},
  {"varmid", "∣", "mid"},
  {"pipechar", "∣", "mid"},
  {"smile", "⌣", 0},
  {"geq", "≥", 0},
  {"gg", "≫", 0},
  {"muchgt", "≫", "gg"},
  {"varsucc", "≻", "succ"},
  {"ordgt", "≻", "succ"},
  {"succeq", "≽", 0},
  {"ordge", "≽", "succeq"},
  {"supset", "⊃", 0},
  {"supseteq", "⊇", 0},
  {"sqsupset", "⊐", 0},
  {"sqsupseteq", "⊒", 0},
  {"ni", "∋", 0},
  {"dashv", "⊣", 0},
  {"varparallel", "∥", "parallel"},
  {"doublepipe", "∥", "parallel"},
  {"frown", "⌢", 0},
  {"notin", "∉", 0},
  {"notinset", "∉", "notin"},
  {"equiv", "≡", 0},
  {"doteq", "≐", 0},
  {"sim", "∼", 0},
  {"simeq", "≃", 0},
  {"approx", "≈", 0},
  {"cong", "≅", 0},
  {"bowtie", "⋈", 0},
  {"propto", "∝", 0},
  {"varmodels", "⊨", "models"},
  {"doubleturnstyle", "⊨", "models"},
  {"perp", "⊥", 0},
  {"bot", "⊥", "perp"},
  {"asymp", "≍", 0},
  {"neq", "≠", 0},
  {"pm", "±", 0},
  {"plusminus", "±", "pm"},
  {"cdot", "⋅", 0},
  {"vartimes", "×", "times"},
  {"cross", "×", "times"},
  {"gradeschooltimes", "×", "times"},
  {"cup", "∪", 0},
  {"sqcup", "⊔", 0},
  {"vee", "∨", 0},
  {"lor", "∨", "vee"},
  {"oplus", "⊕", 0},
  {"odot", "⊙", 0},
  {"otimes", "⊗", 0},
  {"bigtriangleup", "△", 0},
  {"lhd", "⊲", 0},
  {"unlhd", "⊴", 0},
  {"mp", "∓", 0},
  {"vardiv", "÷", "div"},
  {"gradeschooldiv", "÷", "div"},
  {"setminus", "∖", 0},
  {"cap", "∩", 0},
  {"sqcap", "⊓", 0},
  {"wedge", "∧", 0},
  {"land", "∧", "wedge"},
  {"ominus", "⊖", 0},
  {"oslash", "⊘", 0},
  {"bigcirc", "○", 0},
  {"bigtriangledown", "▽", 0},
  {"rhd", "⊳", 0},
  {"unrhd", "⊵", 0},
  {"triangleleft", "◁", 0},
  {"triangleright", "▷", 0},
  {"varstar", "⋆", "star"},
  {"pentastar", "⋆", 0},
  {"ast", "∗", 0},
  {"circ", "∘", 0},
  {"varbullet", "∙", "bullet"},
  {"sqdot", "∙", "bullet"},
  {"diamond", "⋄", 0},
  {"uplus", "⊎", 0},
  {"dagger", "†", 0},
  {"ddagger", "‡", 0},
  {"varwr", "≀", "wr"},
  {"varsum", "∑", 0},
  {"sigma", "∑", 0},
  {"prod", "∏", 0},
  {"coprod", "∐", 0},
  {"varint", "∫", "int"},
  {"integralsign", "∫", "int"},
  {"bigcup", "⋃", 0},
  {"bigcap", "⋂", 0},
  {"bigsqcup", "⊔", 0},
  {"oint", "∮", 0},
  {"bigvee", "⋁", 0},
  {"bigwedge", "⋀", 0},
  {"bigoplus", "⊕", 0},
  {"bigotimes", "⊗", 0},
  {"bigodot", "⊙", 0},
  {"biguplus", "⊎", 0},
  {"leftarrow", "←", 0},
  {"rightarrow", "→", 0},
  {"leftrightarrow", "↔", 0},
  {"Leftarrow", "⇐", 0},
  {"Rightarrow", "⇒", 0},
  {"Leftrightarrow", "⇔", 0},
  {"mapsto", "↦", 0},
  {"hookleftarrow", "↩", 0},
  {"leftharpoonup", "↼", 0},
  {"leftharpoondown", "↽", 0},
  {"hookrightarrow", "↪", 0},
  {"rightharpoonup", "⇀", 0},
  {"rightharpoondown", "⇁", 0},
  {"longleftarrow", "←", 0},
  {"longrightarrow", "→", 0},
  {"longleftrightarrow", "↔", 0},
  {"Longleftarrow", "⇐", 0},
  {"Longrightarrow", "⇒", 0},
  {"Longleftrightarrow", "⇔", 0},
  {"longmapsto", "⇖", 0},
  {"uparrow", "↑", 0},
  {"downarrow", "↓", 0},
  {"updownarrow", "↕", 0},
  {"Uparrow", "⇑", 0},
  {"Downarrow", "⇓", 0},
  {"Updownarrow", "⇕", 0},
  {"nearrow", "↗", 0},
  {"searrow", "↘", 0},
  {"swarrow", "↙", 0},
  {"nwarrow", "↖", 0},
  {"leadsto", "↝", 0},
  {"vardots", "…", 0},
  {"hdots", "…", 0},
  {"cdots", "⋯", 0},
  {"vdots", "⋮", 0},
  {"ddots", "⋱", 0},
  {"hbar", "ℏ", 0},
  {"ell", "ℓ", 0},
  {"Re", "ℜ", 0},
  {"Im", "ℑ", 0},
  {"aleph", "א", 0},
  {"wp", "℘", 0},
  {"forall", "∀", 0},
  {"exists", "∃", 0},
  {"mho", "℧", 0},
  {"varpartial", "∂", "partial"},
  {"partialderiv", "∂", "partial"},
  {"varprime", "′", "prime"},
  {"emptyset", "∅", 0},
  {"infty", "∞", 0},
  {"nabla", "∇", 0},
  {"vartriangle", "△", "triangle"},
  {"VarBox", "□", "Box"},
  {"VarDiamond", "◇", "Diamond"},
  {"varbottom", "⊥", "bot"},
  {"bot", "⊥", 0},
  {"ordbottom", "⊥", "bot"},
  {"vartop", "⊤", "top"},
  {"ordtop", "⊤", "top"},
  {"varangle", "∠", "angle"},
  {"geometricangle", "∠", "angle"},
  {"surd", "√", 0},
  {"diamondsuit", "♢", 0},
  {"heartsuit", "♡", 0},
  {"clubsuit", "♣", 0},
  {"spadesuit", "♠", 0},
  {"varneg", "¬", "neg"},
  {"lnot", "¬", "neg"},
  {"varflat", "♭", "flat"},
  {"varnatural", "♮", "natural"},
  {"varsharp", "♯", "sharp"},
  {"digamma", "Ϝ", 0},
  {"varkappa", "ϰ", 0},
  {"beth", "ב", 0},
  {"daleth", "ד", 0},
  {"gimel", "ג", 0},
  {"lessdot", "⋖", 0},
  {"leqslant", "≤", 0},
  {"leqq", "≦", 0},
  {"lll", "⋘", 0},
  {"lesssim", "≲", 0},
  {"lessgtr", "≶", 0},
  {"lesseqgtr", "⋚", 0},
  {"preccurlyeq", "≼", 0},
  {"curlyeqprec", "⋞", 0},
  {"precsim", "≾", 0},
  {"Subset", "⋐", 0},
  {"sqsubset", "⊏", 0},
  {"vartherefore", "∴", "therefore"},
  {"qed", "∴", "therefore"},
  {"smallsmile", "⌣", 0},
  {"vartriangleleft", "⊲", 0},
  {"trianglelefteq", "⊴", 0},
  {"gtrdot", "⋗", 0},
  {"geqq", "≧", 0},
  {"ggg", "⋙", 0},
  {"gtrsim", "≳", 0},
  {"gtrless", "≷", 0},
  {"gtreqless", "⋛", 0},
  {"succcurlyeq", "≽", 0},
  {"curlyeqsucc", "⋟", 0},
  {"succsim", "≿", 0},
  {"Supset", "⋑", 0},
  {"sqsupset", "⊐", 0},
  {"varbecause", "∵", "because"},
  {"shortparallel", "∥", 0},
  {"smallfrown", "⌢", 0},
  {"vartriangleright", "⊳", 0},
  {"trianglerighteq", "⊵", 0},
  {"doteqdot", "≑", 0},
  {"risingdotseq", "≓", 0},
  {"fallingdotseq", "≒", 0},
  {"eqcirc", "≖", 0},
  {"circeq", "≗", 0},
  {"triangleq", "≜", 0},
  {"bumpeq", "≏", 0},
  {"Bumpeq", "≎", 0},
  {"thicksim", "∼", 0},
  {"thickapprox", "≈", 0},
  {"approxeq", "≊", 0},
  {"backsim", "∽", 0},
  {"vDash", "⊨", 0},
  {"Vdash", "⊩", 0},
  {"Vvdash", "⊪", 0},
  {"backepsilon", "∍", 0},
  {"varpropto", "∝", 0},
  {"varbetween", "≬", "between"},
  {"varpitchfork", "⋔", "pitchfork"},
  {"blacktriangleleft", "◀", 0},
  {"blacktriangleright", "▷", 0},
  {"dashleftarrow", "⇠", 0},
  {"leftleftarrows", "⇇", 0},
  {"leftrightarrows", "⇆", 0},
  {"Lleftarrow", "⇚", 0},
  {"twoheadleftarrow", "↞", 0},
  {"leftarrowtail", "↢", 0},
  {"leftrightharpoons", "⇋", 0},
  {"Lsh", "↰", 0},
  {"looparrowleft", "↫", 0},
  {"curvearrowleft", "↶", 0},
  {"circlearrowleft", "↺", 0},
  {"dashrightarrow", "⇢", 0},
  {"rightrightarrows", "⇉", 0},
  {"rightleftarrows", "⇄", 0},
  {"Rrightarrow", "⇛", 0},
  {"twoheadrightarrow", "↠", 0},
  {"rightarrowtail", "↣", 0},
  {"rightleftharpoons", "⇌", 0},
  {"Rsh", "↱", 0},
  {"looparrowright", "↬", 0},
  {"curvearrowright", "↷", 0},
  {"circlearrowright", "↻", 0},
  {"multimap", "⊸", 0},
  {"upuparrows", "⇈", 0},
  {"downdownarrows", "⇊", 0},
  {"upharpoonleft", "↿", 0},
  {"upharpoonright", "↾", 0},
  {"downharpoonleft", "⇃", 0},
  {"downharpoonright", "⇂", 0},
  {"rightsquigarrow", "⇝", 0},
  {"leftrightsquigarrow", "↭", 0},
  {"dotplus", "∔", 0},
  {"ltimes", "⋉", 0},
  {"Cup", "⋓", 0},
  {"veebar", "⊻", 0},
  {"boxplus", "⊞", 0},
  {"boxtimes", "⊠", 0},
  {"leftthreetimes", "⋋", 0},
  {"curlyvee", "⋎", 0},
  {"centerdot", "⋅", 0},
  {"rtimes", "⋈", 0},
  {"Cap", "⋒", 0},
  {"barwedge", "⊼", 0},
  {"boxminus", "⊟", 0},
  {"boxdot", "⊡", 0},
  {"rightthreetimes", "⋌", 0},
  {"curlywedge", "⋏", 0},
  {"intercal", "⊺", 0},
  {"divideontimes", "⋇", 0},
  {"smallsetminus", "∖", 0},
  {"circleddash", "⊝", 0},
  {"circledcirc", "⊚", 0},
  {"circledast", "⊛", 0},
  {"hbar", "ħ", 0}, /*HERE*/
  {"hslash", "ℏ", 0},
  {"varsquare", "□", "square"},
  {"blacksquare", "■", 0},
  {"circledS", "Ⓢ", 0},
  {"vartriangle", "△", 0},
  {"blacktriangle", "▲", 0},
  {"varcomplement", "∁", "complement"},
  {"triangledown", "▽", 0},
  {"blacktriangledown", "▼", 0},
  {"lozenge", "◊", 0},
  {"blacklozenge", "◆", 0},
  {"bigstar", "★", 0},
  {"measuredangle", "∡", 0},
  {"sphericalangle", "∢", 0},
  {"backprime", "‵", 0},
  {"nexists", "∄", 0},
  {"Finv", "Ⅎ", 0},
  {"varnothing", "∅", "emptyset"},
  {"eth", "ð", 0},
  {"mho", "℧", 0},
  {0, 0, 0}
};
