#include "memory.h"

#include <thread>
#include <chrono>

// 跟踪内存分配
void VulkanMemoryQuery::trackMemoryAllocation(VkDeviceSize size, VkDeviceMemory memory) {
    if (!enableMemoryTracking) return;

    totalAllocatedVRAM += size;
    allocatedMemoryMap[memory] = size;

    std::cout << "[Memory Track] Allocated " << formatMemorySize(size)
              << " (Total: " << formatMemorySize(totalAllocatedVRAM) << ")" << std::endl;
}

// 跟踪内存释放
void VulkanMemoryQuery::trackMemoryFree(VkDeviceMemory memory) {
    if (!enableMemoryTracking) return;

    auto it = allocatedMemoryMap.find(memory);
    if (it != allocatedMemoryMap.end()) {
        VkDeviceSize freedSize = it->second;
        totalAllocatedVRAM -= freedSize;
        allocatedMemoryMap.erase(it);

        std::cout << "[Memory Track] Freed " << formatMemorySize(freedSize)
                  << " (Total: " << formatMemorySize(totalAllocatedVRAM) << ")" << std::endl;
    }
}

bool VulkanMemoryQuery::initialize() {
    // 1. 创建 Vulkan 实例
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Memory Query Tool";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    const char* extensions[] = {
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
    };

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = 1;
    instanceInfo.ppEnabledExtensionNames = extensions;

    VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &instance));

    // 2. 加载扩展函数
    vkGetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2)
        vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties2");

    // 3. 选择物理设备
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        std::cerr << "No Vulkan devices found!" << std::endl;
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    physicalDevice = devices[0];

    // 4. 创建设备
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = 0;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;

    VK_CHECK(vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device));

    // 5. 启用内存跟踪
    enableMemoryTracking = true;

    std::cout << "Vulkan initialized successfully!" << std::endl;
    return true;
}

void VulkanMemoryQuery::queryMemoryInfo() {
    if (!physicalDevice) {
        std::cerr << "Physical device not initialized!" << std::endl;
        return;
    }

    std::cout << "\n=== Vulkan Device Memory Information ===" << std::endl;

    // 1. 设备基本信息
    VkPhysicalDeviceProperties deviceProps;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProps);

    std::cout << "Device: " << deviceProps.deviceName << std::endl;
    std::cout << "Vendor: " << getVendorName(deviceProps.vendorID) << std::endl;

    // 2. 标准内存属性
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    std::cout << "\n--- Memory Configuration ---" << std::endl;
    std::cout << "Memory Heaps: " << memProperties.memoryHeapCount << std::endl;
    std::cout << "Memory Types: " << memProperties.memoryTypeCount << std::endl;

    // 3. 详细内存统计
    printDetailedMemoryStatistics();
}

// 新增：详细内存统计函数
void VulkanMemoryQuery::printDetailedMemoryStatistics() {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    // 方法1: 使用内存预算扩展（最准确）
    if (vkGetPhysicalDeviceMemoryProperties2) {
        VkPhysicalDeviceMemoryBudgetPropertiesEXT budgetProps = {};
        budgetProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT;

        VkPhysicalDeviceMemoryProperties2 memProps2 = {};
        memProps2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
        memProps2.pNext = &budgetProps;

        vkGetPhysicalDeviceMemoryProperties2(physicalDevice, &memProps2);

        std::cout << "\n--- Real-time Memory Usage (VK_EXT_memory_budget) ---" << std::endl;

        VkDeviceSize totalVRAM = 0;
        VkDeviceSize totalUsed = 0;
        VkDeviceSize totalAvailable = 0;

        for (uint32_t i = 0; i < memProperties.memoryHeapCount; i++) {
            const VkMemoryHeap& heap = memProperties.memoryHeaps[i];

            if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                VkDeviceSize heapSize = heap.size;
                VkDeviceSize heapBudget = budgetProps.heapBudget[i];
                VkDeviceSize heapUsage = budgetProps.heapUsage[i];
                VkDeviceSize heapAvailable = (heapBudget > heapUsage) ? (heapBudget - heapUsage) : 0;

                totalVRAM += heapSize;
                totalUsed += heapUsage;
                totalAvailable += heapAvailable;

                std::cout << "Heap " << i << ":" << std::endl;
                std::cout << "  Total Size:    " << formatMemorySize(heapSize) << std::endl;
                std::cout << "  Current Budget: " << formatMemorySize(heapBudget) << std::endl;
                std::cout << "  Current Usage: " << formatMemorySize(heapUsage);

                if (heapBudget > 0) {
                    double usagePercent = (double)heapUsage / heapBudget * 100.0;
                    std::cout << " (" << usagePercent << "%)" << std::endl;
                } else {
                    std::cout << std::endl;
                }

                std::cout << "  Available:     " << formatMemorySize(heapAvailable) << std::endl;

                // 显示使用趋势
                if (heapUsage > 0) {
                    std::cout << "  Usage Trend:   ";
                    int bars = static_cast<int>((double)heapUsage / heapBudget * 50.0);
                    for (int j = 0; j < bars; j++) std::cout << "█";
                    for (int j = bars; j < 50; j++) std::cout << "░";
                    std::cout << std::endl;
                }
            }
        }

        // 总体统计
        std::cout << "\n--- Overall VRAM Statistics ---" << std::endl;
        std::cout << "Total VRAM:        " << formatMemorySize(totalVRAM) << std::endl;
        std::cout << "Currently Used:    " << formatMemorySize(totalUsed);
        if (totalVRAM > 0) {
            double totalUsagePercent = (double)totalUsed / totalVRAM * 100.0;
            std::cout << " (" << totalUsagePercent << "%)" << std::endl;
        } else {
            std::cout << std::endl;
        }
        std::cout << "Currently Free:    " << formatMemorySize(totalAvailable) << std::endl;

        // 碎片化估计
        if (totalAvailable > 0 && totalVRAM > 0) {
            double fragmentation = (1.0 - (double)totalAvailable / totalVRAM) * 100.0;
            std::cout << "Fragmentation:     " << fragmentation << "%" << std::endl;
        }
    }

    // 方法2: 应用程序跟踪的分配（仅统计本程序分配的内存）
    std::cout << "\n--- Application-tracked Allocations ---" << std::endl;
    std::cout << "This Program's VRAM Usage: " << formatMemorySize(totalAllocatedVRAM) << std::endl;
    std::cout << "Active Allocations: " << allocatedMemoryMap.size() << " blocks" << std::endl;

    // 显示分配详情
    if (!allocatedMemoryMap.empty()) {
        std::cout << "Allocation Details:" << std::endl;
        for (const auto& [memory, size] : allocatedMemoryMap) {
            std::cout << "  Block " << std::hex << (uint64_t)memory << std::dec
                      << ": " << formatMemorySize(size) << std::endl;
        }
    }

    // 方法3: 估算系统总使用量（如果没有扩展）
    if (!vkGetPhysicalDeviceMemoryProperties2) {
        std::cout << "\n--- Estimated System-wide Usage ---" << std::endl;
        std::cout << "Memory budget extension not available." << std::endl;
        std::cout << "Cannot determine real-time system-wide usage." << std::endl;

        // 显示总VRAM大小
        VkDeviceSize totalVRAM = 0;
        for (uint32_t i = 0; i < memProperties.memoryHeapCount; i++) {
            if (memProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                totalVRAM += memProperties.memoryHeaps[i].size;
            }
        }
        std::cout << "Total VRAM: " << formatMemorySize(totalVRAM) << std::endl;
        std::cout << "This Program's Usage: " << formatMemorySize(totalAllocatedVRAM) << std::endl;
    }
}

