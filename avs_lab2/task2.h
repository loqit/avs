//
// Created by andrejbobr on 10.11.2020.
//

#ifndef AVS_LAB2_TASK2_H
#define AVS_LAB2_TASK2_H


class DynamicQueue{
private:

    std::mutex m;
    std::queue<uint8_t> q;
    std::condition_variable cv;
public:

    void push(uint8_t val)
    {
        std::unique_lock<std::mutex> locker (this->m);
        q.push(val);
        cv.notify_one();
    }
    bool pop(uint8_t& val)
    {
        m.lock();
        if(q.empty())
        {
            m.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            m.lock();
            if(q.empty())
            {
                m.unlock();
                return false;
            }
        }
        val = q.front();
        q.pop();
        m.unlock();
        return true;
    }

};

class task2{
public:
    static void prodCon(int taskNum, int prods, int cons)
    {
        DynamicQueue q;
        auto producer = [&](){
            for(int i = 0;i < taskNum;i++)
            {
                q.push(1);
            }
            return 0;
        };
        auto consumer = [&](){
            int counter = 0;
            int items = taskNum * prods / cons ;
            for(int i = 0;i < items;i++)
            {
               uint8_t tmp;
               q.pop(tmp);
               counter += tmp;
            }
            return counter;
        };
        std::vector<std::future<int>> threads;
        threads.reserve(prods + cons);
        for(int i = 0;i < prods;i++)
        {
            threads.emplace_back(std::async(producer));
        }
        for(int i = 0;i < cons;i++)
        {
            threads.emplace_back(std::async(consumer));
        }
        int sum = 0;
        for(int i = 0;i < prods + cons;i++)
        {
            sum += threads[i].get();
        }
        std::cout << "Cons " << cons << " Prods " << prods << " sum " << sum << " | " << prods * taskNum << std::endl;
    }

    static void task()
    {
        int taskNum = 4 * 1024 * 1024;
        int prodNum[] = {1, 2, 4};
        int consNum[] = {1, 2, 4};
        for (int i: prodNum) {
            for (int j: consNum) {
                auto l = std::chrono::high_resolution_clock::now();
                prodCon(taskNum, i, j);
                auto r = std::chrono::high_resolution_clock::now();
                auto delta = std::chrono::duration<double, std::milli>(r - l).count();
                std::cout << "Time " << delta << std::endl;
            }
        }
    }
};

#endif //AVS_LAB2_TASK2_H
