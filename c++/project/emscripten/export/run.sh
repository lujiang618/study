# copyright daxiangyun, Inc. All Rights Reserved.



emcc ./ParameterInterface.cpp -o export.js -sMODULARIZE -sEXPORT_NAME='createMyModule' -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,stringToNewUTF8,UTF8ToString --shell-file ./shell.html -lembind


