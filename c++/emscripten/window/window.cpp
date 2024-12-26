#include <emscripten.h>
#include <emscripten/html5.h>
#include <iostream>

static inline const char* emscripten_event_type_to_string(int eventType)
{
    const char* events[] = {"(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize",
                            "scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange",
                            "visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload",
                            "batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "mouseenter", "mouseleave", "mouseover", "mouseout", "(invalid)"};
    ++eventType;
    if (eventType < 0)
        eventType = 0;
    if (eventType >= sizeof(events) / sizeof(events[0]))
        eventType = sizeof(events) / sizeof(events[0]) - 1;
    return events[eventType];
}

const char* emscripten_result_to_string(EMSCRIPTEN_RESULT result)
{
    if (result == EMSCRIPTEN_RESULT_SUCCESS)
        return "EMSCRIPTEN_RESULT_SUCCESS";
    if (result == EMSCRIPTEN_RESULT_DEFERRED)
        return "EMSCRIPTEN_RESULT_DEFERRED";
    if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED)
        return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
    if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED)
        return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
    if (result == EMSCRIPTEN_RESULT_INVALID_TARGET)
        return "EMSCRIPTEN_RESULT_INVALID_TARGET";
    if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET)
        return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
    if (result == EMSCRIPTEN_RESULT_INVALID_PARAM)
        return "EMSCRIPTEN_RESULT_INVALID_PARAM";
    if (result == EMSCRIPTEN_RESULT_FAILED)
        return "EMSCRIPTEN_RESULT_FAILED";
    if (result == EMSCRIPTEN_RESULT_NO_DATA)
        return "EMSCRIPTEN_RESULT_NO_DATA";
    return "Unknown EMSCRIPTEN_RESULT!";
}

#define TEST_RESULT(x)                    \
    if (ret != EMSCRIPTEN_RESULT_SUCCESS) \
        printf("%s returned %s.\n", #x, emscripten_result_to_string(ret));

int keyEventCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData)
{
    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN)
    {
        std::cout << "Key pressed: " << keyEvent->key << " " << keyEvent->keyCode << " charCode: " << keyEvent->charCode << "\n";
    }
    else if (eventType == EMSCRIPTEN_EVENT_KEYUP)
    {
        std::cout << "Key released: " << keyEvent->key << " " << keyEvent->keyCode << " charCode: " << keyEvent->charCode << "\n";
    }

    if (keyEvent->keyCode == 87)
    {
        EMSCRIPTEN_RESULT ret = emscripten_request_fullscreen("#canvas", 1);
        TEST_RESULT(emscripten_request_fullscreen);
    }
    if (keyEvent->keyCode == 81)
    {
        EMSCRIPTEN_RESULT ret = emscripten_exit_fullscreen();
        TEST_RESULT(emscripten_exit_fullscreen);
    }

    if (keyEvent->keyCode == 83)
    {
        emscripten_set_canvas_element_size("#canvas", 720, 480);
        emscripten_set_window_title("hi");

        EM_ASM({
                var canvas = document.getElementById('canvas');
                canvas.style.position = 'relative';  // 确保可以自由定位
                canvas.style.left = $0 + 'px';
                canvas.style.top = $1 + 'px'; }, -400, 0);
    }

    return 0;
}

// 全屏回调函数（可选）
int fullscreen_change_callback(int eventType, const EmscriptenFullscreenChangeEvent* event, void* userData)
{
    if (event->isFullscreen)
    {
        printf("Canvas is now in fullscreen mode.\n");
        printf("w %d %d h %d %d\n", event->screenWidth, event->elementWidth, event->screenHeight, event->elementHeight);
    }
    else
    {
        printf("Canvas exited fullscreen mode.\n");
    }
    return 0;
}

int uieventCallback(int eventType, const EmscriptenUiEvent* e, void* userData)
{
    printf("%s, detail: %d, document.body.client size: (%d,%d), window.inner size: (%d,%d), scrollPos: (%d, %d)\n",
           emscripten_event_type_to_string(eventType), e->detail, e->documentBodyClientWidth, e->documentBodyClientHeight,
           e->windowInnerWidth, e->windowInnerHeight, e->scrollTop, e->scrollLeft);

    return 0;
}

int main()
{
    emscripten_set_canvas_element_size("#canvas", 1080, 720);
    // 绑定键盘事件
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, keyEventCallback);

    EMSCRIPTEN_RESULT ret = emscripten_set_resize_callback("#canvas", nullptr, 1, uieventCallback);
    TEST_RESULT(emscripten_set_resize_callback);

    emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, EM_TRUE, fullscreen_change_callback);
    std::cout << "Listening for keyboard events...\n";

    // 保持程序运行
    // emscripten_exit_with_live_runtime();
    return 0;
}