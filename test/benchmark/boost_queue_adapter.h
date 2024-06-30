#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <optional>

template<template<typename...> class Queue, typename Elem = int>
class BoostQueueAdapter
{
public:
    BoostQueueAdapter(int capacity = 8192) : queue(capacity) {} // Adjust capacity as needed

    void Push(const int& value) {
        while (!queue.push(value)) {
            // Keep trying if push fails
        }
    }

    std::optional<Elem> Pop() {
        Elem value;
        if (queue.pop(value)) {
            return value;
        }
        return std::nullopt;
    }
private:
    Queue<Elem> queue;
};

template<typename T>
using BoostLockfreeQueue = BoostQueueAdapter<boost::lockfree::queue, T>;

template<typename T>
using BoostLockfreeSPSCQueue = BoostQueueAdapter<boost::lockfree::spsc_queue, T>;


/*
template<typename T>
class BoostLockfreeQueueAdapter {
private:
    boost::lockfree::queue<T> queue;

public:
    BoostLockfreeQueueAdapter() : queue(128) {} // Adjust capacity as needed

    void Push(const T& value) {
        while (!queue.push(value)) {
            // Keep trying if push fails
        }
    }

    std::optional<T> Pop() {
        T value;
        if (queue.pop(value)) {
            return value;
        }
        return std::nullopt;
    }
};

template<typename T>
class BoostLockfreeSPSCQueueAdapter {
private:
    boost::lockfree::spsc_queue<T> queue;

public:
    BoostLockfreeSPSCQueueAdapter() : queue(128) {} // Adjust capacity as needed

    void Push(const T& value) {
        while (!queue.push(value)) {
            // Keep trying if push fails
        }
    }

    std::optional<T> Pop() {
        T value;
        if (queue.pop(value)) {
            return value;
        }
        return std::nullopt;
    }
};
*/
