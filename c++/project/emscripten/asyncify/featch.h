#include <assert.h>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>
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
    try
    {
        val options = val::object();
        options.set("method", req.method.c_str());

        if (!req.headers.empty())
        {
            val headers = val::object();
            for (const auto& [key, value] : req.headers)
            {
                headers.set(key, value);
            }
            options.set("headers", headers);
        }

        val fetch    = val::global("fetch");
        val response = fetch(req.path, options).await();

        // 检查响应状态
        if (response.isUndefined())
        {
            throw std::runtime_error("Fetch returned undefined response");
        }

        int status = response["status"].as<int>();
        if (status < 200 || status >= 300)
        {
            throw std::runtime_error("HTTP error: " + std::to_string(status));
        }

        return response.call<val>(req.response_type.c_str()).await();
    }
    catch (const emscripten::val& jsErr)
    {
        // 捕获 JavaScript 异常
        std::string errMsg = jsErr["message"].as<std::string>();
        throw std::runtime_error("JS exception: " + errMsg);
    }
}

static std::string FetchDataForWeb(const GetFileDataOptions& req)
{
    emscripten::val data = RawWebFetchData(req);
    return data.as<std::string>();
}

#endif

void fetchWebData()
{
    for (int i = 0; i < 50; i++)
    {

        auto start_time = std::chrono::high_resolution_clock::now();

        GetFileDataOptions req;
        req.path = "http://192.168.100.106:8086/model/odx?path=O_D_X/advanced.rvt.odx/Scene/LogicNode.odx";
        req.SetHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJiMWJhNWViNjY4OWI0OGNhODU3N2Q1MzhlNDkwMmNlMiIsImV4cCI6MTY2NjI0MzAyMSwic3ViIjoiMTU4In0.6PtmtvMI2mLrTqsQdv9s6GIZ2Erw64_G0LPJE3vUYnU");

        try
        {
            auto data = FetchDataForWeb(req); // 如果用了 .await，需要是 Asyncify 环境
            // std::cout << "data size = " << data.size() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error in fetch: " << e.what() << std::endl;
        }

        emscripten_sleep(10);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << i << " fetch::': " << duration.count() << "ms\n";

    }
}