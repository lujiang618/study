#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>
#include <iostream>
#include <map>

namespace ResponseMethod
{
static const std::string GET  = "GET";
static const std::string POST = "POST";
}; // namespace ResponseMethod

namespace ResponseType
{
static const std::string Text        = "text";
static const std::string ArrayBuffer = "arrayBuffer";
} // namespace ResponseType

struct GetFileDataOptions
{
    std::string path;
    std::string response_type = ResponseType::Text;
    // const char* method = "GET";
    std::string                        method = ResponseMethod::GET;
    std::map<const char*, const char*> headers;
    bool                               useCache = false;

    void SetHeader(const char* key, const char* value)
    {
        headers[key] = value;
    }
};

#ifdef __EMSCRIPTEN__
static emscripten::val RawWebFetchData(const GetFileDataOptions& req)
{
    using namespace emscripten;
    emscripten::val options = emscripten::val::object();
    options.set("method", req.method.c_str());
    if (req.headers.size() > 0)
    {
        emscripten::val headers = emscripten::val::object();

        for (const auto& [key, value] : req.headers)
        {
            headers.set(key, value);
        }
        options.set("headers", headers);
    }
    val fetch    = val::global("fetch");
    val response = fetch(req.path, options).await();
    val data     = response.call<val>(req.response_type.c_str()).await();
    return data;
}

static std::string FetchDataForWeb(const GetFileDataOptions& req)
{
    emscripten::val data = RawWebFetchData(req);
    return data.as<std::string>();
}

#endif

bool state = false;

void FetchOnWheel(int time)
{
    state = true;
    for (int i = 0; i < time; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();

        GetFileDataOptions req;
        // req.path = "http://xbasetest.dxbim.com:18898/viewer/model/odx?path=paas-storage/b1ba5eb6689b48ca8577d538e4902ce2/8ea6afd9b17d48b8865763ec7f626ab3/8ea6afd9b17d48b8865763ec7f626ab3.asm";
        // req.path = "http://192.168.100.106:8086/model/odx?path=O_D_X/advanced.rvt.odx/Scene/Scene.odx";
        req.path = "http://192.168.100.106:8086/model/odx?path=O_D_X/advanced.rvt.odx/Scene/LogicNode.odx";
        req.SetHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJiMWJhNWViNjY4OWI0OGNhODU3N2Q1MzhlNDkwMmNlMiIsImV4cCI6MTY2NjI0MzAyMSwic3ViIjoiMTU4In0.6PtmtvMI2mLrTqsQdv9s6GIZ2Erw64_G0LPJE3vUYnU");

        try
        {
            auto data = FetchDataForWeb(req); // 如果用了 .await，需要是 Asyncify 环境
            std::cout << "data size = " << data.size() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error in fetch: " << e.what() << std::endl;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Time for '" << i << " fetch::': " << duration.count() << "ms\n";
    }
    state = false;
}

int mouseWheelCallback(int eventType, const EmscriptenWheelEvent* wheelEvent, void* userData)
{
    // 打印滚动的偏移量
    std::cout << "Mouse wheel event: deltaX = " << wheelEvent->deltaX << ", deltaY = " << wheelEvent->deltaY << std::endl;

    // emscripten_async_call(FetchOnWheel, nullptr, 0); // 异步调用，不在回调中执行

    FetchOnWheel(5);

    return 0;
}

void Print()
{
    std::cout << "......................................" << std::endl;
}

void DoFetch(int times)
{
    if (state)
    {
        std::cout << "fetching" << std::endl;
        return;
    }
    FetchOnWheel(times);
    // this::thread::sleep_for(std::chrono::milliseconds(5));
}

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("DoFetch", &DoFetch);
    emscripten::function("Print", &Print);
}

int main()
{
    emscripten_set_canvas_element_size("#canvas", 1080, 720);

    emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, 1, mouseWheelCallback);

    return 0;
}