#! /bin/bash
gcc -g -O3 -m64 -std=c99 -flto -Wall -Wextra -Wpedantic \
    pyrepl.c -o pyrepl \
    $(python3-config --includes) \
    $(python3-config --ldflags)
