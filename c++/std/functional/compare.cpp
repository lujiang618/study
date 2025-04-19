#include <iostream>
#include <map>
#include <memory>
#include <functional>
#include <string>

struct Event
{
    std::string name;

    Event(const std::string& n)
        : name(n)
    {
    }
};

using EventCallback = std::function<bool(const std::shared_ptr<Event>)>;

class EventManager
{
public:
    using CallbackMap = std::multimap<std::string, EventCallback>;

    // 添加回调
    void addCallback(const std::string& eventName, EventCallback callback)
    {
        callbacks.emplace(eventName, std::move(callback));
    }

    // 删除特定的回调（使用比较器匹配）
    bool removeCallback(const std::string& eventName, const EventCallback& callbackToRemove)
    {
        auto range = callbacks.equal_range(eventName);
        for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
        {
            // std::function 不能直接比较，需要借助目标地址（适用于函数指针或相同捕获的lambda）
            if (it->second.target_type() == callbackToRemove.target_type())
            {
                callbacks.erase(it);
                return true;
            }
        }
        return false;
    }

    // 触发事件
    void triggerEvent(const std::string& eventName, std::shared_ptr<Event> evt)
    {
        auto range = callbacks.equal_range(eventName);
        for (auto it = range.first; it != range.second; ++it)
        {
            bool handled = it->second(evt);
            if (handled)
            {
                std::cout << "Event handled by one of the callbacks.\n";
            }
        }
    }

public:
    CallbackMap callbacks;
};

class Abc
{
public:
    bool handleEvent(std::shared_ptr<Event> evt)
    {
        std::cout << "abc event handled.\n";
        return true;
    }

    EventCallback handleEvent2 = [](const std::shared_ptr<Event> evt)
    {
        std::cout << "Callback1: " << evt->name << "\n";
        return true;
    };
};

int main()
{
    EventManager manager;

    auto callback1 = [](const std::shared_ptr<Event> evt)
    {
        std::cout << "Callback1: " << evt->name << "\n";
        return true;
    };

    auto callback2 = [](const std::shared_ptr<Event> evt)
    {
        std::cout << "Callback2: " << evt->name << "\n";
        return false;
    };

    manager.addCallback("OnStart", callback1);
    manager.addCallback("OnStart", callback2);

    Abc abc;

    auto callback3 = std::bind(&Abc::handleEvent, &abc, std::placeholders::_1);

    auto callback4 = abc.handleEvent2;

    manager.addCallback("abc", callback3);
    manager.addCallback("abc", callback4);

    std::cout << "Triggering OnStart:\n";
    manager.triggerEvent("OnStart", std::make_shared<Event>("GameStarted"));

    std::cout << "\nRemoving callback1...\n";
    std::cout << manager.callbacks.size() << std::endl;
    manager.removeCallback("OnStart", callback1);
    std::cout << manager.callbacks.size() << std::endl;

    std::cout << "\nRemoving callback3...\n";
    std::cout << manager.callbacks.size() << std::endl;
    manager.removeCallback("abc", callback3);
    std::cout << manager.callbacks.size() << std::endl;

    std::cout << "\nRemoving callback4...\n";
    std::cout << manager.callbacks.size() << std::endl;
    manager.removeCallback("abc", callback4);
    std::cout << manager.callbacks.size() << std::endl;

    std::cout << "\nTriggering OnStart again:\n";
    manager.triggerEvent("OnStart", std::make_shared<Event>("GameStartedAgain"));

    return 0;
}
