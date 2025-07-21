// Copyright 2012 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <assert.h>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>
#include <iostream>
#include <map>
#include "featch.h"

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void init()
    {
        const char* ready_msg = "ready";
        emscripten_worker_respond_provisionally((char*)ready_msg, strlen(ready_msg));
    }

    EMSCRIPTEN_KEEPALIVE
    void fetchData(char* data, int size)
    {
        fetchWebData();

        emscripten_worker_respond(nullptr, 0);
    }
}
