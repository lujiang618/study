
#include "TextureMgr.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace core;

int main()
{

    TextureMgr mgr;

    auto h1 = mgr.CreateTexture("tex1");
    std::cout << "create 1 --> size : " << mgr.Size() << std::endl;
    std::cout << "create 1 --> " << h1->GetName() << ": " << h1->UseCount() << std::endl;

    // auto h3 = nullptr;
    // auto h3 = mgr.GetTexture("tex1");

    // if (h3 == nullptr)
    // {
    //     std::cout << "get 3 error--> "  << h1->UseCount() << std::endl;
    // }
    // std::cout << "get 3       size : " << mgr.Size() << std::endl;
    // std::cout << "get 3    --> " << h1->GetName() << ": " << h1->UseCount() << std::endl;

    // auto h4 = h1;
    // std::cout << "left 4   --> " << "size : " << mgr.Size() << std::endl;
    // std::cout << "left 4   --> " << h4->GetName() << ": " << h4->UseCount() << std::endl;

    // h3 = nullptr;
    // std::cout << "delete 3 --> " << "size : " << mgr.Size() << std::endl;
    // std::cout << "delete 3 --> " << h1->GetName() << ": " << h1->UseCount() << std::endl;

    // auto h2 = mgr.CreateTexture("tex2");
    // std::cout << "create 2 --> " << h2->GetName() << ": " << h2->UseCount() << std::endl;
    // std::cout << "create 2 --> size : " << mgr.Size() << std::endl;
}