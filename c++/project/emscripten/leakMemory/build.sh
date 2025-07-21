#!/bin/bash

rm -rf ./output

mkdir ./output

emcc main.cpp -o ./output/hi.html \
    -gsource-map \
    -fsanitize=address \
    -sALLOW_MEMORY_GROWTH \
    -sEXIT_RUNTIME