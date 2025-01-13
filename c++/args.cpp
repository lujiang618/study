#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <shared_mutex>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <vector>

// 基础事件类
class IEventArg
{
public:
    virtual ~IEventArg() = default;

    virtual int EventType() const = 0;
};

template <typename... Arguments>
class EventArg : public IEventArg
{
public:
    EventArg(int eventType, Arguments&&... args)
        : event_type_(eventType)
        , args_(std::forward<Arguments>(args)...)

    {
    }

    ~EventArg() = default;

    int EventType() const override
    {
        return event_type_;
    }

    std::tuple<Arguments...> Args() const
    {
        return args_;
    }

private:
    std::tuple<Arguments...> args_;

    int event_type_;
};

class EventQueue
{
public:
    EventQueue(size_t capacity = 1024)
        : capacity_(capacity)
        , head_(0)
        , tail_(0)
    {
        buffer_ = new std::unique_ptr<IEventArg>[capacity_];
    }

    ~EventQueue()
    {
        delete[] buffer_;
    }

    void push(std::unique_ptr<IEventArg> event)
    {
        size_t tail      = tail_.load(std::memory_order_relaxed);
        size_t next_tail = (tail + 1) % capacity_;

        // 等待直到队列有空位
        while (next_tail == head_.load(std::memory_order_acquire))
        {
            // 可以选择使用std::this_thread::yield()来降低CPU负担
            std::this_thread::yield();
        }

        buffer_[tail] = std::move(event);
        tail_.store(next_tail, std::memory_order_release);
    }

    std::unique_ptr<IEventArg> pop()
    {
        size_t head = head_.load(std::memory_order_relaxed);

        // 等待直到队列有数据
        while (head == tail_.load(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }

        auto event = std::move(buffer_[head]);
        head_      = (head + 1) % capacity_;

        return event;
    }

private:
    size_t                      capacity_;
    std::unique_ptr<IEventArg>* buffer_;
    std::atomic<size_t>         head_; // 队列头部的索引
    std::atomic<size_t>         tail_; // 队列尾部的索引
};

// 定义回调函数的类型，回调函数可以接受任意数量的参数
using EventCallback = std::function<void(const IEventArg&)>;

struct CallbackInfo
{
    int id;

    bool isOnce = false;

    void* data;

    EventCallback Execute;

    // 用于标识回调函数的唯一hash
    int callbackPtr;
};

// template <typename... Arguments>
// typedef void (*EventCallback)(const Arguments...);

class CallbackManager
{

public:
    CallbackManager()
        : nextCallbackId_(0)
    {
    }

    void Print()
    {
        // std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& callback : callback_map_)
        {
            std::cout << "Event Type: " << callback.first << std::endl;
            for (const auto& cb : callback.second)
            {
                std::cout << "Callback ID: " << cb.id << std::endl;
            }
        }
    }

    template <typename... Arguments>
    int Add(int eventType, std::function<void(Arguments...)> callback)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);

        int callbackId = ++nextCallbackId_;

        // void (*const* callbackTarget)(Arguments...) = callback.target<std::function<void(Arguments...)>>();

        EventCallback wrappedCallback = [callback](const IEventArg& e)
        {
            const auto* event = dynamic_cast<const EventArg<Arguments...>*>(&e);
            if (event)
            {
                callback(std::get<Arguments>(event->Args())...);
            }
            else
            {
                std::cerr << "Failed to cast event to expected type!" << std::endl;
            }
        };

        CallbackInfo info{callbackId, false, nullptr, wrappedCallback, 0};

        callback_map_[eventType].emplace_back(info);

        return callbackId;
    }

    void Remove(int eventType, int id)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);

        auto& vec = callback_map_[eventType];
        auto  it  = std::find_if(vec.begin(), vec.end(), [id](const auto& callback)
                                 { return callback.id == id; });

        if (it != vec.end())
        {
            std::cout << "Remove callback id: " << id << std::endl;
            vec.erase(it);
        }

        Print();
    }

    inline std::vector<CallbackInfo>& Get(int eventType)
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return callback_map_[eventType];
    }

private:
    std::atomic<int> nextCallbackId_;

    std::unordered_map<int, std::vector<CallbackInfo>> callback_map_;

    std::shared_mutex mutex_;
};

class EventManager

{
public:
    EventManager()
        : stop_(false)
    {
        startAsyncThread();
    }

    ~EventManager()
    {
        stopAsyncThread();
    }

    // 修改后的addCallback，接受std::function类型的回调
    template <typename... Arguments>
    int addCallback(int eventType, std::function<void(Arguments...)> callback)
    {
        return callbackManager_.Add(eventType, callback);
    }

    void printCallbacks()
    {
        callbackManager_.Print();
    }

    // 触发所有回调函数，传入参数包
    template <typename... Arguments>
    void triggerCallbacks(int eventType, bool async = false, Arguments&&... args)
    {
        auto callbackArr = callbackManager_.Get(eventType);

        if (callbackArr.empty())
        {
            return;
        }

        std::unique_ptr<IEventArg> event = std::make_unique<EventArg<Arguments...>>(eventType, std::forward<Arguments>(args)...);

        if (async)
        {
            asyncTriggerCallbacks(std::move(event));
        }
        else
        {
            // 如果是同步，直接触发回调
            for (const auto& callback : callbackArr)
            {
                callback.Execute(*event);
            }
        }
    }

