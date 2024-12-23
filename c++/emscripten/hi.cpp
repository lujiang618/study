#include <iostream>
#include <emscripten.h>
#include <emscripten/html5.h>

int keyEventCallback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN)
    {
        std::cout << "Key pressed: " << keyEvent->key << " " << keyEvent->keyCode << " charCode: " << keyEvent->charCode << "\n";
    }
    else if (eventType == EMSCRIPTEN_EVENT_KEYUP)
    {
        std::cout << "Key released: " << keyEvent->key << " " << keyEvent->keyCode << " charCode: " << keyEvent->charCode << "\n";
    }

    return 0;
}

int mouseEventCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
    {
        std::cout << "Mouse button " << mouseEvent->button << " pressed at (" << mouseEvent->clientX << ", " << mouseEvent->clientY << ")\n";
    }
    else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
    {
        std::cout << "Mouse button " << mouseEvent->button << " released at (" << mouseEvent->clientX << ", " << mouseEvent->clientY << ")\n";
    }
    else if (eventType == EMSCRIPTEN_EVENT_DBLCLICK)
    {
        std::cout << "Mouse double clicked " << mouseEvent->button << " at (" << mouseEvent->clientX << ", " << mouseEvent->clientY << ")\n";
    }
    else if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
    {
        std::cout << "Mouse moved to (" << mouseEvent->clientX << ", " << mouseEvent->clientY << ")\n";
        std::cout << "Mouse moved to (" << mouseEvent->targetX << ", " << mouseEvent->targetY << ")\n";
    }
    else if (eventType == EMSCRIPTEN_EVENT_CLICK)
    {
        std::cout << "Mouse clicked " << mouseEvent->button << " at (" << mouseEvent->clientX << ", " << mouseEvent->clientY << ")\n";
    }
    else
    {
        std::cout << "Mouse button " << eventType << " at (" << mouseEvent->clientX << ", " << mouseEvent->clientY << ")\n";
    }

    return 0;
}

int mouseWheelCallback(int eventType, const EmscriptenWheelEvent *wheelEvent, void *userData)
{
    // 打印滚动的偏移量
    std::cout << "Mouse wheel event: deltaX = " << wheelEvent->deltaX
              << ", deltaY = " << wheelEvent->deltaY << std::endl;

    // 返回 0 表示事件已处理
    return 0;
}

int main()
{
    emscripten_set_canvas_element_size("#canvas", 1080, 720);
    // 绑定键盘事件
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, keyEventCallback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, keyEventCallback);

    emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseWheelCallback);

    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);
    emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);
    emscripten_set_dblclick_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseEventCallback);

    std::cout << "Listening for keyboard events...\n";

    // 保持程序运行
    emscripten_exit_with_live_runtime();
    return 0;
}