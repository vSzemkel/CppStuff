
#define  _CRT_SECURE_NO_WARNINGS 1

#include <numeric>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

std::mutex g_mutex;
std::vector<double> g_rates;
std::atomic_bool g_finished{false};
std::vector<const wchar_t*> g_uris = {
    L"http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-03",
    //L"http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-XX", // this one is bad
    L"http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-12",
    L"http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-20"
};
size_t g_valid_uris = g_uris.size();

void get(const wchar_t* uri, std::function<void(const std::string&)> callback)
{
    web::http::client::http_client client(uri);
    Concurrency::streams::container_buffer<std::string> buf(std::ios_base::out);

    try {
        client.request(web::http::methods::GET)
            .then([&](web::http::http_response resp) {
                if (resp.status_code() == web::http::status_codes::OK) {
                    return resp.body().read_to_end(buf);
                } else {
                    printf("Couldn't get from %S\n", uri);
                    return Concurrency::task_from_result<size_t>(0);
                }
            }).then([&](size_t len) {
                if (len <= 0)
                    --g_valid_uris;
                else
                    callback(buf.collection());
            }).wait(); // without this wait it crashes as buf is deallocted prematurely
    } catch (web::http::http_exception he) {
        --g_valid_uris;
        printf("Error for uri %S: %s\n", uri, he.what());
    }
}

void consume_all()
{
    const auto avg_rate = std::accumulate(g_rates.begin(), g_rates.end(), double{});
    printf("Avarage rate from %llu days is %f\n", g_valid_uris, avg_rate / g_valid_uris);
    g_finished = true;
}

void consume(const std::string& s)
{
    std::wstring ws(s.size(), L' '); // Overestimate number of code points.
    mbstowcs(const_cast<wchar_t*>(ws.data()), s.c_str(), s.size());
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
    for (const auto uri : g_uris)
        get(uri, consume);

    // with wait() at line 42 this join is not nessesery
    std::unique_lock<std::mutex> ul{g_mutex};
    std::condition_variable cv;
    while (!g_finished)
        cv.wait(ul);

    return 0;
}

/*

Output:

Couldn't get from http://api.nbp.pl/api/exchangerates/rates/A/USD/2020-08-XX
Avarage rate from 3 days is 3.740700

*/
