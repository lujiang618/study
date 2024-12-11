#include <GLFW/glfw3.h>
#include <iostream>

// 键盘回调函数
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        std::cout << "Key pressed: " << key << std::endl;
    } else if (action == GLFW_RELEASE) {
        std::cout << "Key released: " << key << std::endl;
    }
}

// 鼠标按键回调函数
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        std::cout << "Mouse button pressed: " << button << std::endl;
    } else if (action == GLFW_RELEASE) {
        std::cout << "Mouse button released: " << button << std::endl;
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
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Mouse Event", nullptr, nullptr);
    if (!window) {
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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();  // 处理所有待处理的事件

        // glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
