
#include <array>
#include <condition_variable>
#include <intrin.h>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// case definition
constexpr size_t g_produced_pack_size = 5;
constexpr size_t g_producers_count = 6;
constexpr size_t g_consumed_pack_size = 10;
constexpr size_t g_consumers_count = 3;
constexpr size_t g_storage_size = 10;
// logging
unsigned int g_line = 0;
std::mutex g_printf_mx;
std::vector<std::string> g_log;

struct Item
{
    std::thread::id producer_id;
    int item_id;
};

template <typename T, size_t N>
class Magazine
{
  public:
    void put(T&& item) {
        std::unique_lock<std::mutex> ul(_m);
        while (_size >= N)
            _cv.wait(ul);

        const size_t ind = (_start + _size) % N;
        _storage[ind] = std::move(item);
        ++_tick;
        ++_size;
        ul.unlock();
        _cv.notify_all();
    }

    T get() {
        std::unique_lock<std::mutex> ul(_m);
        while (_size < 1)
            _cv.wait(ul);

        T item = std::move(_storage[_start]);
        _start = (_start + 1) % N;
        --_size;
        ++_tick;
        ul.unlock();
        _cv.notify_all();
        return item;
    }

    void check() {
        if (_size != 0 || _tick != 2 * g_produced_pack_size * g_producers_count)
            throw std::exception("Fraud detected");
        for (const auto& s : g_log)
            puts(s.c_str());
    }

  private:
    size_t _start{0};
    size_t _size{0};
    size_t _tick{0};
    std::mutex _m;
    std::condition_variable _cv;
    std::array<T, N> _storage{};
};

Magazine<Item, g_storage_size> g_magazine;

void produce(int order)
{
    int id = 0;
    while (order--) {
        char buf[100];
        sprintf_s(buf, sizeof(buf), "[%03u-%llu] Produced: [%u, %i]", g_line++, __rdtsc(), *(unsigned int*)&std::this_thread::get_id(), id);
        g_magazine.put(Item{std::this_thread::get_id(), ++id});
        {
            std::lock_guard lg{g_printf_mx};
            g_log.push_back(buf);
        }
    }
}

void consume(int order)
{
    while (order--) {
        const auto item = g_magazine.get();
        char buf[100];
        sprintf_s(buf, sizeof(buf), "[%03u-%llu] Consumed by: %u [%u, %i]", g_line++, __rdtsc(), *(unsigned int*)&std::this_thread::get_id(), *(unsigned int*)&item.producer_id, item.item_id);
        {
            std::lock_guard lg{g_printf_mx};
            g_log.push_back(buf);
        }
    }
}

int main()
{
    static_assert(g_produced_pack_size * g_producers_count == g_consumed_pack_size * g_consumers_count);
    // work
    std::vector<std::thread> threads_to_join;
    for (int i = 0; i < g_producers_count; ++i)
        threads_to_join.push_back(std::thread(produce, g_produced_pack_size));
    for (int i = 0; i < g_consumers_count; ++i)
        threads_to_join.push_back(std::thread(consume, g_consumed_pack_size));
    // join
    for (std::thread& t : threads_to_join)
        t.join();
    // present result
    g_magazine.check();
}

/*

Sample output:

[000-91016767183217] Produced: [14824, 1]
[001-91016767360362] Produced: [14824, 2]
[002-91016767379316] Produced: [14824, 3]
[003-91016767398691] Produced: [14824, 4]
[004-91016767452038] Produced: [14824, 5]
[005-91016767756222] Produced: [14064, 1]
[006-91016767780541] Produced: [14064, 2]
[007-91016767812648] Produced: [14064, 3]
[008-91016767824199] Produced: [14064, 4]
[009-91016767835137] Produced: [14064, 5]
[010-91016769105312] Consumed by: 20000 [14824, 1]
[011-91016769136004] Consumed by: 20000 [14824, 2]
[012-91016769155217] Consumed by: 20000 [14824, 3]
[013-91016769172464] Produced: [6972, 1]
[014-91016769250172] Consumed by: 21120 [14064, 1]
[015-91016769288265] Consumed by: 21120 [14064, 2]
[016-91016769305711] Consumed by: 21120 [14064, 3]
[017-91016769350868] Consumed by: 21120 [14064, 4]
[018-91016769407453] Consumed by: 21120 [14064, 5]
[019-91016769424703] Consumed by: 21120 [22764, 1]
[020-91016769500942] Consumed by: 21120 [15404, 1]
[021-91016769518117] Consumed by: 21120 [6972, 1]
[022-91016769536437] Produced: [15404, 1]
[023-91016769583733] Produced: [15404, 2]
[024-91016769603977] Produced: [15404, 3]
[025-91016769621360] Produced: [15404, 4]
[026-91016769635590] Produced: [15404, 5]
[027-91016769661304] Produced: [22764, 1]
[028-91016769694822] Produced: [22764, 2]
[029-91016769816742] Produced: [22764, 3]
[030-91016769830342] Produced: [22764, 4]
[031-91016769882207] Produced: [22764, 5]
[032-91016769898830] Consumed by: 20000 [14824, 4]
[033-91016769939691] Consumed by: 20000 [6972, 2]
[034-91016769960427] Consumed by: 20000 [15404, 2]
[035-91016769975846] Consumed by: 20000 [15404, 3]
[036-91016769989809] Consumed by: 20000 [15404, 4]
[037-91016770003576] Consumed by: 20000 [15404, 5]
[038-91016770017310] Consumed by: 20000 [22764, 2]
[039-91016770101730] Consumed by: 23936 [14824, 5]
[040-91016770135529] Consumed by: 23936 [22764, 3]
[041-91016770151162] Consumed by: 23936 [22764, 4]
[042-91016770165826] Consumed by: 23936 [22764, 5]
[043-91016770306841] Produced: [24556, 1]
[044-91016770344139] Produced: [24556, 2]
[045-91016770358915] Produced: [24556, 3]
[046-91016770403360] Produced: [24556, 4]
[047-91016770449563] Produced: [24556, 5]
[048-91016770468649] Consumed by: 23936 [24556, 2]
[049-91016770491644] Consumed by: 23936 [24556, 3]
[050-91016770508645] Consumed by: 23936 [24556, 4]
[051-91016770523791] Consumed by: 23936 [24556, 5]
[052-91016770630387] Consumed by: 21120 [24556, 1]
[053-91016770684465] Produced: [6972, 2]
[054-91016770716282] Produced: [6972, 3]
[055-91016770729082] Consumed by: 23936 [6972, 3]
[056-91016770778913] Consumed by: 23936 [6972, 4]
[057-91016770826672] Produced: [6972, 4]
[058-91016770845440] Produced: [6972, 5]
[059-91016770931314] Consumed by: 21120 [6972, 5]

*/
