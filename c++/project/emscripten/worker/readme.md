



```
emcc worker_api_worker.cpp -o worker.js -s ASYNCIFY=1 --bind -s NO_DISABLE_EXCEPTION_CATCHING -s ENVIRONMENT=web -sALLOW_MEMORY_GROWTH=1 -sFETCH_DEBUG -sGL_DEBUG -sFS_DEBUG -g4

emcc worker_api_main.cpp -o hi.html -s ASYNCIFY=1 --bind -s NO_DISABLE_EXCEPTION_CATCHING -sENVIRONMENT=web,worker -sALLOW_MEMORY_GROWTH=1 -sFETCH_DEBUG -sGL_DEBUG -sFS_DEBUG -g4


```

--closure 1 与 ASYNCIFY_DEBUG 可能有冲突