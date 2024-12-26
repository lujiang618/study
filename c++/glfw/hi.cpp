#include <GLFW/glfw3.h>
#include <iostream>

// 键盘回调函数
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        std::cout << "Key pressed: " << key << " scan code: " << scancode << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        std::cout << "Key released: " << key << " scan code: " << scancode << std::endl;
    }
}

// 鼠标按键回调函数
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    std::cout << "action:" << action << std::endl;
    if (action == GLFW_PRESS)
    {
        std::cout << "Mouse button pressed: " << button << " mods:" << mods << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        std::cout << "Mouse button released: " << button << " mods:" << mods << std::endl;
    }

    if (button == 1)
    {
        glfwSetWindowSize(window, 1280, 720);
        glfwSetWindowTitle(window, "hi~");
        glfwSetWindowPos(window, 400, 400);
    }
}

// 鼠标移动回调函数
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << "Mouse moved to: (" << xpos << ", " << ypos << ")" << std::endl;
}

// 鼠标滚轮回调函数
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << "Mouse scrolled: (" << xoffset << ", " << yoffset << ")" << std::endl;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    if (yoffset > 0)
    {
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
    }
    else
    {
        glfwSetWindowMonitor(window, nullptr, 0, 0, 800, 600, GLFW_DONT_CARE);
    }
}

void sizeCallBack(GLFWwindow* window, int w, int h)
{
    std::cout << "-----------------------------------------\n";
    std::cout << "size:" << w << " " << h << std::endl;
}

void posCallback(GLFWwindow* window, int x, int y)
{
    std::cout << "-----------------------------------------\n";
    std::cout << "pos:" << x << " " << y << std::endl;
}

void focusCallback(GLFWwindow* window, int focused)
{
    std::cout << "focus:" << focused << std::endl;
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Mouse Event", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 设置键盘事件回调函数
    glfwSetKeyCallback(window, keyCallback);

    // 设置鼠标事件回调函数
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetWindowSizeCallback(window, sizeCallBack);
    glfwSetWindowPosCallback(window, posCallback);
    glfwSetWindowFocusCallback(window, focusCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents(); // 处理所有待处理的事件

        // glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
