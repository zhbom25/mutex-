//
// Created by zhaobo on 2022/3/18.
//
// 线程安全的队列

#pragma once

#include <queue>
#include <mutex>

using namespace std;

template<typename T>
class SafeQueue {
private:
    queue<T>    m_queue;
    mutex       m_mutex;

public:
    SafeQueue() {}
    ~SafeQueue() {}

    bool empty() {
        unique_lock<mutex> lock(m_mutex);
        return m_queue.empty();
    }

    int size() {
        unique_lock<mutex> lock(m_mutex);
        return m_queue.size();
    }

    // 将元素t入队列
    void enqueue(T& t) {
        unique_lock<mutex> lock(m_mutex);
        m_queue.push(t);
    }

    // 取队列头元素，存入t
    bool dequeue(T& t) {
        unique_lock<mutex> lock(m_mutex);
        if (m_queue.empty()) {
            return false;
        }
        t = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }
};