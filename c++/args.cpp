#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

// 一个示例回调函数，接收一个整数
void callback1(int a, double b)
{
    std::cout << "Callback 1, received: " << a << " and " << b << std::endl;
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

typedef void (*EVentCallBack)(const IEventArg&);

class CallbackManager
{
public:
    // 修改后的addCallback，接受std::function类型的回调
    template <typename... Arguments>
    int addCallback(int eventType, std::function<void(Arguments...)> callback)
    {
        int callbackId = nextCallbackId_++;

        EventCallback wrappedCallback = [callback](const IEventArg& e)
        {
            const auto* event = dynamic_cast<const EventArg<Arguments...>*>(&e);
            if (event)
            {
                std::apply(callback, event->Args());
            }
        };

        callbacks[eventType].emplace_back(callbackId, wrappedCallback);

        return callbackId;
    }

    void printCallbacks()
    {
        for (const auto& callback : callbacks)
        {
            std::cout << "Event Type: " << callback.first << std::endl;
            for (const auto& cb : callback.second)
            {
                std::cout << "Callback ID: " << cb.first << std::endl;
            }
        }
    }

    // 触发所有回调函数，传入参数包
    template <typename... Arguments>
    void triggerCallbacks(int eventType, Arguments&&... args)
    {
        std::shared_ptr<IEventArg> event = std::make_shared<EventArg<Arguments...>>(std::forward<Arguments>(args)...);

        // 遍历所有回调函数并触发它们
        for (const auto& callback : callbacks[eventType])
        {
            // 触发回调函数时传入参数
            callback.second(*event);
        }
    }

    void removeCallback(int eventType, int id)
    {
        auto it = std::find_if(callbacks[eventType].begin(), callbacks[eventType].end(), [id](const auto& callback)
                               {
            std::cout << "callback id:" << callback.first << std::endl;
            std::cout << "remove id:" << id << std::endl;
            std::cout << (callback.first == id) << std::endl;

            return callback.first == id; });

        if (it != callbacks[eventType].end())
        {
            callbacks[eventType].erase(it);
        }
    }

private:
    int nextCallbackId_ = 0;
    // 存储所有的回调函数
    std::unordered_map<int, std::vector<std::pair<int, EventCallback>>> callbacks;
};

int main()
{
    CallbackManager manager;

    // 使用std::function将普通的回调函数传递给addCallback
    auto c1  = std::function<void(int, double)>(callback1);
    auto c2  = std::function<void(const std::string)>(callback2);
    auto c3  = std::function<void(int, const std::string)>(callback3);
    auto id1 = manager.addCallback(1, c1); // 注册接收一个整数的回调函数
    auto id2 = manager.addCallback(2, c2); // 注册接收一个字符串的回调函数
    auto id3 = manager.addCallback(3, c3); // 注册接收一个整数和一个字符串的回调函数

    std::cout << "===================================================" << std::endl;
    std::cout << "id1:" << id1 << std::endl;
    std::cout << "id2:" << id2 << std::endl;
    std::cout << "id3:" << id3 << std::endl;

    // manager.printCallbacks();

    std::cout << "===================================================" << std::endl;
    // 触发所有回调函数，传入不同数量和类型的参数
    manager.triggerCallbacks(1, 22, 32.0);                              // 触发回调1
    manager.triggerCallbacks(2, std::string("Hello, world!"));          // 触发回调2
    manager.triggerCallbacks(3, 7, std::string("Multiple parameters")); // 触发回调3

    std::cout << "===================================================" << std::endl;
    manager.removeCallback(1, id1);
    // manager.printCallbacks();

    std::cout << "===================================================" << std::endl;
    manager.triggerCallbacks(1, 22, 32.0);                              // 触发回调1
    manager.triggerCallbacks(2, std::string("Hello, world!"));          // 触发回调2
    manager.triggerCallbacks(3, 7, std::string("Multiple parameters")); // 触发回调3

    return 0;
}
