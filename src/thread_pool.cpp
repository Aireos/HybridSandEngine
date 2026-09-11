#include "thread_pool.hpp"
#include <iostream>

ThreadPool::ThreadPool(uint32_t num_threads) 
    : num_threads_(num_threads), stop_(false) {
    for (uint32_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this, i]() {
            while (true) {
                std::function<void(int)> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) return;
                    if (tasks_.empty()) continue;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task(i);
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::wait_all() {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    condition_.wait(lock, [this] { return tasks_.empty(); });
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (std::thread& worker : workers_) {
        if (worker.joinable()) worker.join();
    }
}
