
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <format>
#include <fstream>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

// Actor Model
// Actor keeps working thread and queue of request to process
// Any thread can shedule work to be processed asynchronously on actor thread

template <typename T>
class actor_t
{
  public:
    actor_t(std::function<void(T)> process_work)
        : _process_work(std::move(process_work))
        , _worker([this](){ this->run(); })
    {
    }

    void push(const T& work) {
        std::unique_lock lock{_mutex};
        _queue[_active_id].push_back(work);
        lock.unlock();
        _condv.notify_one();
    }

  private:
    void run() {
        std::unique_lock lock{_mutex};
        while (_queue[_active_id].empty())
            _condv.wait(lock);

        _active_id = 1 - _active_id;
        lock.unlock();

        auto& work_queue = _queue[1 - _active_id];
        for (const auto& w : work_queue)
            _process_work(w);
        work_queue.clear();
    }

    int _active_id{};
    std::vector<T> _queue[2];
    std::function<void(T)> _process_work;
    std::mutex _mutex;
    std::condition_variable _condv;
    std::jthread _worker;
};

/***************************** TESTING ************************** */

using hr_clock_t = std::chrono::high_resolution_clock;
using duration_t = std::chrono::duration<double, std::milli>;

std::ofstream log_file("actor.log");

struct work_t {
    std::thread::id thread_id{std::this_thread::get_id()};
    hr_clock_t::time_point created{hr_clock_t::now()};
    int message_id;
};

void work(const work_t& w) {
    std::this_thread::sleep_for(duration_t{1});
    const duration_t duration = hr_clock_t::now() - w.created;
    log_file << std::format("Executor {:0>5}, job {:0>2} from thread {:0>5} processed in {} ms.\n", std::this_thread::get_id(), w.message_id, w.thread_id, duration);
}

actor_t<work_t> superstar{work};

void type1_worker() {
    work_t w;
    for (int i = 0; i < 1000; ++i) {
        w.message_id = i;
        superstar.push(w);
    }
}

void type2_worker() {
    work_t w;
    for (int i = 0; i < 1000; ++i) {
        w.message_id = 100 + i;
        superstar.push(w);
    }
}

int main(int, char**)
{
    std::jthread wt1(type1_worker);
    std::jthread wt2(type2_worker);
    wt1.join();
    wt2.join();
}

/*

Format:
clang-format -i actor.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++23 actor.cpp -o actor.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address actor.cpp -o actor

*/
