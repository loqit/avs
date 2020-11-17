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

#include "task1.h"
#include "task2.h"
#include "task2_v2.h"
#include "task2_v3.h"


int main() {
    //task1::mutex_counter();
    //task1::atomic_counter();
    task2::task();
   // task2_v2::task();
   //task2_v3::task();
    return 0;
}
