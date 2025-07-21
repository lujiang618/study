


```
emcc mouse.cpp -o hi.html -s ASYNCIFY=1 --bind -s NO_DISABLE_EXCEPTION_CATCHING -s ENVIRONMENT=web -sALLOW_MEMORY_GROWTH=1 -sFETCH_DEBUG -sGL_DEBUG -sFS_DEBUG -g4

python3 -m http.server 1092
```