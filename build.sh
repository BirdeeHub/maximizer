#!/usr/bin/env bash
which gcc || { echo "gcc not found" && exit 1; }
gcc -o maximize_program ./maximizer.c -lX11 -lXrandr
