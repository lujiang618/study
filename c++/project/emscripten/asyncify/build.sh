#!/bin/bash

emcc debug.cpp -o ./debug/hi.html \
    -s WASM_BIGINT=1 \
    -s SAFE_HEAP=0 \
    --bind \
    -O3 \
    -s ASYNCIFY=1 \
    -s ASYNCIFY_IMPORTS='["emscripten_sleep"]' \
    -s ASYNCIFY_STACK_SIZE=32768 \
    -s ALLOW_MEMORY_GROWTH=1

emcc loop.cpp -o ./loop/hi.html \
    -pthread \
    -s PROXY_TO_PTHREAD \
    -s WASM_BIGINT=1 \
    -s SAFE_HEAP=0 \
    --bind \
    -s ASYNCIFY=1 \
    -s ASYNCIFY_IMPORTS='["emscripten_sleep"]' \
    -s ASYNCIFY_STACK_SIZE=32768 \
    -s ALLOW_MEMORY_GROWTH=1

rm -rf ./worker/hi.html ./worker/hi.js ./worker/hi.wasm ./worker/hi.wasm.map ./worker/hi.worker.js
rm -rf ./worker/worker.js ./worker/worker.wasm

# 编译主线程
emcc main.cpp -o ./worker/hi.html \
    -s ASSERTIONS=2 \
    -s WASM_BIGINT=1 \
    -s SAFE_HEAP=0 \
    --bind \
    -s ASYNCIFY=1 \
    -s ASYNCIFY_IMPORTS='["emscripten_sleep"]' \
    -s ASYNCIFY_STACK_SIZE=5242880 \
    -s ALLOW_MEMORY_GROWTH=1 \

# 编译 Worker
emcc -std=c++17 worker.cpp -o ./worker/worker.js \
    -s BUILD_AS_WORKER=1 \
    --bind \
    -s ASYNCIFY=1 \
    -s ASYNCIFY_STACK_SIZE=5242880 \
    -s ENVIRONMENT=worker \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s DISABLE_EXCEPTION_CATCHING=0 \
    -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1

echo "Build complete! Remember to run with a server that sets COOP/COEP headers"
