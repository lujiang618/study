// Copyright 2012 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <stdio.h>
#include <assert.h>
#include <atomic>
#include <thread>
#include <iostream>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>

#include "featch.h"

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

int w1;

void c1(char* data, int size, void* arg)
{
    assert((long)arg == 93);
    int* x = (int*)data;

    // 销毁 Worker 资源
    if (w1 != 0)
    {
        printf("Destroying worker %d\n", w1);
        emscripten_destroy_worker(w1);
        w1 = 0; // 重置为无效值
    }
}

void Init()
{
    w1 = emscripten_create_worker("worker.js");

    emscripten_call_worker(w1, "init", nullptr, 0, worker_ready_callback, nullptr);
}

void Print()
{
    std::cout << "......................................" << std::endl;
}

void DoFetch()
{
    fetchWebData();
}

void DoWorkerFetch()
{
    emscripten_call_worker(w1, "fetchData", nullptr, 0, c1, (void*)93);
}

bool useWorker = false;

int i = 0;
int main()
{
    emscripten_set_canvas_element_size("#canvas", 1080, 720);

    Init();

    emscripten_set_main_loop([]()
    {
        if (!worker_ready.load())
        {
            return;
        }

        if (i % 10 == 0)
        {
            emscripten_call_worker(w1, "fetchData", nullptr, 0, c1, (void*)93);
        }
        i++;
    }, 60, 1);
    return 0;
}

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("Print", &Print);
    emscripten::function("DoFetch", &DoFetch);
    emscripten::function("DoWorkerFetch", &DoWorkerFetch);

    emscripten::function("Init", &Init);
}