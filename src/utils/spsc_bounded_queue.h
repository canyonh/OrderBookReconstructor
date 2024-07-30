#pragma once 

#include <atomic>
#include <array>
#include <cstddef>
#include <optional>

namespace Utils
{

#ifdef __GCC_DESTRUCTIVE_SIZE
    static constexpr size_t hardware_destructive_interference_size = __GCC_DESTRUCTIVE_SIZE;
#else
    static constexpr size_t hardware_destructive_interference_size = 64;
#endif

template<typename ElemType, size_t Capacity, typename IndexType = uint64_t>
class SPSCBoundedQueueSeqCstFalseSharing
{
    static_assert(Capacity > 0 && (Capacity & (Capacity - 1)) == 0, "Capacity must be a power of 2");
public:
    bool Push(const ElemType& elem)
    {
        if (push_pos - pop_pos == Capacity)
            return false;

        data[push_pos & (Capacity - 1)] = elem;
        push_pos.fetch_add(1, std::memory_order_release);
        return true;
    }

    bool Push(ElemType&& elem) noexcept
    {
        if (push_pos - pop_pos == Capacity)
            return false;

        data[push_pos & (Capacity - 1)] = std::move(elem);
        push_pos.fetch_add(1, std::memory_order_release);
        return true;
    }

    std::optional<ElemType> Pop()
    {
        if (push_pos - pop_pos == 0)
            return std::nullopt;

        std::optional<ElemType> ret(std::move(data[pop_pos & (Capacity - 1)]));
        pop_pos.fetch_add(1, std::memory_order_release);
        return ret;
    }

private:
    std::atomic<IndexType> push_pos{};
    std::atomic<IndexType> pop_pos{};
    std::array<ElemType, Capacity> data;
};

template<typename ElemType, size_t Capacity, typename IndexType = uint64_t>
class SPSCBoundedQueueSeqCst
{
    static_assert(Capacity > 0 && (Capacity & (Capacity - 1)) == 0, "Capacity must be a power of 2");
public:
    bool Push(const ElemType& elem)
    {
        if (push_pos - pop_pos == Capacity)
            return false;

        data[push_pos & (Capacity - 1)] = elem;
        push_pos.fetch_add(1, std::memory_order_release);
        return true;
    }

    bool Push(ElemType&& elem) noexcept
    {
        if (push_pos - pop_pos == Capacity)
            return false;

        data[push_pos & (Capacity - 1)] = std::move(elem);
        push_pos.fetch_add(1, std::memory_order_release);
        return true;
    }

    std::optional<ElemType> Pop()
    {
        if (push_pos - pop_pos == 0)
            return std::nullopt;

        std::optional<ElemType> ret(std::move(data[pop_pos & (Capacity - 1)]));
        pop_pos.fetch_add(1, std::memory_order_release);
        return ret;
    }

private:
    alignas(hardware_destructive_interference_size) std::atomic<IndexType> push_pos{};
    alignas(hardware_destructive_interference_size) std::atomic<IndexType> pop_pos{};
    std::array<ElemType, Capacity> data;
};

template<typename ElemType, size_t Capacity, typename IndexType = uint64_t>
class SPSCBoundedQueueModulous
{
public:
    bool Push(const ElemType& elem)
    {
        IndexType current_push_pos = push_pos.load(std::memory_order_relaxed);
        IndexType current_pop_pos = pop_pos.load(std::memory_order_acquire);

        if (current_push_pos - current_pop_pos == Capacity)
            return false;

        data[current_push_pos % Capacity] = elem;
        push_pos.fetch_add(1, std::memory_order_release);
        return true;
    }

    bool Push(ElemType&& elem) noexcept
    {
        IndexType current_push_pos = push_pos.load(std::memory_order_relaxed);
        IndexType current_pop_pos = pop_pos.load(std::memory_order_acquire);

        if (current_push_pos - current_pop_pos == Capacity)
            return false;

        data[current_push_pos % Capacity] = std::move(elem);
        push_pos.fetch_add(1, std::memory_order_release);
        return true;
    }

    std::optional<ElemType> Pop()
    {
        IndexType current_pop_pos = pop_pos.load(std::memory_order_relaxed);
        IndexType current_push_pos = push_pos.load(std::memory_order_acquire);

        if (current_push_pos - current_pop_pos == 0)
            return std::nullopt;

        std::optional<ElemType> ret(std::move(data[current_pop_pos % Capacity]));
        pop_pos.fetch_add(1, std::memory_order_release);
        return ret;
    }

private:
    alignas(hardware_destructive_interference_size) std::atomic<IndexType> push_pos{};
    alignas(hardware_destructive_interference_size) std::atomic<IndexType> pop_pos{};
    std::array<ElemType, Capacity> data;
};

template<typename ElemType, size_t Capacity, typename IndexType = uint64_t>
class SPSCBoundedQueue
{
    static_assert(Capacity > 0 && (Capacity & (Capacity - 1)) == 0, "Capacity must be a power of 2");
public:
    bool Push(const ElemType& elem)
    {
        IndexType current_push_pos = push_pos.load(std::memory_order_relaxed);
        IndexType current_pop_pos = pop_pos.load(std::memory_order_acquire);

        if (current_push_pos - current_pop_pos == Capacity)
            return false;

        data[current_push_pos & (Capacity - 1)] = elem;
        push_pos.fetch_add(1, std::memory_order_release);
        return true;
    }

    bool Push(ElemType&& elem) noexcept
    {
        IndexType current_push_pos = push_pos.load(std::memory_order_relaxed);
        IndexType current_pop_pos = pop_pos.load(std::memory_order_acquire);

        if (current_push_pos - current_pop_pos == Capacity)
            return false;

        data[current_push_pos & (Capacity - 1)] = std::move(elem);
        push_pos.fetch_add(1, std::memory_order_release);
        return true;
    }

    std::optional<ElemType> Pop()
    {
        IndexType current_pop_pos = pop_pos.load(std::memory_order_relaxed);
        IndexType current_push_pos = push_pos.load(std::memory_order_acquire);

        if (current_push_pos - current_pop_pos == 0)
            return std::nullopt;

        std::optional<ElemType> ret(std::move(data[current_pop_pos & (Capacity - 1)]));
        pop_pos.fetch_add(1, std::memory_order_release);
        return ret;
    }

private:
    alignas(hardware_destructive_interference_size) std::atomic<IndexType> push_pos{};
    alignas(hardware_destructive_interference_size) std::atomic<IndexType> pop_pos{};
    std::array<ElemType, Capacity> data;
};

}
