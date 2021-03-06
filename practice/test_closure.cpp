#include <iostream>
#include <memory>

int main()
{
    auto a = std::make_shared<int>();
    std::cout << a.use_count() << std::endl;

    {
        auto x = [a]() {
            std::cout << a.use_count() << std::endl;
        };
        x();
        std::cout << a.use_count() << std::endl;
    }
    std::cout << a.use_count() << std::endl;
}
