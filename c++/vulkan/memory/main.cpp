#include "memory.h"

#include <iostream>

int main() {
    std::cout << "Enhanced Vulkan Memory Query Tool" << std::endl;
    std::cout << "=================================" << std::endl;

    VulkanMemoryQuery query;

    try {
        if (query.initialize()) {
            // 第一次查询（初始状态）
            query.queryMemoryInfo();

            // 创建测试分配并再次查询
            // query.createTestAllocations();

            // // 第二次查询（分配后状态）
            // std::cout << "\n\n=== After Test Allocations ===" << std::endl;
            // query.queryMemoryInfo();

        } else {
            std::cerr << "Failed to initialize Vulkan!" << std::endl;
            return -1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    // std::cout << "\nQuery completed. Press Enter to exit...";
    // std::cin.get();

    return 0;
}