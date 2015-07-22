# pseuf
Copyright &copy; 2015 Bart Massey

This is a work-in-progress "pseudocode formatter" designed
to take ASCII pseudocode to well-formatted stuff. `pseuf`
currently supports output in LaTeX (`--latex`), Markdown
(`--markdown`) and raw UTF-8 (`--utf8`). It expects one or
more files on the command line with the file extension
`.pseu` and produces format as instructed by the formatting
flag.

This is really an unfinished unpolished pile of LEX and C. I
cannot recommend it yet. Consider it a proof-of-concept.

This program is licensed under the "3-clause ('new') BSD
License".  Please see the file `COPYING` in the source
distribution of this software for license terms.
