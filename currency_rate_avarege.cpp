
#include <chrono>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <ctime>
#include <numeric>


// case definition
constexpr int g_days_to_check = 30;
const wchar_t g_api[] = L"http://api.nbp.pl/api/exchangerates/rates/A/USD/%Y-%m-%d";

// globals
std::mutex g_mutex;
std::vector<double> g_rates;
std::atomic_bool g_finished{false};
std::condition_variable g_cv_finished;
std::vector<std::wstring> g_uris;
size_t g_valid_uris;

void init()
{
    tm local_time;
    wchar_t buf[60];
    auto now = std::chrono::system_clock::now();

    for (int i = 0; i < g_days_to_check; ++i) {
        std::time_t day = std::chrono::system_clock::to_time_t(now - i * std::chrono::hours(24));
        localtime_s(&local_time, &day);
        wcsftime(buf, _countof(buf), g_api, &local_time);
        g_uris.push_back(buf);
    }

    g_valid_uris = g_uris.size();
}

void get(std::wstring uri, std::function<void(const std::string&)> callback)
{
    web::http::client::http_client client(uri);
    Concurrency::streams::container_buffer<std::string> buf(std::ios_base::out);

    try {
        // captures by value in lambdas are critical as 'callback' parm is deallocated with current stack frame
        client.request(web::http::methods::GET)
            .then([=](web::http::http_response resp) {
                if (resp.status_code() == web::http::status_codes::OK) {
                    return resp.body().read_to_end(buf);
                } else {
                    // printf("Couldn't get from %S\n", uri.c_str());
                    return Concurrency::task_from_result<size_t>(0);
                }
            }).then([=](size_t len) {
                if (len <= 0)
                    --g_valid_uris;
                else
                    callback(buf.collection());
                }); // no wait() here, go unobserved
    } catch (web::http::http_exception he) {
        --g_valid_uris;
        printf("Error for uri %S: %s\n", uri.c_str(), he.what());
    }
}

void consume_all()
{
    const auto avg_rate = std::accumulate(g_rates.begin(), g_rates.end(), double{});
    printf("Avarage rate from last %llu days is %f\n", g_valid_uris, avg_rate / g_valid_uris);
    g_finished = true;
    g_cv_finished.notify_one();
}

void consume(const std::string& s)
{
    auto ws = std::make_unique<wchar_t[]>(s.size() + 1);
    mbstowcs_s(nullptr, ws.get(), s.size() + 1, s.c_str(), s.size());

    utility::stringstream_t ss;
    ss << ws;
    web::json::value v = web::json::value::parse(ss);
    auto rate = v[L"rates"][0][L"mid"].as_number().to_double();

    { // critical section
        std::lock_guard<std::mutex> lock{g_mutex};
        g_rates.push_back(rate);
        if (g_rates.size() == g_valid_uris)
            consume_all();
    }
}

int main()
{
    init();
    for (const auto& uri : g_uris)
        get(uri, consume);

    std::unique_lock<std::mutex> ul{g_mutex};
    g_cv_finished.wait(ul, []{ return g_finished == true; });

    return 0;
}

/* clang++.exe -O0 -Wall -g -std=c++17 -Ic:\GitHub\vcpkg\installed\x64-windows\include\ -lc:\GitHub\vcpkg\installed\x64-windows\lib\cpprest_2_10.lib currency_rate_avarege.cpp -o currency_rate_avarege.exe

Output:

Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-15
Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-07-26
Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-21
Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-01
Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-09
Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-07-25
Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-02
Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-16
Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-08
Avarage rate from last 21 days is 3.736895

*/
