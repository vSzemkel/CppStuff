
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>
#include <mutex>
#include <numeric>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

/*---------------------
Order cache exercise
-----------------------

Required order information: 
Order ID (string)     // e.g  "ID1" (unique id for order)
Security ID (string)  // e.g. "US9128473801"
Side (string)         // e.g. “Buy” or “Sell” 
Quantity (int)        // e.g. 120000
	User ID (string)      // e.g. “John Smith” 
	Company Name (string) // e.g. “Hedge Fund A” (company of User ID)

Example set of orders:
OrdId1 US9128473801 Buy  1000 User1 CompanyA
OrdId2 US5422358DA3 Sell 3000 User2 CompanyB
OrdId3 US9128473801 Sell  500 User3 CompanyA
OrdId4 US5422358DA3 Buy   600 User4 CompanyC
OrdId5 US5422358DA3 Buy   100 User5 CompanyB
OrdId6 US19635GY645 Buy  1000 User6 CompanyD
OrdId7 US5422358DA3 Buy  2000 User7 CompanyE
OrdId8 US5422358DA3 Sell 5000 User8 CompanyE


Design and implement the cache. The cache should support:

-	Adding orders
o	Function to add a single order to the cache

-	Cancelling orders
o	Cancel single order by Order ID
o	Cancel all orders for a given User ID
o	Cancel all orders for a given Security ID at or above a given quantity

-	Order matching opportunities for a Security ID
o	Given a Security ID, return total quantity that can match
o	Can only match a Buy order with a Sell order
o	Buy order can match against multiple Sell orders (and vice versa) if quantity allows
o	Any order quantity already allocated to a match cannot be reused as a match against a different order
o	Users in the same company cannot match against each other
o	E.g. given Security ID US5422358DA3
	OrdId2 matches quantity 600 against OrdId4 and quantity 2000 against OrdId7 for a total quantity of 2600
	OrdId8 matches quantity 100 against OrdId5 only 
	OrdId4 had its quantity fully allocated to match OrdId2 so none is left for the remaining 4900 of OrdId8
	Total quantity matched is 2700.  (2600 + 100) 
**/

enum class side_t {
    buy,
    sell
};

struct order_t {
    // for cancel operations
    order_t (std::string order_id) : _id(order_id) {};
    // for add operation
    order_t (
        std::string order_id,
        std::string security_id,
        std::string side,
        int quantity,
        std::string user_id,
        std::string company_name
    ) : _id(order_id), _security_id(security_id), _user_id(user_id), _company(company_name), _quantity(quantity) {
        if (quantity > 0 && !security_id.empty() && !user_id.empty() && (side == "Buy" || side == "Sell")) {
            _side = (side[0] == 'B') ? side_t::buy : side_t::sell;
        } else
            _id.clear();
    };

    bool operator==(const order_t& ord) const { return _id == ord._id; }

    std::string _id; // if empty then record is invalid
    std::string _security_id;
    std::string _user_id;
    std::string _company;
    side_t _side;
    int _quantity;
};

struct order_hasher_t {
    size_t operator()(const order_t& ord) const {
        return std::hash<std::string>()(ord._id);
    }
};

using order_set_t = std::unordered_set<order_t, order_hasher_t>;
using index_t = std::unordered_map<std::string, std::vector<std::string>>;

constexpr int large_cache_size = 1000000;
constexpr auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

class OrderCache
{
  public:
    void clear();
    bool add(order_t&& ord);
    int64_t match(const std::string& seq);
    bool cancel_by_id(const std::string& id);
    int cancel_by_user(const std::string& user);
    int cancel_by_seq_qt(const std::string& seq, int quantity);
  private:
    std::mutex _mx;
    order_set_t _orders;
    index_t _user_idx;
    index_t _security_idx;
};

void OrderCache::clear() {
    std::lock_guard lg{_mx};
    _user_idx.clear();
    _security_idx.clear();
    _orders.clear();
}

/** @brief Adds a new order to the cache
 *  @param ord Order rvalue reference
 * 
 * Orders with empty _id are invalid and ignored
 * For orderdes with existing _id this is update uperation
 */
bool OrderCache::add(order_t&& ord) {
    if (ord._id.empty())
        return false;
    {
        std::lock_guard lg{_mx};
        auto& user_idx = _user_idx[ord._user_id];
        auto insit = std::lower_bound(user_idx.begin(), user_idx.end(), ord._id);
        user_idx.insert(insit, ord._id);
        auto& security_idx = _security_idx[ord._security_id];
        insit = std::lower_bound(security_idx.begin(), security_idx.end(), ord._id);
        security_idx.insert(insit, ord._id);
        _orders.insert(std::move(ord));
        return true;
    }
}

/** @brief Cancells order by id
 *  @param id Order id
 * 
 * Removes the order from main container and updates indexes
 * Architect decided not to remove empty lists from indexes
 */
