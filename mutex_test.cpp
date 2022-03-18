//
// Created by zhaobo on 2022/3/18.
//

#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

// 不加锁实现
void Add(int* px, int n) {
    for (int i = 0; i < n; i++) {
        (*px)++;
    }
}

// 外层加锁
void Add1(int* px, int n, mutex* pmt) {
    (*pmt).lock();
    for (int i = 0; i < n; i++) {
        (*px)++;
    }
    (*pmt).unlock();
}

// 内层加锁
void Add2(int* px, int n, mutex* pmt) {
    for (int i = 0; i < n; i++) {
        (*pmt).lock();
        (*px)++;
        (*pmt).unlock();
    }
}

// 应用lock_guard管理锁
void Add3(int* px, int n, mutex* pmt) {
    lock_guard <mutex> lg(*pmt);
    for (int i = 0; i < n; i++) {
        (*px)++;
    }
}

// 应用unique_lock管理锁
void Add4(int* px, int n, mutex* pmt) {
    unique_lock<mutex> ulg(*pmt);
    for (int i = 0; i < n; i++) {
        (*px)++;
    }
}

// 测试不加锁线程抢占情况下执行结果
void Func() {
    int x = 0;
    mutex pmt;
    thread t1(Add, &x, 10000);
    thread t2(Add, &x, 10000);
    t1.join();      // 等待t1结束
    t2.join();      // 等待t2结束
    cout << "func() x after add : " << x << endl;
}

// 测试外层加锁的执行结果
void Func1() {
    int x = 0;
    mutex pmt;
    thread t1(Add1, &x, 10000, &pmt);
    thread t2(Add1, &x, 10000, &pmt);
    t1.join();
    t2.join();
    cout << "func1() x after add : " << x << endl;
}

// 测试内层加锁的执行结果
void Func2() {
    int x = 0;
    mutex pmt;
    thread t1(Add2, &x, 10000, &pmt);
    thread t2(Add2, &x, 10000, &pmt);
    t1.join();
    t2.join();
    cout << "func1() x after add : " << x << endl;
}

// 测试lock_guard执行结果
void Func3() {
    int x = 0;
    mutex pmt;
    thread t1(Add3, &x, 10000, &pmt);
    thread t2(Add3, &x, 10000, &pmt);
    t1.join();
    t2.join();
    cout << "func3() x after add : " << x << endl;
}

// 测试unique_lock执行结果
void Func4() {
    int x = 0;
    mutex pmt;
    thread t1(Add4, &x, 10000, &pmt);
    thread t2(Add4, &x, 10000, &pmt);
    t1.join();
    t2.join();
    cout << "func4() x after add : " << x << endl;
}

int main() {
    int start = clock();
    Func();
    int end = clock();
    cout << "func() use time : " << end-start << endl;

    int start1 = clock();
    Func1();
    int end1 = clock();
    cout << "func1() use time : " << end1-start1 << endl;

    int start2 = clock();
    Func2();
    int end2 = clock();
    cout << "func2() use time : " << end2-start2 << endl;

    int start3 = clock();
    Func3();
    int end3 = clock();
    cout << "func3() use time : " << end3-start3 << endl;

    int start4 = clock();
    Func4();
    int end4 = clock();
    cout << "func4() use time : " << end4-start4 << endl;

    return 0;
}
