
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <format>
#include <functional>
#include <iostream>
#include <latch>
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
    /*
     * Starts worker thread to process double buffering queue with constructor callable argument
     */
    actor_t(std::function<void(T)> process_work)
        : _process_work(std::move(process_work))
        , _worker(std::bind_front(&actor_t::run_loop, this))
    {
    }

    /*
     * Pushes a job request to the active work queue
     */
    void push(const T& work) {
        std::unique_lock lock{_mutex};
        _queue[_active_id].push_back(work);
        lock.unlock();
        _condv.notify_one();
    }

    /*
     * Terminates worker thread and completes unfinished work on callers thread
     */
    void finish_join() {
        _stop_requested = true;
        _condv.notify_one();
        _worker.join();
        run();
    }

    /*
     * Terminates worker thread possibly having unfinished jobs
     */
    void join() {
        _stop_requested = true;
        _condv.notify_one();
        _worker.join();
    }

    /*
     * Returns processed jobs counter
     */
    size_t get_processed_count() { return _counter; }

  private:
    void run_loop() {
        while (!_stop_requested)
            run();

std::cerr << std::format("Actor {:0>5} terminates after completing {} jobs.\n", std::this_thread::get_id(), size_t(_counter));
    }

    void run() {
        std::unique_lock lock{_mutex};
        while (_queue[_active_id].empty() && !_stop_requested)
            _condv.wait(lock);

        _active_id = 1 - _active_id;
        lock.unlock();

        auto& work_queue = _queue[1 - _active_id];
        for (const auto& w : work_queue)
            _process_work(w);

std::cerr << std::format("Actor {:0>5} next jobs chunk of size {}.\n", std::this_thread::get_id(), work_queue.size());
        _counter += work_queue.size();
        work_queue.clear();
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
std::vector<std::jthread> type1_workers(350);
std::vector<std::jthread> type2_workers(150);
std::latch job_scheduling_latch{static_cast<ptrdiff_t>(type1_workers.size() + type2_workers.size())};

struct work_t {
    std::thread::id thread_id{std::this_thread::get_id()};
    hr_clock_t::time_point created{hr_clock_t::now()};
    int message_id;
};

void work(const work_t& w) {
    // std::this_thread::sleep_for(duration_t{0.01});
    const duration_t duration = hr_clock_t::now() - w.created;
    std::cout << std::format("Executor {:0>5}, job {:0>2} from thread {:0>5} processed in {}.\n", std::this_thread::get_id(), w.message_id, w.thread_id, duration);
}

actor_t<work_t> superstar{work};

void type1_worker() {
    const int local_jobs_count = 7;
    total_jobs_count += local_jobs_count;

    work_t w;
    for (int i = 0; i < local_jobs_count; ++i) {
        std::this_thread::sleep_for(duration_t{200});
        w.message_id = i;
        superstar.push(w);
    }

    job_scheduling_latch.count_down();
}

void type2_worker() {
    const int local_jobs_count = 16;
    total_jobs_count += local_jobs_count;

    work_t w;
    for (int i = 0; i < local_jobs_count; ++i) {
        std::this_thread::sleep_for(duration_t{150});
        w.message_id = 100 + i;
        superstar.push(w);
    }

    job_scheduling_latch.count_down();
}

int main(int, char**)
{
    for (auto& t : type1_workers) t = std::jthread{type1_worker};
    for (auto& t : type2_workers) t = std::jthread{type2_worker};

    // do something useful instead
    std::this_thread::sleep_for(duration_t{500});

    job_scheduling_latch.wait();
    superstar.finish_join();
    std::cerr << std::format("Main thread {:0>5} finished. Progress ({}/{})\n", std::this_thread::get_id(), superstar.get_processed_count(), size_t(total_jobs_count));
}

/*

Format:
clang-format -i actor.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++23 actor.cpp -o actor.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address actor.cpp -o actor

*/