    // 移除回调函数
    void removeCallback(int eventType, int id)
    {
        callbackManager_.Remove(eventType, id);
    }

    void Stop()
    {
        std::cout << "Stop EventManager" << std::endl;
        stopAsyncThread();
    }

protected:
    // 异步执行回调
    void asyncTriggerCallbacks(std::unique_ptr<IEventArg> event)
    {
        eventQueue_.push(std::move(event));
    }

    void stopAsyncThread()
    {
        {
            std::lock_guard<std::mutex> lock(stopMutex_);
            stop_ = true;
        }
        cv_.notify_all();
        if (asyncThread_.joinable())
        {
            asyncThread_.join();
        }
    }

    // 启动线程
    void startAsyncThread()
    {
        asyncThread_ = std::thread(&EventManager::processAsyncCallbacks, this);
    }

    // 处理异步回调
    void processAsyncCallbacks()
    {
        while (true)
        {
            auto event = eventQueue_.pop();

            {
                std::lock_guard<std::mutex> lock(stopMutex_);
                if (stop_)
                    break;
            }

            auto callbackArr = callbackManager_.Get(event->EventType());
            for (const auto& callback : callbackArr)
            {
                try
                {
                    callback.Execute(*event);
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Exception occurred while executing callback ID " << callback.id << ": " << e.what() << std::endl;
                }
                catch (...)
                {
                    std::cerr << "Unknown exception occurred during callback execution." << std::endl;
                }
            }
        }

        std::cout << "processAsyncCallbacks exit" << std::endl;
    }

private:
    std::atomic<bool> stop_; // 线程停止标志

    std::mutex stopMutex_;

    std::thread asyncThread_;

    std::condition_variable cv_;

    CallbackManager callbackManager_;

    EventQueue eventQueue_;
};

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

void callback4(const std::string& str)
{
    std::cout << "Callback 4, received: " << str << std::endl;
}

// 一个示例回调函数，接收一个整数和一个字符串
void callback3(int a, const std::string str)
{
    std::cout << "Callback 3, received: " << a << " and " << str << std::endl;
}

void callback5(void* a, double b)
{
    std::cout << "Callback 5, received: " << *static_cast<int*>(a) << " and " << b << std::endl;
}

void callback6()
{
    std::cout << "Callback 6 " << std::endl;
}

int main()

{
    EventManager manager;

    // 使用std::function将普通的回调函数传递给addCallback
    auto c1 = std::function<void(int, double)>(callback1);
    auto c2 = std::function<void(const std::string)>(callback2);
    auto c3 = std::function<void(int, const std::string)>(callback3);
    auto c4 = std::function<void(const std::string)>(callback4);

    int a = 20;

    int* b = &a;

    std::function<void(double)> c5(std::bind(callback5, b, std::placeholders::_1));

    std::function<void()> c6(callback6);

    auto id1 = manager.addCallback(1, c1); // 注册接收一个整数的回调函数
    auto id2 = manager.addCallback(2, c2); // 注册接收一个字符串的回调函数
    auto id3 = manager.addCallback(3, c3); // 注册接收一个整数和一个字符串的回调函数
    auto id4 = manager.addCallback(2, c4); // 注册接收一个整数和一个字符串的回调函数
    auto id5 = manager.addCallback(5, c5); // 注册接收一个整数和一个字符串的回调函数
    auto id6 = manager.addCallback(6, c6); // 注册接收一个整数和一个字符串的回调函数

    std::cout << "===================================================" << std::endl;
    std::cout << "id1:" << id1 << std::endl;
    std::cout << "id2:" << id2 << std::endl;
    std::cout << "id3:" << id3 << std::endl;

    std::cout << "===================================================" << std::endl;
    // 触发所有回调函数，传入不同数量和类型的参数
    manager.triggerCallbacks(1, false, 22, 32.0);                             // 触发回调1（同步）
    manager.triggerCallbacks(2, true, std::string("Hello, world!"));          // 触发回调2（异步）
    manager.triggerCallbacks(3, true, 7, std::string("Multiple parameters")); // 触发回调3（异步）

    manager.triggerCallbacks(6, false);
    manager.triggerCallbacks(6);

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待异步回调执行

    std::cout << "===================================================" << std::endl;
    manager.removeCallback(1, id1); // 删除回调1

    std::cout << "===================================================" << std::endl;
    manager.triggerCallbacks(1, true, 22, 32.0);                               // 触发回调1（异步）已删除
    manager.triggerCallbacks(2, false, std::string("Hello, world!"));          // 触发回调2（同步）
    manager.triggerCallbacks(3, false, 7, std::string("Multiple parameters")); // 触发回调3（同步）
    manager.triggerCallbacks(5, false, 32.0);                                  // 触发回调3（同步）

    // 等待异步线程完成
    // asyncThread.join();

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待异步回调执行

    manager.Stop();

    return 0;
}
