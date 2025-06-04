#include <iostream>
#include <chrono>

#include "task_scheduler.h"

TaskScheduler::TaskScheduler()
    : stop{ false },
    worker{ &TaskScheduler::run, this }
{}

TaskScheduler::~TaskScheduler()
{
    {
        std::lock_guard<std::mutex> lock(mu);
        stop = true;
    }
    cv.notify_one();
    if (worker.joinable())
        worker.join();
}

void TaskScheduler::Add(std::function<void()> task, std::time_t timestamp)
{
    {
        std::lock_guard<std::mutex> lock(mu);
        tasks.push(Task{ timestamp, std::move(task) });
    }
    cv.notify_one();
}

void TaskScheduler::run()
{
    std::unique_lock<std::mutex> lock(mu);

    while (true)
    {
        cv.wait(lock, [this] { 
            return stop || !tasks.empty(); 
            });

        if (stop && tasks.empty())
            break;                             

        auto next = tasks.top();
        auto next_time = std::chrono::system_clock::from_time_t(next.time);

        if (cv.wait_until(lock, next_time, [this, next_time] {
            return stop || tasks.top().time < std::chrono::system_clock::to_time_t(next_time);
            }))
        {
            continue;
        }

        auto fn = std::move(tasks.top().fn);
        tasks.pop();

        lock.unlock();

        try 
        { 
            fn(); 
        }
        catch (const std::exception& ex)
        {
            std::cerr << "err in task: " << ex.what() << std::endl;
        }
        lock.lock();
    }
}
