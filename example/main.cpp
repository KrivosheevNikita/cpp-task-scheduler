#include <iostream>
#include <thread>
#include <chrono>

#include "task_scheduler.h"

int main()
{
    TaskScheduler scheduler;

    auto now = std::time(nullptr);

    scheduler.Add([] {
        std::cout << "1" << std::endl;
        }, now + 1);

    scheduler.Add([] {
        std::cout << "2" << std::endl;
        }, now + 3);

    scheduler.Add([] {
        std::cout << "3" << std::endl;
        }, now);

    std::this_thread::sleep_for(std::chrono::seconds(4));

    return 0;
}
