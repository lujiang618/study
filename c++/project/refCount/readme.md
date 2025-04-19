launch.json
```
{
      "name": "test",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/c++/refCount/test",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${fileDirname}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ]
    }
```

compile
```
g++ -g3 test.cpp -o test -std=c++17 && ./test
```