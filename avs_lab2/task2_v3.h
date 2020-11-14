//
// Created by andrejbobr on 14.11.2020.
//

#ifndef AVS_LAB2_TASK2_V3_H
#define AVS_LAB2_TASK2_V3_H

class FixedAtomicQueue{
private:
    std::atomic<int> tail, head;
    int *q;
    const unsigned int size;

public:
    explicit FixedAtomicQueue(int size) : size(size), tail(0), head(0)
    {
        q = new int[size];
        for (int i = 0; i < size; i++) q[i] = 0;
    }

    void push(int val)
    {
        while (true)
        {
            int tl = tail.load();
            int tmp = q[tl % size];
            if(tmp != 0) continue;
            if(tail.compare_exchange_strong(tl, tl + 1))
            {
                q[tl % size] = val;
                return;
            }
        }
    }

    bool pop(int& val)
    {
        while(true)
        {
            int hl = head.load();
            if(head == tail) continue;
            val = q[hl % size];
            if(val == 0) continue;
            if(head.compare_exchange_strong(hl, hl + 1))
            {
                q[hl % size] = 0;
                return true;
            }
        }
        return false;
    }


};
class task2_v3{

    static void prodCon(int taskNum, int prods, int cons, int size)
    {
        FixedAtomicQueue q(size);
        std::atomic<int> counter(0);

        auto producer = [&](){

            for(int i = 0;i < taskNum;i++)
            {
                q.push(1);
            }
        };
        auto consumer = [&](){

            int items = taskNum * prods / cons;
            int tmp;
            for(int i = 0;i < items ;i++)
            {
                while(!q.pop(tmp)) {}
                counter += tmp;
            }
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
         std::cout << "Join consumers" << std::endl;
        for(auto &t: cons_thr)
        {
            if(t.joinable())
            {
                t.join();
            }
        }
        std::cout << "Sum " << counter << " / " << prods * taskNum << "; Queue size " << size << std::endl;
    }

public:
    static void task()
    {
        int taskNum = 1024;
        int prodNum[] = {1, 2, 4};
        int consNum[] = {1, 2, 4};
        int queueSize[] = {1, 4, 16};
        for(int j: consNum){
            for (int i: prodNum) {
                std::cout << "Producers : " << i << " Consumers : " << j << std::endl;
                for (int s: queueSize) {
                    auto l = std::chrono::high_resolution_clock::now();
                    //std::cout << "Enter prodCon" << std::endl;
                    prodCon(taskNum, i, j, s);
                    // std::cout << "Out prodCon" << std::endl;
                    auto r = std::chrono::high_resolution_clock::now();
                    auto delta = std::chrono::duration<double, std::milli>(r - l);
                    std::cout << "Time: " << delta.count() << std::endl << std::endl;

                }
                std::cout << "===================================" << std::endl << std::endl;
            }
        }
    }
};

#endif //AVS_LAB2_TASK2_V3_H
