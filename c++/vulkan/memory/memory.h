#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <atomic>
#include <chrono>

#define VK_CHECK(result) \
    if (result != VK_SUCCESS) { \
        std::cerr << "Vulkan error at " << __FILE__ << ":" << __LINE__ << ": " << result << std::endl; \
        std::exit(-1); \
    }

class VulkanMemoryQuery {
private:
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    // 扩展函数指针
    PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2 = nullptr;

    // 内存统计相关
    std::atomic<VkDeviceSize> totalAllocatedVRAM{0};
    std::map<VkDeviceMemory, VkDeviceSize> allocatedMemoryMap; // 跟踪分配的内存块
    bool enableMemoryTracking = false;

public:
    VulkanMemoryQuery() = default;
    ~VulkanMemoryQuery() { cleanup(); }

    bool initialize();
    void queryMemoryInfo();
    void createTestAllocations(); // 创建测试分配以演示统计
    void cleanup();

    // 内存跟踪函数
    void enableDetailedTracking(bool enable) { enableMemoryTracking = enable; }
    VkDeviceSize getTrackedVRAMUsage() const { return totalAllocatedVRAM; }

private:
    // 辅助函数
    const char* getVendorName(uint32_t vendorID);
    std::string formatMemorySize(VkDeviceSize size);
    std::string getHeapFlagsString(VkMemoryHeapFlags flags);
    std::string getMemoryPropertyFlagsString(VkMemoryPropertyFlags flags);

    // 增强的内存统计函数
    void printDetailedMemoryStatistics();
    void trackMemoryAllocation(VkDeviceSize size, VkDeviceMemory memory);
    void trackMemoryFree(VkDeviceMemory memory);
};