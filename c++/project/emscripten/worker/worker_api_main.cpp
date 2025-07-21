// Copyright 2012 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <stdio.h>
#include <assert.h>
#include <atomic>
#include <thread>
#include <emscripten.h>
#include <emscripten/html5.h>

static bool reported = false;

void _ReportResult(int result)
{
    if (reported)
    {
        printf("ERROR: result already reported\n");
        exit(1);
    }
    reported = true;
    printf("RESULT: %d\n", result);
}

#define REPORT_RESULT(result) _ReportResult((result))

std::atomic<bool> worker_ready(false);

extern "C"
{
    void worker_ready_callback(char* data, int size, void* arg)
    {
        std::string msg(data, size);
        if (msg == "ready")
        {
            worker_ready.store(true);
            printf("Worker is ready\n");
        }
        else
        {
            printf("Unexpected message from worker: %s\n", msg.c_str());
        }
    }
}

// void print_memory_usage()
// {
//     printf("内存使用: %d / %d bytes\n",
//            emscripten_run_script_int("HEAPU8.length"),
//            emscripten_run_script_int("Module.asm.memory.buffer.byteLength"));
// }

int w1;

void c1(char* data, int size, void* arg)
{
    printf("............................................................\n");

    assert((long)arg == 93);
    int* x = (int*)data;

    printf("c1: %d, %d\n", x[0], x[1]);

    int result = x[1] % x[0];
    printf("%d / %d = %d\n", x[1], x[0], result);
    REPORT_RESULT(result);

    // 销毁 Worker 资源
    if (w1 != 0)
    {
        printf("Destroying worker %d\n", w1);
        emscripten_destroy_worker(w1);
        w1 = 0; // 重置为无效值
    }

    // // 等待所有异步操作完成
    // printf("Waiting for async operations to complete...\n");
    // emscripten_runtime_keepalive_push();
    // emscripten_runtime_keepalive_pop();

    // // 强制退出运行时
    // printf("Program completed. Forcing exit with status 0.\n");
    // emscripten_force_exit(0);
}

int main()
{
    // emscripten_set_canvas_element_size("#canvas", 1080, 720);

    w1 = emscripten_create_worker("worker.js");

    emscripten_call_worker(w1, "init", nullptr, 0, worker_ready_callback, nullptr);

    // // 等待 Worker 初始化完成
    // while (!worker_ready.load())
    // {
    //     emscripten_sleep(100);
    // }

    // int x[2] = {100, 6002};
    // emscripten_call_worker(w1, "one", (char*)x, sizeof(x), c1, (void*)93);

    emscripten_set_main_loop([]()
    {
        if (!worker_ready.load())
            return;

        int x[2] = {100, 6002};
        emscripten_call_worker(w1, "one", (char*)x, sizeof(x), c1, (void*)93);

        // print_memory_usage();

        emscripten_cancel_main_loop();
    }, 60, 1);

    return 0;
}
