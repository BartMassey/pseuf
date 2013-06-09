#! /bin/sh
rm -f configure
autoreconf -Wall -v --install || exit 1
./configure "$@"
