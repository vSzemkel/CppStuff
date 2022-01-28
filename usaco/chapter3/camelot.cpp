/*
ID: marcin.8
LANG: C++
TASK: camelot
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=unnUOe16hgm&S=camelot
*/

#include <array>
#include <fstream>
#include <queue>
#include <vector>

std::ifstream task_in("camelot.in");
std::ofstream task_out("camelot.out");

constexpr int INF = 1e08; // times 10 must not overflow

std::vector<int> knight_distance;
std::vector<bool> in_queue;
std::queue<int> qq;

using pos_t = std::pair<int, int>;

const pos_t singularity{1, 1};

static pos_t diff(const pos_t& pos, const int r, const int c) {
    return {std::abs(pos.first - r), std::abs(pos.second - c)};
}

static pos_t diff(const pos_t& pos1, const pos_t& pos2) {
    return {std::abs(pos1.first - pos2.first), std::abs(pos1.second - pos2.second)};
}

/**
 * @brief Precompute knight distances between every two cells on board
 * 
 * @param rows Board's height
 * @param cols Board's width
 */
static void init_knight_distance(const int rows, const int cols) {
    const auto sz = rows * cols;
    knight_distance.assign(sz, INF);
    in_queue.assign(sz, false);

    const auto dijkstra = [](const int cost, const int next) {
        if (cost < knight_distance[next]) {
            knight_distance[next] = cost;
            if (!in_queue[next]) {
                in_queue[next] = true;
                qq.push(next);
            }
        }
    };

    knight_distance[0] = 0;
    qq.push(0);
    in_queue[0] = true;
    while (!qq.empty()) {
        const int cell = qq.front(); qq.pop();
        in_queue[0] = false;
        const int r = cell / cols;
        const int c = cell % cols;
        const int cost = knight_distance[r * cols + c] + 1;
        if (c + 2 < cols) {
            if (r > 0) dijkstra(cost, (r - 1) * cols + c + 2);
            if (r < rows - 1) dijkstra(cost, (r + 1) * cols + c + 2);
        }
        if (0 <= c - 2) {
            if (r > 0) dijkstra(cost, (r - 1) * cols + c - 2);
            if (r < rows - 1) dijkstra(cost, (r + 1) * cols + c - 2);
        }
        if (r + 2 < rows) {
            if (c > 0) dijkstra(cost, (r + 2) * cols + c - 1);
            if (c < cols - 1) dijkstra(cost, (r + 2) * cols + c + 1);
        }
        if (0 <= r - 2) {
            if (c > 0) dijkstra(cost, (r - 2) * cols + c - 1);
            if (c < cols - 1) dijkstra(cost, (r - 2) * cols + c + 1);
        }
    }
}

/**
 * @brief Knight approaches the king, king may move itself.
 * They are to meeting with minimal number of moves
 * Store new knight position
 * @param knight out - Its position is stored here
 * @param king 
 * @return int Number of moves
 */
static int give_a_ride(pos_t& knight, const pos_t& king) {
    const int sig_r = (king.first >= knight.first) ? 1 : -1;
    const int sig_c = (king.second >= knight.second) ? 1 : -1;
    auto dist = diff(knight, king);
    int ret{0}, r{dist.first}, c{dist.second};
    while (r + c > 3) {
        if (r < c) {
            c -= 2;
            r ? --r : ++r;
        } else {
            r -= 2;
            c ? --c : ++c;
        }
        ++ret;
    }

    if (r == 3 || c == 3) {
        r = c = 1;
        ++ret;
    } else if (r * c == 2) {
        r = c = 0;
        ++ret;
    }

    knight.first = king.first - r * sig_r;
    knight.second = king.second - c * sig_c;
    dist = diff(knight, king);

    return ret + std::max(dist.first, dist.second); // knight moves + king moves
}

int main(int, char**)
{
    int R, C;
    task_in >> R >> C;
    int r; char c;
    task_in >> c >> r >> std::ws;
    pos_t king = {r - 1, c - 'A'};
    std::vector<pos_t> knights;
    while (task_in.good()) {
        task_in >> c >> r >> std::ws;
        knights.emplace_back(r - 1, c - 'A');
    }

    if (knights.empty()) {
        task_out << "0\n";
        return 0;
    } else if (knights.size() == 1) {
        task_out << give_a_ride(knights[0], king) << '\n';
        return 0;
    }

    init_knight_distance(R, C);

    // no knight takes the king
    int ans = INF, gather_r{INF}, gather_c{INF};
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            int can{0};
            for (const auto& k : knights) {
                const auto d = diff(k, r, c);
                if (d == singularity && !(r == 0 || c == 0 || r == R - 1 || c == C - 1 || k.first == 0 || k.second == 0 || k.first == R - 1 || k.second == C - 1))
                    can += 2;
                else
                    can += knight_distance[d.first * C + d.second];
            }
            if (can < ans || (can == ans && diff(king, r, c) < diff(king, gather_r, gather_c))) {
                gather_r = r;
                gather_c = c;
                ans = can;
            }
        }

    const int knights_gathering_cost = ans;
    ans = INF;
    for (const auto& k : knights) { // what if the king initially goes to knight k
        const auto dist = diff(k, king);
        ans = std::min(ans, knights_gathering_cost + std::max(dist.first, dist.second));
    }
    const auto king_dist = diff(king, gather_r, gather_c); // what if the king goes to gathering place alone at the end
    ans = std::min(ans, knights_gathering_cost + std::max(king_dist.first, king_dist.second));

    // some knight gives king a ride
    // to meet time requrements check only some vicinity of the gathering point
    const int tolerance = 2;
    const int lo_r = std::max(0, gather_r - tolerance), hi_r = std::min(gather_r + tolerance + 1, R);
    const int lo_c = std::max(0, gather_c - tolerance), hi_c = std::min(gather_c + tolerance + 1, C);
    for (auto& k : knights) {
        auto copy_knight = k;
        const int init = give_a_ride(k, king);
        for (int r = lo_r; r < hi_r; ++r)
            for (int c = lo_c; c < hi_c; ++c) {
                int can = init;
                for (const auto& k : knights) {
                    const auto d = diff(k, r, c);
                    if (d == singularity && !(r == 0 || c == 0 || r == R - 1 || c == C - 1 || k.first == 0 || k.second == 0 || k.first == R - 1 || k.second == C - 1))
                        can += 2;
                    else
                        can += knight_distance[d.first * C + d.second];
                }
                ans = std::min(can, ans);
            }
        k = copy_knight;
    }

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 camelot.cpp -o camelot.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address camelot.cpp -o camelot

Input:

8 8 
D 5 
B 1 
F 1 
B 3

Output:

5

*/
