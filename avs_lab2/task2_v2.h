//
// Created by andrejbobr on 11.11.2020.
//

#ifndef AVS_LAB2_TASK2_V2_H
#define AVS_LAB2_TASK2_V2_H

class FixedQueue{
private:
    const unsigned int size;
    unsigned int curr_size;
    unsigned int head, tail;
    std::mutex m;
    std::condition_variable cv_push, cv_pop;
    int* q;
public:
    void push(int val)
    {
        std::unique_lock<std::mutex> locker (m);

        cv_push.wait(locker, [&] { return size > curr_size; });

        q[tail] = val;
        tail = (tail + 1) % size;
        curr_size++;
        cv_pop.notify_one();

    }
    bool pop(int & val)
    {
        std::unique_lock<std::mutex> locker (m);

        if (cv_pop.wait_for(locker, std::chrono::milliseconds(1), [&] { return curr_size > 0; }))
        {
            val = q[head];
            q[head] = 0;
            head = (head + 1) % size;
            curr_size--;
            cv_push.notify_one();

            return true;
        }
        else
            return false;
    }
    explicit FixedQueue(int size) : size(size), curr_size(0), head(0), tail(0)
    {
        q = new int [size];
    }
    ~FixedQueue()
    {
        delete[] q;
    }
};

class task2_v2{

    static void prodCon(int taskNum, int prods, int cons, int size)
    {
        FixedQueue q(size);
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
        int taskNum = 4 * 1024 * 1024;
        int prodNum[] = {1, 2, 4};
        int consNum[] = {1, 2, 4};
        int queueSize[] = {1, 4, 16};
        for(int j: consNum){
            for (int i: prodNum) {
                std::cout << "Producers : " << i << " Consumers : " << j << std::endl;
                for (int s: queueSize) {
                    auto l = std::chrono::high_resolution_clock::now();
                    prodCon(taskNum, i, j, s);
                    auto r = std::chrono::high_resolution_clock::now();
                    auto delta = std::chrono::duration<double, std::milli>(r - l);
                    std::cout << "Time: " << delta.count() << std::endl << std::endl;

                }
                std::cout << "===================================" << std::endl << std::endl;
            }
        }
    }
};

#endif //AVS_LAB2_TASK2_V2_H
