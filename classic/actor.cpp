
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <format>
#include <functional>
#include <iostream>
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
        , _worker(std::bind_front(&actor_t::run, this))
    {
    }

    ~actor_t() {
        _condv.notify_one();
        _stop_requested = true;
        _condv.notify_one();
    }

    void push(const T& work) {
        std::unique_lock lock{_mutex};
        _queue[_active_id].push_back(work);
        lock.unlock();
        _condv.notify_one();
    }

    size_t get_processed_count() { return _counter; }

  private:
    void run() {
        while (!_stop_requested) {
            std::unique_lock lock{_mutex};
            while (_queue[_active_id].empty() && !_stop_requested)
                _condv.wait(lock);

            _active_id = 1 - _active_id;
            lock.unlock();

            auto& work_queue = _queue[1 - _active_id];
            for (const auto& w : work_queue)
                _process_work(w);
            _counter += work_queue.size();
            work_queue.clear();
        }

        std::cout << std::format("Actor {:0>5} terminates after completing {} jobs.\n", std::this_thread::get_id(), size_t(_counter));
    }

    int _active_id{};
    std::vector<T> _queue[2];
    std::function<void(T)> _process_work;
    std::jthread _worker;
    std::atomic<size_t> _counter{};
    std::mutex _mutex;
    std::condition_variable _condv;
    std::atomic<bool> _stop_requested{};
};

/***************************** TESTING ************************** */

using hr_clock_t = std::chrono::high_resolution_clock;
using duration_t = std::chrono::duration<double, std::milli>;

std::atomic<size_t> total_jobs_count{};

struct work_t {
    std::thread::id thread_id{std::this_thread::get_id()};
    hr_clock_t::time_point created{hr_clock_t::now()};
    int message_id;
};

void work(const work_t& w) {
    std::this_thread::sleep_for(duration_t{1});
    const duration_t duration = hr_clock_t::now() - w.created;
    std::cout << std::format("Executor {:0>5}, job {:0>2} from thread {:0>5} processed in {} ms.\n", std::this_thread::get_id(), w.message_id, w.thread_id, duration);
}

actor_t<work_t> superstar{work};

void type1_worker() {
    const int local_jobs_count = 30;
    total_jobs_count += local_jobs_count;

    work_t w;
    for (int i = 0; i < local_jobs_count; ++i) {
        w.message_id = i;
        superstar.push(w);
    }
}

void type2_worker() {
    const int local_jobs_count = 50;
    total_jobs_count += local_jobs_count;

    work_t w;
    for (int i = 0; i < local_jobs_count; ++i) {
        w.message_id = 100 + i;
        superstar.push(w);
    }
}

int main(int, char**)
{
    std::jthread wt11(type1_worker);
    std::jthread wt12(type1_worker);
    std::jthread wt2(type2_worker);
    wt11.join();
    wt12.join();
    wt2.join();

    std::this_thread::sleep_for(duration_t{2000});
    std::cout << std::format("Main thread {:0>5} finished. Progress ({}/{})\n", std::this_thread::get_id(), superstar.get_processed_count(), size_t(total_jobs_count));
}

/*

Format:
clang-format -i actor.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++23 actor.cpp -o actor.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address actor.cpp -o actor

*/
