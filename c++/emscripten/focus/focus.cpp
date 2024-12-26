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

int focus_callback(int eventType, const EmscriptenFocusEvent* e, void* userData)
{
    printf("%s, nodeName: \"%s\", id: \"%s\"\n", emscripten_event_type_to_string(eventType), e->nodeName, e->id[0] == '\0' ? "(empty string)" : e->id);

    return 0;
}

int main()
{
    emscripten_set_canvas_element_size("#canvas", 1080, 720);

    EMSCRIPTEN_RESULT ret = emscripten_set_blur_callback("#canvas", 0, 1, focus_callback);
    TEST_RESULT(emscripten_set_blur_callback);
    ret = emscripten_set_focus_callback("#canvas", 0, 1, focus_callback);
    TEST_RESULT(emscripten_set_focus_callback);
    ret = emscripten_set_focusin_callback("#canvas", 0, 1, focus_callback);
    TEST_RESULT(emscripten_set_focusin_callback);
    ret = emscripten_set_focusout_callback("#canvas", 0, 1, focus_callback);
    TEST_RESULT(emscripten_set_focusout_callback);

    // 保持程序运行
    // emscripten_exit_with_live_runtime();
    return 0;
}