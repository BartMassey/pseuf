#! /bin/sh
# Copyright © 2015 Bart Massey
# [This program is licensed under the "3-clause ('new') BSD License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.
rm -f configure
autoreconf -Wall -v --install || exit 1
./configure "$@"
