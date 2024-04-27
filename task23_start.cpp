#include <thread>
#include <iostream>
#include <cinttypes>
#include <barrier>


int main() {
    std::int32_t v{0};
    std::barrier barrier{2};

    auto thread_func = [&v, &barrier] {
        for (std::int32_t i{0}; i < 1e9; ++i) {
            auto v_copy = v;
            barrier.arrive_and_wait();
            v = v_copy + 1;
        }
    };
    std::jthread t1{thread_func}, t2{thread_func};
    t1.join();
    t2.join();
    std::cout << "v = " << v << '\n';
}
