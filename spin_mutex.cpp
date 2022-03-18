//
// Created by zhaobo on 2022/3/18.
//

#include <iostream>
#include <string>
#include <thread>
#include <atomic>

using namespace std;

class spin_mutex {
public:
    spin_mutex() = default;
    spin_mutex(const spin_mutex&) = delete;
    spin_mutex &operator= (const spin_mutex &) = delete;

    void lock()
    {
        bool expected = false;

        /*如果flag为false, 则被修改为true*/
        while(!flag.compare_exchange_strong(expected, true))
        {
            expected = false;
        }
    }

    void unlock()
    {
        /*flag被修改为false*/
        flag.store(false);
    }

private:
    atomic<bool> flag = ATOMIC_VAR_INIT(false);
};

int num = 0;
spin_mutex sm;

void thread_proc()
{
    sm.lock();
    for(int i=0; i<100000; i++)
    {
        //sm.lock();
        ++num;
        //sm.unlock();
    }
    sm.unlock();

    cout << num << endl;
}

int main()
{
    thread td1(thread_proc);
    thread td2(thread_proc);

    td1.join();
    td2.join();

    cout << num << endl;

    return 0;
}
