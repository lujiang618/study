#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

class A
{
public:
    A()
    {
        a = new int;
        a = 10;
        std::function<void(void*,int, int)> f = std::bind(&A::func,this, a, std::repleacehold::_1, std::repleacehold::_2);
        xxx.add(1, f);
    }

    void func(void* data, int i, int j)
    {

    }

    int * a = nullptr;
};

xxx.triggerCallbacks(1, 50,50);
// 一个示例回调函数，接收一个整数
void callback1(int a, double b)
{
    std::cout << "Callback 1, received: " << a << " " << b << std::endl;
}

// 一个示例回调函数，接收一个字符串
void callback2(const std::string& str)
{
    std::cout << "Callback 2, received: " << str << std::endl;
}

// 一个示例回调函数，接收一个整数和一个字符串
void callback3(int a, const std::string str)
{
    std::cout << "Callback 3, received: " << a << " and " << str << std::endl;
}

// 基础事件类
class IEventArg
{
public:
    virtual ~IEventArg() = default;
};

template <typename... Arguments>
class EventArg : public IEventArg
{
public:
    EventArg(Arguments&&... args)
        : args_(std::forward<Arguments>(args)...)
    {
    }

    ~EventArg() = default;

    std::tuple<Arguments...> Args() const
    {
        return args_;
    }

private:
    std::tuple<Arguments...> args_;
};

// 定义回调函数的类型，回调函数可以接受任意数量的参数
using EventCallback = std::function<void(const IEventArg&)>;

class CallbackManager
{
public:
    // 修改后的addCallback，接受std::function类型的回调
    template <typename... Arguments>
    void addCallback(std::function<void(Arguments...)> callback)
    {
        EventCallback wrappedCallback = [callback](const IEventArg& e)
        {
            try
            {
                const auto& event = dynamic_cast<const EventArg<Arguments...>&>(e);
                std::apply(callback, event.Args());
            }
            catch (const std::bad_cast& e)
            {
                std::cout << "Error: Invalid event type" << std::endl;
            }
        };

        callbacks.push_back(wrappedCallback);
    }

    // 触发所有回调函数，传入参数包
    template <typename... Arguments>
    void triggerCallbacks(Arguments&&... args)
    {

        (std::cout << ... << args) << std::endl;

        std::shared_ptr<IEventArg> event = std::make_shared<EventArg<Arguments...>>(std::forward<Arguments>(args)...);

        // 遍历所有回调函数并触发它们
        for (auto& callback : callbacks)
        {
            // 触发回调函数时传入参数
            callback(*event);
        }
    }

private:
    // 存储所有的回调函数
    std::vector<EventCallback> callbacks;
};

int main()
{
    CallbackManager manager;

    // 使用std::function将普通的回调函数传递给addCallback
    manager.addCallback(std::function<void(int, double)>(callback1));            // 注册接收一个整数的回调函数
    manager.addCallback(std::function<void(const std::string)>(callback2));      // 注册接收一个字符串的回调函数
    manager.addCallback(std::function<void(int, const std::string)>(callback3)); // 注册接收一个整数和一个字符串的回调函数

    // 触发所有回调函数，传入不同数量和类型的参数
    manager.triggerCallbacks(22, 32.0);                              // 触发回调1
    manager.triggerCallbacks(std::string("Hello, world!"));          // 触发回调2
    manager.triggerCallbacks(7, std::string("Multiple parameters")); // 触发回调3

    return 0;
}
