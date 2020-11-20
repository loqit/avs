#include <iostream>
#include <mutex>
#include <chrono>
#include <atomic>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <memory>
#include <queue>
#include <future>


#include "DynamicQueue.h"
#include "FixedAtomicQueue.h"
#include "FixedMutexQueue.h"

int taskNum = 4 * 1024 * 1024;
int prodNum[] = {1, 2, 4};
int consNum[] = {1, 2, 4};
int queueSize[] = {1, 4, 16};

template<typename Q>
void producer(Q& q)
{
    for(int i = 0;i < taskNum;i++)
    {
        q.push(1);
    }
}
template<typename Q>
void consumer(int prods, int cons, Q& q, std::atomic<int>& counter)
{
    int items = taskNum * prods / cons;
    int tmp;
    for(int i = 0;i < items ;i++)
    {
        while(!q.pop(tmp)) {}
        counter += tmp;
    }
}
template<typename Q>
void prodCon(int prods, int cons, Q& q)
{
    std::atomic<int> counter {0};

    std::vector<std::thread> prod_thr;
    std::vector<std::thread> cons_thr;
    prod_thr.reserve(prods);
    cons_thr.reserve(cons);

    for(int i = 0;i < prods;i++)
    {
        std::thread thr(producer<Q>, std::ref(q));
        prod_thr.push_back(move(thr));
    }

    for(int i = 0;i < cons;i++)
    {
        std::thread thr(consumer<Q>, prods, cons, std::ref(q), std::ref(counter));
        cons_thr.push_back(move(thr));
    }
    for(auto &t: prod_thr)
    {
        if(t.joinable())
        {
            t.join();
        }
    }
    for(auto &t: cons_thr)
    {
        if(t.joinable())
        {
            t.join();
        }
    }
    std::cout << "Sum : " << counter << "/" << prods *  taskNum << std::endl;

}
template <typename Q>
void task(Q& q)
{
    for(int c: consNum)
    {
        for(int p: prodNum)
        {
            std::cout << "Producers : " << p << " Consumers : " << c << std::endl;
            auto begin = std::chrono::high_resolution_clock::now();
            prodCon<Q>(p, c, std::ref(q));
            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration<double, std::milli>(end - begin);
            std::cout << "Time : " << time.count() << std::endl;
        }
    }
}

int main() {

    int num;
    std::cout << "Choose sub-task num: ";
    std::cin >> num;
    if(num == 1)
    {
        DynamicQueue q;
        task<DynamicQueue>(std::ref(q));
    }
    if(num == 2)
    {
        for(int s: queueSize)
        {
            FixedMutexQueue q(s);
            std::cout << "Queue size : " << s << std::endl;
            task<FixedMutexQueue>(std::ref(q));
            std::cout << "================================" << std::endl;
        }

    }
    if(num == 3)
    {
        for(int s: queueSize)
        {
            FixedAtomicQueue q(s);
            std::cout << "Queue size : " << s << std::endl;
            task<FixedAtomicQueue>(std::ref(q));
            std::cout << "================================" << std::endl;
        }

    }
    return 0;
}
