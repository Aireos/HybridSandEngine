#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <atomic>
#include <memory>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(uint32_t num_threads = 8);
    ~ThreadPool();

    // Push work to thread pool
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(0, args...))> {
        using return_type = decltype(f(0, args...));
        auto task = std::make_shared<std::packaged_task<return_type(int)>>(
            [f = std::forward<F>(f), args...](int id) mutable {
                return f(id, std::forward<Args>(args)...);
            }
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (stop_) throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks_.emplace([task](int id) { (*task)(id); });
        }
        condition_.notify_one();
        return res;
    }

    uint32_t get_num_threads() const { return num_threads_; }
    void wait_all();
    void stop();

private:
    uint32_t num_threads_;
    std::vector<std::thread> workers_;
    std::queue<std::function<void(int)>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
};
