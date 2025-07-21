// Copyright 2012 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <thread>
#include <iostream>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>

#include "featch.h"


void Init()
{

}

bool doFetch = false;

void Print()
{
    doFetch = true;

    std::cout << "......................................" << std::endl;
}

void DoFetch()
{
    fetchWebData();
}

void Update()
{

}

int main()
{
    emscripten_set_canvas_element_size("#canvas", 1080, 720);

    emscripten_set_main_loop([]()
    {

        if (doFetch)
        {
            DoFetch();
            doFetch = false;
        }

    }, 60, 1);

    return 0;
}

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("Init", &Init);
    emscripten::function("Print", &Print);
    emscripten::function("Update", &Update);
}