bool OrderCache::cancel_by_id(const std::string& id) {
    const auto ord = _orders.find(id);
    if (ord != _orders.end()) {
        std::lock_guard lg{_mx};
        // update _security_idx
        auto& orders_by_sec = _security_idx[ord->_security_id];
        const auto sit = std::lower_bound(orders_by_sec.begin(), orders_by_sec.end(), ord->_id);
        orders_by_sec.erase(sit);
        // update _user_id
        auto& orders_by_user = _user_idx[ord->_user_id];
        const auto uit = std::lower_bound(orders_by_user.begin(), orders_by_user.end(), ord->_id);
        orders_by_user.erase(uit);
        _orders.erase(ord);
        return true;
    }

    return false;
}

/** @brief Cancells order by id
 *  @param user User ID
 * 
 * Removes the orders from particular User ID
 * from main container and updates indexes
 * Architect decided not to remove empty lists from indexes
 */
int OrderCache::cancel_by_user(const std::string& user) {
    int ret{0};
    std::lock_guard lg{_mx};
    const auto candidates = _user_idx[user]; // must copy as erase() clobbers iterator
    for (const auto& id : candidates) {
        const auto ord = _orders.find(id);
        if (ord != _orders.end()) {
            // update _security_idx
            auto& orders_by_sec = _security_idx[ord->_security_id];
            const auto sit = std::lower_bound(orders_by_sec.begin(), orders_by_sec.end(), ord->_id);
            orders_by_sec.erase(sit);
            // update _user_id
            auto& orders_by_user = _user_idx[user];
            const auto uit = std::lower_bound(orders_by_user.begin(), orders_by_user.end(), ord->_id);
            orders_by_user.erase(uit);
            // cancel order
            _orders.erase(ord);
            ++ret;
        }
    }

    return ret;
}

/** @brief Cancells order by id
 *  @param seq Security ID
 *  @param quantity Order volume
 * 
 * Removes the orders for seq not less then quantity 
 * from main container and updates indexes
 * Architect decided not to remove empty lists from indexes
 */
int OrderCache::cancel_by_seq_qt(const std::string& seq, const int quantity) {
    int ret{0};
    std::lock_guard lg{_mx};
    const auto candidates = _security_idx[seq]; // must copy as erase() clobbers iterator
    for (const auto& id : candidates) {
        const auto ord = _orders.find(id);
        if (ord != _orders.end() && ord->_quantity >= quantity) {
            // update _security_idx
            auto& orders_by_sec = _security_idx[seq];
            const auto sit = std::find(orders_by_sec.begin(), orders_by_sec.end(), ord->_id);
            orders_by_sec.erase(sit);
            // update _user_id
            auto& orders_by_user = _user_idx[ord->_user_id];
            const auto uit = std::find(orders_by_user.begin(), orders_by_user.end(), ord->_id);
            orders_by_user.erase(uit);
            // cancel order
            _orders.erase(ord);
            ++ret;
        }
    }

    return ret;
}

/** @brief Computer matching total for orders on Security ID
 *  @param seq Security ID
 * 
 * STAGE1: divide buy and sell orders, grouping by company
 * STAGE2: compute matching
 * Both stages can be further optimized for huge data with map-reduce
 */
int64_t OrderCache::match(const std::string& seq) {
    int64_t ret{0};
    using match_t = std::pair<std::string, int64_t>; // {company, quantity}
    std::vector<match_t> sellers, buyers;

    {
        std::lock_guard lg{_mx};
        for (const auto& id : _security_idx[seq]) {
            const auto ord = _orders.find(id);
            if (ord == _orders.end()) continue;
            // CORRECT STRATEGY HERE DEPENDS ON DATA. 
            // SUPPOSE THERE ARE MANY ORDERS FROM SINGLE COMPANY, e.g. #ORDERS >> #COMPANIES
            auto& cont = (ord->_side == side_t::buy) ? buyers : sellers;
            const auto it = std::lower_bound(cont.begin(), cont.end(), match_t{ord->_company, 0});
            if (it != cont.end() && it->first == ord->_company)
                it->second += ord->_quantity;
            else
                cont.insert(it, {ord->_company, ord->_quantity});
        }
    }

    int mins{0};
    const int szb = (int)buyers.size(), szs = (int)sellers.size();
    for (int b = 0; b < szb; ++b) {
        auto& buy = buyers[b];
        for (int s = mins; s < szs; ++s) {
            auto& sell = sellers[s];
            if (buy.first != sell.first) {
                const auto volume = std::min(buy.second, sell.second);
                buy.second -= volume;
                sell.second -= volume;
                ret += volume;
            }
            if (s == mins && sell.second == 0) // more usefull if more #COMPANIES
                ++mins;
            if (buy.second == 0)
                continue;
        }
    }

    return ret;
}

OrderCache g_cache;

