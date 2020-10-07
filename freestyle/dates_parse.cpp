#include <algorithm>
#include <iostream>
#include <intrin.h>
#include <time.h>
#include <fstream>
#include <numeric>
#include <map>
#include <mutex>
#include <thread>
#include <vector>


struct Trade {
    time_t when;
    float rate;
    int volume;
    std::string stock;
};

constexpr int g_count = 100;

std::mutex g_mutex;
time_t g_start, g_finish;
std::vector<Trade> g_trades;
std::map<std::string, int> g_map_zone;

void collect_line(const Trade& t, int hour)
{
    if (g_start <= t.when && t.when < g_finish && 10 <= hour && hour < 12) {
        std::lock_guard<std::mutex> lg{g_mutex};
        g_trades.push_back(t);
    }
}

void process_file(int i)
{
    char filename[20], line[256];
    sprintf_s(filename, sizeof(filename), "data%03i.txt", i);
    std::ifstream f{filename};
    std::string str;
    while (f.getline(line, sizeof(line))) {
        tm t{0};
        int ignore, vol;
        char curr[4], stock[5];
        float rate;
        sscanf_s(line, "%04u%02u%02u %02u:%02u:%02u.%06u %s %f %d %s", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec, &ignore, curr, sizeof(curr), &rate, &vol, stock, sizeof(stock));
        t.tm_year -= 1900;
        if (strcmp(curr, "BTC") == 0)
            collect_line(Trade{_mktime64(&t), rate, vol, stock}, t.tm_hour);
    }

}

int main()
{
    // 10:00:00.000000 and 11:59:59.999999 for the date range of 20170601 to 20180201
    struct tm st { 0 };
    st.tm_year = 2017 - 1900;
    st.tm_mon = 6;
    st.tm_mday = 1;
    g_start = _mktime64(&st);
    struct tm fi { 0 };
    fi.tm_year = 2018 - 1900;
    fi.tm_mon = 2;
    fi.tm_mday = 2;
    g_finish = _mktime64(&fi);

    std::vector<std::thread> threads_to_join;
    for (int i = 0; i < g_count; ++i)
        threads_to_join.push_back(std::thread(process_file, i));
    // join
    for (std::thread& t : threads_to_join)
        t.join();
    // compute
    int count = g_trades.size();
    float sum = std::accumulate(g_trades.begin(), g_trades.end(), 0.0f, [](float s, const Trade& t) { return s + t.rate * t.volume; });
    float absmean = std::accumulate(g_trades.begin(), g_trades.end(), 0.0f, [](float s, const Trade& t) { return s + fabs(t.rate * t.volume); }) / count;
    std::cout << "Count: " << count << ", sum: " << sum << ", absmean: " << absmean << "\n";
}

/*

Sample log:

20180104 10:35:42.920471 BTC 7129.29 -29 GDAX
20180104 10:35:43.380511 BTC 7124.21 15 BTSP
20180104 10:35:45.225692 BTC 7129.15 -57 BTCX
20180104 10:35:45.749711 ETH 810.42 129 GDAX
20180104 10:35:47.837951 BTC 7125.01 32 POLO
20180104 10:35:51.157252 BTC 7124.67 7 FLYR
20180104 10:36:39.179241 ETH 813.24 -291 ETHF

*/
