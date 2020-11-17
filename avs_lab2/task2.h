//
// Created by andrejbobr on 10.11.2020.
//

#ifndef AVS_LAB2_TASK2_H
#define AVS_LAB2_TASK2_H


class DynamicQueue{
private:

    std::mutex m;
    std::queue<int> q;
public:

    void push(int val)
    {
        m.lock();
        q.push(val);
        m.unlock();
    }
    bool pop(int& val)
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
        std::atomic<int> counter{0};
        auto producer = [&](){
            for(int i = 0;i < taskNum;i++)
            {
                q.push(1);
            }
          // return 0;
        };
        auto consumer = [&](){
            int items = taskNum * prods / cons ;
            int tmp;
            for(int i = 0;i < items;i++)
            {
               q.pop(tmp);
               counter += tmp;
            }
           // return counter;
        };
        std::vector<std::thread> prod_thr;
        std::vector<std::thread> cons_thr;
        prod_thr.reserve(prods);
        cons_thr.reserve(cons);
        for(int i = 0;i < prods;i++)
        {
            std::thread thr(producer);
            prod_thr.push_back(move(thr));
        }
        for(int i = 0;i < cons;i++)
        {
            std::thread thr(consumer);
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
        std::cout << "Cons " << cons << " Prods " << prods << " sum " << counter << " | " << prods * taskNum << std::endl;
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
