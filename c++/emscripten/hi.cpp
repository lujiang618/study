#include <iostream>
#include <emscripten.h>
#include <emscripten/html5.h>

int keyEventCallback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
        std::cout << "Key pressed: " << keyEvent->key << "\n";
    } else if (eventType == EMSCRIPTEN_EVENT_KEYUP) {
        std::cout << "Key released: " << keyEvent->key << "\n";
    }

    return 0; 
}

int mouseEventCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
        std::cout << "Mouse button pressed at (" << mouseEvent->canvasX << ", " << mouseEvent->canvasY << ")\n";
    } else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) {
        std::cout << "Mouse button released at (" << mouseEvent->canvasX << ", " << mouseEvent->canvasY << ")\n";
    } else {
        std::cout << "Mouse button"<< eventType <<" at (" << mouseEvent->canvasX << ", " << mouseEvent->canvasY << ")\n";
    }

    return 0; 
}

int main() {
    // 绑定键盘事件
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, keyEventCallback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, keyEventCallback);

    emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);
    emscripten_set_dblclick_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);

    std::cout << "Listening for keyboard events...\n";

    // 保持程序运行
    emscripten_exit_with_live_runtime();
    return 0;
}