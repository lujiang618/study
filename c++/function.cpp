#include <functional>
#include <iostream>

template <typename... Arguments>
std::vector<std::function<void(Arguments...)>> callbacks;

template <typename... Arguments>
void add(std::function<void(Arguments...)> callback1, std::function<void(Arguments...)>& callback2)
{

    std::cout << &callback1 << " == " << (callback2 << std::endl;
    if (&callback1 == &callback2)
    {
        std::cout << "Callbacks are equal." << std::endl;
    }
    else
    {
        std::cout << "Callbacks are not equal." << std::endl;
    }

    // callbacks.push_back(&callback1);
    // callbacks.push_back(&callback2);
}

void func(int x) { std::cout << "Called func with " << x << std::endl; }

int main()
{
    std::function<void(int)> callback1 = func;
    std::function<void(int)> callback2 = callback1;

    add(callback1, callback2); // Outputs: Callbacks are equal.

    std::function<void(int)> callback3 = [](int x)
    { std::cout << "Lambda called with " << x << std::endl; };

    add(callback1, callback3); // Outputs: Callbacks are not equal.

    return 0;
}
