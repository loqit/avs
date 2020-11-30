#include <iostream>
#include <mutex>
#include <chrono>
#include <atomic>
#include <thread>
#include <vector>
#include <memory>


#include "task1.h"

int numTasks = 1024 * 1024;
int numThreads[] = {4, 8, 16, 32};
std::mutex m;

template <typename T>
void mutex_counter(int* arr, T& index)
{
    m.lock();
    int tmp = index++;
    m.unlock();
    while (tmp < numTasks)
    {
        arr[tmp]++;
        m.lock();
        tmp = index++;
        m.unlock();
    }
}
template <typename T>
void atomic_counter(int* arr, T& index)
{
    int tmp = index++;
    while (tmp < numTasks)
    {
        arr[tmp]++;
        tmp = index++;
    }
}
template <typename T>
void calc(int num_threads, void(*f) (int*, T&), T& index)
{
    std::vector<std::thread> threads;
    int* array = new int [numTasks];
    threads.reserve(num_threads);

    for (int i = 0; i < num_threads; ++i)
    {
        std::thread thr((*f), std::ref(array), std::ref(index));
        threads.push_back(move(thr));
    }
    for (auto &t : threads) {
        if (t.joinable())
        {
            t.join();
        }
    }

    for (int i = 0; i < numTasks; i++) {
        if (array[i] != 1)
        {
            std::cout << array[i] << std::endl;
        }
    }
    delete [] array;

}

int main() {

    std::cout << "Choose task :";
    int num;
    std::cin >> num;
    if (num == 1)
    {
        for(int n: numThreads)
        {
            int index = 0;
            auto begin = std::chrono::high_resolution_clock::now();
            calc<int>(n, mutex_counter, std::ref(index));
            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration<double, std::milli>(end - begin).count();
            std::cout << "Threads : " << n << " Time : " << time << std::endl;
        }

    } else {
        for(int n: numThreads)
        {
            std::atomic<int> index{0};
            auto begin = std::chrono::high_resolution_clock::now();
            calc<std::atomic<int>>(n, atomic_counter, std::ref(index));
            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration<double, std::milli>(end - begin).count();
            std::cout << "Threads : " << n << " Time : " << time << std::endl;
        }
    }

    return 0;
}
