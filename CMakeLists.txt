cmake_minimum_required(VERSION 3.14)
project(TaskScheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(task_scheduler
    example/main.cpp
    src/task_scheduler.cpp
)
