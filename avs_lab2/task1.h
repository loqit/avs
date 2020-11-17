//
// Created by andrejbobr on 10.11.2020.
//

#ifndef AVS_LAB2_TASK1_H
#define AVS_LAB2_TASK1_H

class task1 {
public:
    static void mutex_counter()
    {
        int numTasks = 1024 * 1024;
        int numThreads[] = {4, 8, 16, 32};
        std::vector<int> v;
        std::mutex m;
        m.lock();
        int index = 0;
        m.unlock();
        auto func = [&m](std::vector<int>& v,const int& n, int& index)
        {
            while(index < n)
            {
                v[index]++;
                m.lock();
                index++;
                std::this_thread::sleep_for(std::chrono::nanoseconds(10));
                m.unlock();

            }
        };
        for(int i: numThreads)
        {
            std::vector<std::thread> threads;
            v.clear();
            v.resize(numTasks);
            threads.reserve(i);
            auto l = std::chrono::high_resolution_clock::now();
            for(int j = 0;j < i;j++)
            {
                std::thread thr(func, std::ref(v), std::ref(numTasks), std::ref(index));
                threads.push_back(move(thr));
            }
            for(auto &t: threads)
            {
                t.join();
            }
            auto r = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration<double, std::milli>(r - l).count();
            std::cout << i << " threads in " << time << std::endl;
            m.lock();
            index = 0;
            m.unlock();
        }
    }
    static void atomic_counter()
    {
        int numTasks = 1024 * 1024;
        int numThreads[] = {4, 8, 16, 32};
        std::vector<int> v;
        std::atomic<int> i = {0};

        auto func = [](std::vector<int>& v, const int& n, std::atomic<int>& i)
        {
            int c = 0;
            while(c < n)
            {
                v[c]++;
                c = i.fetch_add(1);
                std::this_thread::sleep_for(std::chrono::nanoseconds(10));
            }
        };
        for(int j: numThreads)
        {
            std::vector<std::thread> threads;
            v.clear();
            v.resize(numTasks);
            auto l = std::chrono::high_resolution_clock::now();
            threads.reserve(j);
            for(int q = 0;q < j; q++)
            {
                std::thread thr(func, std::ref(v),std::ref(numTasks), std::ref(i));
                threads.push_back(move(thr));
            }
            for(auto &t: threads)
            {
                t.join();
            }
            auto r = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration<double, std::milli>(r - l).count();
            std::cout << j << " threads in " << delta << " using atomic" <<std::endl;
            i = {0};
        }

    }
};
#endif //AVS_LAB2_TASK1_H
