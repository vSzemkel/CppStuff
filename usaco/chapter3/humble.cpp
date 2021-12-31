/*
ID: marcin.8
LANG: C++
TASK: humble
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=C7pFl5TCSr7&S=humble
*/

#include <algorithm>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <vector>

std::ifstream task_in("humble.in");
std::ofstream task_out("humble.out");

/**
 * @brief Key obeservation1: humble[n] == primes[i] * humble[j] for j < n
 *        Key obeservation2: humble[n + l] == primes[i] * humble[j + k] : l>0 => k>0
 */
int main(int, char**)
{
    int N, Z; task_in >> N >> Z;
    std::vector<int> primes(N);
    for (auto& p : primes)
        task_in >> p;

    std::vector<int64_t> dp(Z + 1);
    std::vector<int> next_humble_for_prime(N);
    dp[0] = 1;
    for (int i = 1; i <= Z; ++i) {
        int64_t next = 1e18;
        for (int p = 0; p < N; ++p) {
            while (primes[p] * dp[next_humble_for_prime[p]] <= dp[i - 1])
                ++next_humble_for_prime[p];
            next = std::min(next, primes[p] * dp[next_humble_for_prime[p]]);
        }

        dp[i] = next;
    }

    task_out << dp[Z] << '\n';
}

int main_slow_but_acceptable(int, char**)
{
    int N, Z; task_in >> N >> Z;
    std::vector<int> primes(N);
    std::unordered_set<int> seen;
    std::priority_queue<int64_t, std::vector<int64_t>, std::greater<>> pq;
    for (auto& p : primes) {
        task_in >> p;
        pq.push(p);
        // seen.insert(p); p is prime and we know p != can
    }

    int pqsz{N}; // using this variable proves t obe faster then pq.size() method
    int64_t ans, max{0};
    for (int z = Z; z; --z) {
        ans = pq.top(); pq.pop(); --pqsz;
        seen.erase(ans);
        for (const auto p : primes) {
            const auto can = ans * p;
            if (seen.count(can) == 0 && (pqsz < z || can < max)) {
                pq.push(can); ++pqsz;
                seen.insert(can);
                max = std::max(max, can);
            }
        }
    }

    task_out << ans << '\n';
    return 0;
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 humble.cpp -o humble.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address humble.cpp -o humble

Input:

100 100000 
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97
101 103 107 109 113 127 131 137 139 149 151 157 163 167 173 179 181
191 193 197 199 211 223 227 229 233 239 241 251 257 263 269 271 277
281 283 293 307 311 313 317 331 337 347 349 353 359 367 373 379 383
389 397 401 409 419 421 431 433 439 443 449 457 461 463 467 479 487
491 499 503 509 521 523 541

Output:

284456

*/
