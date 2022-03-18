//
// Created by zhaobo on 2022/3/18.
//

#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

std::mutex              g_mutex;    // 全局锁
std::condition_variable g_cond;     // 条件变量

int     g_i         = 0;
bool    g_running   = true;

void ThreadFunc(int n) {            // 线程执行函数
    for (int i = 0; i < n; i++) {
        {
            std::lock_guard <std::mutex> lock(g_mutex);     // 加锁，离开{}作用域后锁释放
            ++g_i;
            std::cout << "plus g_i by func thread " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));    // 休眠5毫秒
        }
    }
    std::unique_lock<std::mutex> lock(g_mutex);     // 加锁
    while (g_running) {
        std::cout << "wait for exit" << std::endl;
        g_cond.wait(lock);      // wait调用后，会先释放锁，之后进入等待状态；当其他进程调用通知激活后，会再次加锁
    }
    std::cout << "func thread exit" << std::endl;
}

int main()
{
    int n = 10;
    std::thread t1(ThreadFunc, n);      // 创建t1线程，t1会执行`ThreadFunc`中的指令
    std::thread t2(ThreadFunc, n);

    for (int i = 0; i < n; i++) {
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            ++g_i;
            std::cout << "plus g_i by main thread " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));    // 休眠5毫秒
        }
    }

    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_running = false;
        g_cond.notify_all();    // 通知其他线程
    }

    t1.join();      // 等待线程t1结束
    t2.join();
    std::cout << "g_i = " << g_i << std::endl;
}
