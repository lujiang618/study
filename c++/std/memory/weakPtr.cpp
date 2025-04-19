#include <functional>
#include <iostream>
#include <vector>

void callback5(int b)
{
    std::cout << "Callback 5, received: " << b << std::endl;
}

class Test
{

public:
    void add(std::function<void(int)> callback)
    {
        auto it = find_if(callbacks.begin(), callbacks.end(), [callback](auto& c)
                          { return c == callback; });

        if (it == callbacks.end())
        {
            callbacks.push_back(callback);
            std::cout << "==============================" << std::endl;
        }
    }

private:
    std::vector<std::function<void(int)>> callbacks;
};

int main()

{
    Test test;

    // test.add(std::function<void(int)>(callback5));

    return 0;
}
