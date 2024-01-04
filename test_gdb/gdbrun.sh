#!/bin/sh
gdb-multiarch build/test_gdb.elf \
-readnow \
-ex 'target remote [::1]:9123' \
-ex 'set python print-stack full' \
-ex 'source gdb_corrupt.py'
