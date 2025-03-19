#include <iostream>
#include <memory>

struct Event
{
    virtual ~Event() = default; // 虚析构函数，确保多态
};

struct MouseMoveEvent : public Event
{
    MouseMoveEvent(int x, int y)
        : x(x)
        , y(y)
    {
    }

    int x;

    int y;

    void handle() { std::cout << "MouseMoveEvent handled: " << x << " - " << y << std::endl; }
};

MouseMoveEvent* convertToMouseMoveEvent(std::unique_ptr<Event> event)
{
    // 尝试转换为派生类类型
    if (MouseMoveEvent* derived = dynamic_cast<MouseMoveEvent*>(event.get()))
    {
        event.release(); // 释放所有权，防止双重删除
        return derived;
    }

    return nullptr; // 转换失败返回空指针
}

int main()
{
    std::unique_ptr<Event> event = std::make_unique<MouseMoveEvent>(0, 0);

    MouseMoveEvent* mouseEvent = convertToMouseMoveEvent(std::move(event));

    if (mouseEvent)
    {
        mouseEvent->handle(); // 调用 MouseMoveEvent 的方法
    }
    else
    {
        std::cout << "Conversion failed" << std::endl;
    }

    return 0;
}
