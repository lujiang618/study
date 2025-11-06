#pragma once

#include <cstdint>
#include <string>
#include <vulkan/vulkan.h>

// 获取厂商名称
const char* getVendorName(uint32_t vendorID) {
    switch (vendorID) {
        case 0x1002: return "AMD";
        case 0x10DE: return "NVIDIA";
        case 0x8086: return "Intel";
        case 0x13B5: return "ARM";
        case 0x5143: return "Qualcomm";
        default: return "Unknown";
    }
}


// 格式化内存大小
std::string formatMemorySize(VkDeviceSize size) {
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

// 获取堆标志字符串
std::string getHeapFlagsString(VkMemoryHeapFlags flags) {
    std::string result;
    if (flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) result += "DEVICE_LOCAL ";
    if (flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT) result += "MULTI_INSTANCE ";
    if (result.empty()) result = "NONE";
    return result;
}

// 获取内存属性标志字符串
std::string getMemoryPropertyFlagsString(VkMemoryPropertyFlags flags) {
    std::string result;
    if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) result += "DEVICE_LOCAL ";
    if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) result += "HOST_VISIBLE ";
    if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) result += "HOST_COHERENT ";
    if (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) result += "HOST_CACHED ";
    if (flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) result += "LAZILY_ALLOCATED ";
    if (result.empty()) result = "NONE";
    return result;
}