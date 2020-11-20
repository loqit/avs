//
// Created by andrejbobr on 19.11.2020.
//

#ifndef AVS_LAB2_FIXEDATOMICQUEUE_H
#define AVS_LAB2_FIXEDATOMICQUEUE_H


class FixedAtomicQueue{
private:
    std::atomic<int> tail, head;
    int *q;
    const unsigned int size;

public:
    explicit FixedAtomicQueue(int size) : size(size), tail(0), head(0)
    {
        q = new int [size];
        for (int i = 0; i < size; i++) q[i] = 0;
    }

    void push(int val)
    {
        while (true)
        {
            int tl = tail.load();
            if(tl == head + size)continue;
            if(tl != tail)continue;
            int tmp = q[tl % size];
            if(tmp != 0) continue;
            if(tail.compare_exchange_strong(tl, tl + 1))
            {
                q[tl % size] = val;
                return;
            }
        }
    }

    bool pop(int & val)
    {
        while(true)
        {
            int hl = head.load();
            if(head == tail)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                hl = head.load();
                if(head == tail)
                {
                    return false;
                }
            }
            int tmp = q[hl % size];
            if(tmp == 0 || hl != head) continue;
            if(head.compare_exchange_strong(hl, hl + 1))
            {
                val = tmp;
                q[hl % size] = 0;
                return true;
            }
        }
        return true;
    }


};

#endif //AVS_LAB2_FIXEDATOMICQUEUE_H
