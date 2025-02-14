#include "VKBase.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

GLFWwindow*  pWindow;
GLFWmonitor* pMonitor;
const char*  windowTitle = "EasyVK";

auto PreInitialization_EnableSrgb()
{
    static bool enableSrgb; // Static object will be zero-initialized at launch
    enableSrgb = true;
    return [=]() -> bool
    { return enableSrgb; }; // 使用[=]捕获静态变量
}

auto PreInitialization_TrySetColorSpaceByOrder(arrayRef<const VkColorSpaceKHR> colorSpaces)
{
    static std::unique_ptr<VkColorSpaceKHR[]> pColorSpaces;
    pColorSpaces = std::make_unique<VkColorSpaceKHR[]>(colorSpaces.Count() + 1); // Value-initialization
    memcpy(pColorSpaces.get(), colorSpaces.Pointer(), sizeof(VkColorSpaceKHR) * colorSpaces.Count());

    return [pColorSpaces = std::move(pColorSpaces)]() -> const VkColorSpaceKHR*
    {
        return pColorSpaces.get(); // 返回指针
    };
}

bool InitializeWindow(VkExtent2D size, bool fullScreen = false, bool isResizable = true, bool limitFrameRate = true)
{
    using namespace vulkan;

    if (!glfwInit())
    {
        outStream << "[ InitializeWindow ] ERROR Failed to initialize GLFW!\n";
        return false;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, isResizable);
    pMonitor                 = glfwGetPrimaryMonitor();
    const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
    pWindow                  = fullScreen ? glfwCreateWindow(pMode->width, pMode->height, windowTitle, pMonitor, nullptr) : glfwCreateWindow(size.width, size.height, windowTitle, nullptr, nullptr);
    if (!pWindow)
    {
        outStream << "[ InitializeWindow ]\nFailed to create a glfw window!\n";
        glfwTerminate();
        return false;
    }

#ifdef _WIN32
    graphicsBase::Base().AddInstanceExtension(VK_KHR_SURFACE_EXTENSION_NAME);
    graphicsBase::Base().AddInstanceExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#else
    uint32_t     extensionCount = 0;
    const char** extensionNames;
    extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);
    if (!extensionNames)
    {
        outStream << "[ InitializeWindow ]\nVulkan is not available on this machine!\n";
        glfwTerminate();
        return false;
    }
    for (size_t i = 0; i < extensionCount; i++)
        graphicsBase::Base().AddInstanceExtension(extensionNames[i]);
#endif
    const VkColorSpaceKHR* pColorSpaces = PreInitialization_TrySetColorSpaceByOrder({})();

    if (pColorSpaces)
    {
        //  graphicsBase::Base().AddInstanceExtension(VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME);
    }
    graphicsBase::Base().AddDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    graphicsBase::Base().UseLatestApiVersion();
    if (graphicsBase::Base().CreateInstance())
        return false;

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (VkResult result = glfwCreateWindowSurface(vulkan::graphicsBase::Base().Instance(), pWindow, nullptr, &surface))
    {
        outStream << "[ InitializeWindow ] ERROR Failed to create a window surface! Error code: " << int32_t(result) << std::endl;
        glfwTerminate();
        return false;
    }
    graphicsBase::Base().Surface(surface);

    if (vulkan::graphicsBase::Base().GetPhysicalDevices() ||
        vulkan::graphicsBase::Base().DeterminePhysicalDevice(0, true, false) ||
        vulkan::graphicsBase::Base().CreateDevice())
        return false;

    if (graphicsBase::Base().GetSurfaceFormats())
        return false;
    if (pColorSpaces)
    {
        VkResult result_setColorSpace = VK_SUCCESS;
        while (*pColorSpaces)
        {
            result_setColorSpace = graphicsBase::Base().SetSurfaceFormat({VK_FORMAT_UNDEFINED, *pColorSpaces++});
            std::cout << "set color space:" << *pColorSpaces << " result: " << result_setColorSpace << std::endl;
            if (result_setColorSpace == VK_SUCCESS)
            {
                break;
            }
        }

        if (result_setColorSpace)
            outStream << "[ InitializeWindow ] WARNING Failed to satisfy the requirement of color space!\n";
    }

    auto srgbEnabled = PreInitialization_EnableSrgb();
    if (!graphicsBase::Base().SwapchainCreateInfo().imageFormat && srgbEnabled())
    {
        if (graphicsBase::Base().SetSurfaceFormat({VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}) &&
            graphicsBase::Base().SetSurfaceFormat({VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}))
        {
            outStream << "[ InitializeWindow ] WARNING Failed to enable sRGB!\n";
        }
    }

    if (graphicsBase::Base().CreateSwapchain(limitFrameRate))
        return false;

    return true;
}

void TerminateWindow()
{
    vulkan::graphicsBase::Base().WaitIdle();
    glfwTerminate();
}

void MakeWindowFullScreen()
{
    const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
    glfwSetWindowMonitor(pWindow, pMonitor, 0, 0, pMode->width, pMode->height, pMode->refreshRate);
}

void MakeWindowWindowed(VkOffset2D position, VkExtent2D size)
{
    const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
    glfwSetWindowMonitor(pWindow, nullptr, position.x, position.y, size.width, size.height, pMode->refreshRate);
}

void TitleFps()
{
    static double            time0 = glfwGetTime();
    static double            time1;
    static double            dt;
    static int               dframe = -1;
    static std::stringstream info;
    time1 = glfwGetTime();
    dframe++;
    if ((dt = time1 - time0) >= 1)
    {
        info.precision(1);
        info << windowTitle << "    " << std::fixed << dframe / dt << " FPS";
        glfwSetWindowTitle(pWindow, info.str().c_str());
        info.str("");
        time0  = time1;
        dframe = 0;
    }
}