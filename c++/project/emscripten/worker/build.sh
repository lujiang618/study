#!/bin/bash

rm -rf hi.html hi.js hi.wasm hi.wasm.map hi.worker.js worker.js worker.wasm

# 编译主线程
emcc worker_api_main.cpp -o hi.html \
    -pthread \
    -s PROXY_TO_PTHREAD \
    -s USE_PTHREADS=1 \
    -s PTHREAD_POOL_SIZE=2 \
    -s ASSERTIONS=2 \
    -gsource-map \
    -s WASM_BIGINT=1 \
    -s SAFE_HEAP=0 \
    -s INITIAL_MEMORY=64MB \
    -s MAXIMUM_MEMORY=64MB \
    -s ALLOW_MEMORY_GROWTH=0 \
    -s ASYNCIFY \
    -s ASYNCIFY_IMPORTS='["emscripten_sleep"]' \
    -s EXIT_RUNTIME=1 \
    -s NO_FILESYSTEM=1 \
    -Os \
    -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
    -s EXPORTED_FUNCTIONS='["_main", "_worker_ready_callback"]'

# 编译 Worker
emcc worker_api_worker.cpp -o worker.js \
    -s BUILD_AS_WORKER=1 \
    -s NO_FILESYSTEM=1 \
    -s EXPORTED_FUNCTIONS='["_one", "_init"]'

echo "Build complete! Remember to run with a server that sets COOP/COEP headers"