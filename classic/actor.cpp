
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
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
        : _process_work(std::move(process_work)) {
        // TODO create worker thread
    }

    void push(const T& work) {

    }

  private:
    void run() {

    }

    int _active_id{};
    std::vector<T> queue[2];
    std::vector<T>& _active_queue = queue[0];
    std::function<void(T)> _process_work;
};

/***************************** TESTING ************************** */

using hr_clock_t = std::chrono::high_resolution_clock;
using duration_t = std::chrono::duration<double, std::milli>;

struct work_t {
    std::thread::id thread_id{std::this_thread::get_id()};
    hr_clock_t::time_point created{hr_clock_t::now()};
    int message_id;
};

void work(const work_t& w) {
    std::this_thread::sleep_for(duration_t{1});
    const duration_t duration = hr_clock_t::now() - w.created;
    std::cout << std::format("Job {:0>2} from thread {:0>5} processed in {} ms.\n", w.message_id, w.thread_id, duration);
}

int main(int, char**)
{
    work_t w;
    w.message_id = 3;
    work(w);
}

/*

Format:
clang-format -i actor.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++23 actor.cpp -o actor.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address actor.cpp -o actor

*/
