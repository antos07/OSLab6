#include <thread>
#include <iostream>
#include <cinttypes>


int main() {
    std::atomic<std::int32_t> v{0};

    auto thread_func = [&v] {
        for (std::int32_t i{0}; i < 1e9; ++i)
            ++v;
    };
    std::jthread t1{thread_func}, t2{thread_func};
    t1.join();
    t2.join();
    std::cout << "v = " << v << '\n';
}
