//
// Created by andrejbobr on 19.11.2020.
//

#ifndef AVS_LAB2_FIXEDMUTEXQUEUE_H
#define AVS_LAB2_FIXEDMUTEXQUEUE_H

class FixedMutexQueue{
private:
    const unsigned int size;
    unsigned int curr_size;
    unsigned int head, tail;
    std::mutex m;
    std::condition_variable cv_push, cv_pop;
    int * q;
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
    explicit FixedMutexQueue(int size) : size(size), curr_size(0), head(0), tail(0)
    {
        q = new int [size];
    }
    ~FixedMutexQueue()
    {
        delete[] q;
    }
};

#endif //AVS_LAB2_FIXEDMUTEXQUEUE_H
