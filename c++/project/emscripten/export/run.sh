# copyright daxiangyun, Inc. All Rights Reserved.



emcc ./ParameterInterface.cpp -o export.js -sMODULARIZE -sEXPORT_NAME='sme' -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,stringToNewUTF8,UTF8ToString -lembind