int generate_file(const std::string& path) {
    constexpr int sec_ids = 12;
    constexpr int user_ids = 5;
    constexpr int companies = 20;

    std::ofstream fout{path, std::ios_base::trunc};
    if (!fout.is_open()) return 0;

    const std::string sides[] = {"Buy", "Sell"};
    std::cout << "Generating " << large_cache_size << " records.." << '\n';
    for (int i = 0; i < large_cache_size; ++i) {
        std::string id = "ORDER" + std::to_string(i);
        std::string sec_id = "US" + std::to_string(2674959 + rand_in_range(sec_ids));
        std::string side = sides[rand_in_range(2)];
        int quantity = 100 + 50 * rand_in_range(20);
        int comp = rand_in_range(companies);
        std::string user = "USER" + std::string(1, 'A' + comp) + std::string(1, 'A' + rand_in_range(user_ids));
        std::string company = "COMPANY" + std::string(1, 'A' + comp);
        fout << id << " " << sec_id << " " << side << " " << quantity << " " << user << " " << company << '\n';
    }
    std::cout << "Generating finished\n";

    return sec_ids;
}

int load_from_file(const std::string& path) {
    std::ifstream fin{path};
    if (!fin.is_open()) return 0;

    int ret{0};
    std::cout << "Loading orders from " << path << '\n';
    g_cache.clear();
    while (fin.good()) {
        std::string id; fin >> id;
        std::string sec_id; fin >> sec_id;
        std::string side; fin >> side;
        int quantity; fin >> quantity;
        std::string user_id; fin >> user_id;
        std::string company_name; fin >> company_name;
        if (g_cache.add(order_t{id, sec_id, side, quantity, user_id, company_name}))
            ++ret;
        fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Orders loaded into the cache\n";
    return ret;
}

void basic_tests()
{
    order_t ord{"ORD1", "MSFT", "Buy", 1500, "Grazyna", "PKOBP"};
    g_cache.add(std::move(ord));
    assert(g_cache.cancel_by_id("ORD-1") == false);
    assert(g_cache.cancel_by_id("ORD1") == true);
    g_cache.add(order_t{"ORD2", "AMAZ", "Sell", 950, "Zofia", "mBank"});
    g_cache.add(order_t{"ORD3", "AMAZ", "Sell", 1200, "Adam", "TFI"});
    g_cache.add(order_t{"ORD4", "AMAZ", "Sell", 3000, "Marta", "Siusiak"});
    assert(g_cache.cancel_by_seq_qt("AMAZ", 5000) == 0);
    assert(g_cache.cancel_by_seq_qt("AMAZ", 2000) == 1);
    assert(g_cache.cancel_by_seq_qt("AMAZ", 5) == 2);
    g_cache.add(order_t{"ORD5", "AMAZ", "Sell", 3000, "Marfa", "Kowalska LTD"});
    g_cache.add(order_t{"ORD6", "AMAZ", "Buy", 120, "Marfa", "Kowalska LTD"});
    assert(g_cache.cancel_by_user("Marta") == 0);
    assert(g_cache.cancel_by_user("Marfa") == 2);

    load_from_file("order_cache.in");
    assert(g_cache.match("US5422358DA3") == 2700);
    // measure add performance
    const std::string lfn = "order_cache_large.in";
    const int sec_ids = generate_file(lfn);
    auto start = std::chrono::high_resolution_clock::now();
    const int cache_size = load_from_file(lfn);
    std::chrono::duration<double, std::milli> dur = std::chrono::high_resolution_clock::now() - start;
    std::cout << "Loading " << cache_size << " orders takes: " << dur.count() << " ms.\n";
    // measure match performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < sec_ids; ++i) {
        const std::string company = "US" + std::to_string(2674959 + i);
        std::cout << company + " orders matching size is: " << g_cache.match(company) << '\n';
    }
    dur = std::chrono::high_resolution_clock::now() - start;
    std::cout << "For " << cache_size << " orders, avarage match operation execution time: " << dur.count() / sec_ids << " ms.\n";
    // measure cancel_by_id performance
    constexpr int cancelling_size = 1000;
    std::unordered_set<std::string> cancelled;
    while (cancelled.size() < cancelling_size)
        cancelled.insert("ORDER" + std::to_string(rand_in_range(cancelling_size)));
    start = std::chrono::high_resolution_clock::now();
    for (const auto& ord_id : cancelled)
        g_cache.cancel_by_id(ord_id);
    dur = std::chrono::high_resolution_clock::now() - start;
    std::cout << "Cancelling by id operation avarage execution time: " << dur.count() / cancelling_size << " ms.\n";
}

int main(int, char**)
{
    basic_tests();
    return 0;
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 order_cache.cpp -o order_cache.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address order_cache.cpp -o order_cache
g++ -O3 -std=c++17 order_cache.cpp -o order_cache

Output:

Loading orders from order_cache.in
Orders loaded into the cache
Loading orders from order_cache_large.in
Orders loaded into the cache
US2674959 orders matching size is: 23832300
US2674960 orders matching size is: 23927300
US2674961 orders matching size is: 23962650
US2674962 orders matching size is: 23849350
US2674963 orders matching size is: 23919450
US2674964 orders matching size is: 24004350
US2674965 orders matching size is: 23720750
US2674966 orders matching size is: 23701250
US2674967 orders matching size is: 23758400
US2674968 orders matching size is: 23835350
US2674969 orders matching size is: 23891150
US2674970 orders matching size is: 23737350
For 1000000 orders, avarage match operation execution time: 49.4113 ms.

*/
