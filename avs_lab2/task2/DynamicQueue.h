//
// Created by andrejbobr on 19.11.2020.
//

#ifndef AVS_LAB2_DYNAMICQUEUE_H
#define AVS_LAB2_DYNAMICQUEUE_H

//template< typename T >
class DynamicQueue {
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
    bool pop(int & val)
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

#endif //AVS_LAB2_DYNAMICQUEUE_H
