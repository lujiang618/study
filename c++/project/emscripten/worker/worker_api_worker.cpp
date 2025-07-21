// Copyright 2012 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <assert.h>
#include <emscripten.h>
#include <iostream>

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void init()
    {
          const char* ready_msg = "ready";
          emscripten_worker_respond_provisionally((char*)ready_msg, strlen(ready_msg));
    }

    void one(char* data, int size)
    {
        int* x   = (int*)data;
        int  num = size / sizeof(int);
        for (int i = 0; i < num; i++)
        {
            x[i] += 1234;
        }

        std::cout << "one size: " << size << std::endl;

        emscripten_worker_respond(data, size);
    }
}
