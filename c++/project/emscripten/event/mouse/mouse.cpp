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

int mouseWheelCallback(int eventType, const EmscriptenWheelEvent* wheelEvent, void* userData)
{
    // 打印滚动的偏移量
    std::cout << "Mouse wheel event: deltaX = " << wheelEvent->deltaX
              << ", deltaY = " << wheelEvent->deltaY << std::endl;

    return 0;
}

int mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData)
{
    printf("%s, screen: (%d,%d), client: (%d,%d),%s%s%s%s button: %hu, buttons: %hu, movement: (%d,%d), canvas: (%d,%d), timestamp: %lf\n",
           emscripten_event_type_to_string(eventType), e->screenX, e->screenY, e->clientX, e->clientY,
           e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "",
           e->button, e->buttons, e->movementX, e->movementY, e->canvasX, e->canvasY,
           e->timestamp);

    return 0;
}

int main()
{
    emscripten_set_canvas_element_size("#canvas", 1080, 720);

    emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseWheelCallback);

    emscripten_set_mousemove_callback("#canvas", nullptr, 1, mouse_callback);
    emscripten_set_click_callback("#canvas", nullptr, 1, mouse_callback);
    emscripten_set_dblclick_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouse_callback);
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouse_callback);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouse_callback);

    EMSCRIPTEN_RESULT ret = emscripten_set_mouseenter_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
    TEST_RESULT(emscripten_set_mouseenter_callback);
    // 保持程序运行
    // emscripten_exit_with_live_runtime();
    return 0;
}