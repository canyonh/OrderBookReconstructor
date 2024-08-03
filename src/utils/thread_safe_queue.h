#pragma once
#include <optional>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace Util
{

template <typename T>
class ThreadSafeQueue
{
public:
    bool Push(const T& t)
    {
        {
            std::lock_guard lock(mutex);
            q.emplace_back(t);
        }
        cv.notify_one();
        return true;
    }

    bool Push(T&& t)
    {
        {
            std::lock_guard lock(mutex);
            q.emplace_back(std::move(t));
        }
        cv.notify_one();
        return true;
    }

    std::optional<T> Pop()
    {
        auto pred = [this]{ return !q.empty(); };

        std::unique_lock lock(mutex);
        cv.wait(lock, pred);
        T ret = std::move(q.front());
        q.pop_front();
        return ret;
    }

    std::optional<T> TryPop()
    {
        std::unique_lock lock(mutex);
        if (!q.empty())
        {
            T ret = std::move(q.front());
            q.pop_front();
            return ret;
        }
        return std::nullopt;
    }

    std::optional<T> Pop(std::chrono::milliseconds timeout)
    {
        auto pred = [this]{ return !q.empty(); };

        std::unique_lock lock(mutex);
        if (!cv.wait_for(lock, timeout, pred))
            return std::nullopt ;

        assert(!q.empty());
        T ret = std::move(q.front());
        q.pop_front();
        return ret;
    }
private:
    std::mutex mutex;
    std::condition_variable cv;
    std::deque<T> q;
};

}
