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

int main() {
    task1::mutex_counter();
    //task1::atomic_counter();

    return 0;
}
