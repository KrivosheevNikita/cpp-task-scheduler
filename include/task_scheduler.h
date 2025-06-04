#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <queue>
#include <functional>
#include <ctime>
#include <mutex>
#include <thread>
#include <condition_variable>

class TaskScheduler
{
public:
    TaskScheduler();
    ~TaskScheduler();

    // Add ��������� ������, ������� ������ ����������� �� ������ timestamp
    void Add(std::function<void()> task, std::time_t timestamp);

private:
    struct Task
    {
        std::time_t time;
        std::function<void()> fn;

        // ��������������� ��������� < ��� ����, ����� ������� ���� ������ � ����� ������ timestamp
        bool operator<(const Task& other) const
        {
            return time > other.time;
        }
    };

    // run ��������� �������� ����, � ������� ���� ��������� �����, ���� ����������� �� timestamp � ��������� �� ����� �����
    void run();

    std::priority_queue<Task> tasks;
    std::thread worker;
    bool stop;

    std::mutex mu;
    std::condition_variable cv;
};

#endif
