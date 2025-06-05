

```
emcc hi.cpp -o hi.html
python3 -m http.server 9086  # browser access http://localhost:9086/hi.html

# 也可以通过emrun启动
emrun ./hi.html


rm -rf window.html window.js window.wasm && emcc window.cpp -s "EXPORTED_RUNTIME_METHODS=[]" -O3 -o test.html && python3 -m http.server 9086
rm -rf mouse.html mouse.js mouse.wasm && emcc mouse.cpp -s "EXPORTED_RUNTIME_METHODS=[]" -O3 -o test.html && python3 -m http.server 9086
rm -rf keyboard.html keyboard.js keyboard.wasm && emcc keyboard.cpp -s "EXPORTED_RUNTIME_METHODS=[]" -O3 -o test.html && python3 -m http.server 9086
rm -rf focus.html focus.js focus.wasm && emcc focus.cpp -s "EXPORTED_RUNTIME_METHODS=[]" -O3 -o test.html && python3 -m http.server 9086
rm -rf touch.html touch.js touch.wasm && emcc touch.cpp -s "EXPORTED_RUNTIME_METHODS=[]" -O3 -o test.html && python3 -m http.server 9086


```