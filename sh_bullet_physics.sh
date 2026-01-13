#!/bin/bash


# make -f Makefile-bullet-physics fclean
# make -f Makefile-bullet-physics -j6

make -f Makefile-bullet-physics build_platform=native build_mode=release -j6
make -f Makefile-bullet-physics build_platform=native build_mode=debug -j6
make -f Makefile-bullet-physics build_platform=web-wasm build_mode=release -j6