void VulkanMemoryQuery::createTestAllocations() {
    std::cout << "\n--- Creating Test Allocations ---" << std::endl;

    // 创建几个测试内存分配来演示跟踪功能
    const int numAllocations = 3;
    const VkDeviceSize sizes[] = {1024 * 1024, 2 * 1024 * 1024, 512 * 1024}; // 1MB, 2MB, 0.5MB

    std::vector<VkDeviceMemory> testAllocations;

    for (int i = 0; i < numAllocations; i++) {
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = sizes[i];

        // 使用设备本地内存（VRAM）
        allocInfo.memoryTypeIndex = 0; // 通常第一个类型是设备本地内存

        VkDeviceMemory memory;
        VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &memory);

        if (result == VK_SUCCESS) {
            testAllocations.push_back(memory);
            trackMemoryAllocation(sizes[i], memory);
            std::cout << "Created test allocation " << i << ": " << formatMemorySize(sizes[i]) << std::endl;
        } else {
            std::cout << "Failed to create test allocation " << i << std::endl;
        }
    }

    // 等待一段时间后释放
    std::cout << "Waiting 2 seconds before freeing test allocations..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 释放测试分配
    for (VkDeviceMemory memory : testAllocations) {
        trackMemoryFree(memory);
        vkFreeMemory(device, memory, nullptr);
    }

    std::cout << "Test allocations completed." << std::endl;
}

const char* VulkanMemoryQuery::getVendorName(uint32_t vendorID) {
    switch (vendorID) {
        case 0x1002: return "AMD";
        case 0x10DE: return "NVIDIA";
        case 0x8086: return "Intel";
        case 0x13B5: return "ARM";
        case 0x5143: return "Qualcomm";
        default: return "Unknown";
    }
}

std::string VulkanMemoryQuery::formatMemorySize(VkDeviceSize size) {
    const char* units[] = {"B", "KB", "MB"};
    int unitIndex = 0;
    double sizeValue = static_cast<double>(size);

    while (sizeValue >= 1024.0 && unitIndex < 2) {
        sizeValue /= 1024.0;
        unitIndex++;
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.2f %s", sizeValue, units[unitIndex]);
    return std::string(buffer);
}

std::string VulkanMemoryQuery::getHeapFlagsString(VkMemoryHeapFlags flags) {
    std::string result;
    if (flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) result += "DEVICE_LOCAL ";
    if (flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT) result += "MULTI_INSTANCE ";
    if (result.empty()) result = "NONE";
    return result;
}

std::string VulkanMemoryQuery::getMemoryPropertyFlagsString(VkMemoryPropertyFlags flags) {
    std::string result;
    if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) result += "DEVICE_LOCAL ";
    if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) result += "HOST_VISIBLE ";
    if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) result += "HOST_COHERENT ";
    if (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) result += "HOST_CACHED ";
    if (flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) result += "LAZILY_ALLOCATED ";
    if (result.empty()) result = "NONE";
    return result;
}

void VulkanMemoryQuery::cleanup() {
    if (device != VK_NULL_HANDLE) {
        vkDestroyDevice(device, nullptr);
        device = VK_NULL_HANDLE;
    }
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
    }
}