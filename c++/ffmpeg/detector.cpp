// gpu_cap_check.cpp
#include <iostream>
#include <filesystem>
#include <dlfcn.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

// VAAPI 函数指针类型定义
typedef void* (*vaGetDisplayDRM_t)(int);
typedef int (*vaInitialize_t)(void*, int*, int*);
typedef int (*vaTerminate_t)(void*);
typedef const char* (*vaErrorStr_t)(int);

bool check_vaapi() {
    // 1. 检查 VAAPI 设备
    if (!std::filesystem::exists("/dev/dri/renderD128")) {
        cerr << "VAAPI: /dev/dri/renderD128 不存在，可能没有 GPU 或驱动未加载。\n";
        return false;
    }

    // 2. 动态加载 libva 库
    void* va_lib = dlopen("libva.so.2", RTLD_LAZY);
    if (!va_lib) {
        cerr << "VAAPI: 无法加载 libva 库: " << dlerror() << "\n";
        return false;
    }

    // 加载函数
    vaGetDisplayDRM_t vaGetDisplayDRM = (vaGetDisplayDRM_t)dlsym(va_lib, "vaGetDisplayDRM");
    vaInitialize_t vaInitialize = (vaInitialize_t)dlsym(va_lib, "vaInitialize");
    vaTerminate_t vaTerminate = (vaTerminate_t)dlsym(va_lib, "vaTerminate");
    vaErrorStr_t vaErrorStr = (vaErrorStr_t)dlsym(va_lib, "vaErrorStr");

    if (!vaGetDisplayDRM || !vaInitialize || !vaTerminate || !vaErrorStr) {
        cerr << "VAAPI: 无法加载所需函数\n";
        dlclose(va_lib);
        return false;
    }

    // 3. 尝试初始化 VAAPI
    int drm_fd = open("/dev/dri/renderD128", O_RDWR);
    if (drm_fd < 0) {
        cerr << "VAAPI: 无法打开设备文件\n";
        dlclose(va_lib);
        return false;
    }

    void* va_dpy = vaGetDisplayDRM(drm_fd);
    if (!va_dpy) {
        cerr << "VAAPI: vaGetDisplayDRM 失败。\n";
        close(drm_fd);
        dlclose(va_lib);
        return false;
    }

    int major_ver = 0, minor_ver = 0;
    int status = vaInitialize(va_dpy, &major_ver, &minor_ver);
    if (status != 0) {  // VA_STATUS_SUCCESS
        cerr << "VAAPI: 初始化失败\n";
        vaTerminate(va_dpy);
        close(drm_fd);
        dlclose(va_lib);
        return false;
    }

    cout << "VAAPI: 可用 (版本 " << major_ver << "." << minor_ver << ")\n";
    vaTerminate(va_dpy);
    close(drm_fd);
    dlclose(va_lib);
    return true;
}


bool check_vulkan_video() {
     VkInstance instance;
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Detector";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        cerr << "Vulkan: 创建实例失败。\n";
        return false;
    }

    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
    if (gpuCount == 0) {
        cerr << "Vulkan: 没有检测到 GPU。\n";
        vkDestroyInstance(instance, nullptr);
        return false;
    }

    vector<VkPhysicalDevice> gpus(gpuCount);
    vkEnumeratePhysicalDevices(instance, &gpuCount, gpus.data());

    bool hasVideo = false;
    for (auto &gpu : gpus) {
        uint32_t extCount = 0;
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extCount, nullptr);
        vector<VkExtensionProperties> exts(extCount);
        vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extCount, exts.data());

        for (auto &e : exts) {
            if (string(e.extensionName).find("video") != string::npos) {
                cout << "Vulkan: " << e.extensionName << " (revision " << e.specVersion << ")\n";
                hasVideo = true;
            }
        }
    }

    vkDestroyInstance(instance, nullptr);

    if (hasVideo) {
        cout << "Vulkan Video: ✅ 支持视频扩展\n";
        return true;
    } else {
        cout << "Vulkan Video: ❌ 未检测到视频扩展\n";
        return false;
    }
}

int main() {
    cout << "==== GPU Video Capability Check ====\n\n";

    bool vaapi_ok = check_vaapi();
    bool vulkan_ok = check_vulkan_video();

    cout << "\n==== 最终推荐方案 ====\n";
    if (vaapi_ok) {
        cout << "✅ 优先使用 VAAPI (硬件视频加速)\n";
    } else if (vulkan_ok) {
        cout << "✅ 退回使用 Vulkan Video (次优)\n";
    } else {
        cout << "⚠️ 无硬件加速支持，使用 CPU 软编解码。\n";
    }

    return 0;
}
