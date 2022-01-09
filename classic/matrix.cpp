
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <vector>

// Matrix
// Basic operations implementation


constexpr const int INF = 1e9;
constexpr const double EPS = 1e-9;

template <int R, int C, typename T = int>
struct matrix_t {
    matrix_t() {
        for (int r = 0; r < R; ++r)
            _m[r].fill(T{});
    }

    matrix_t(const std::initializer_list<T>& data) {
        assert(data.size() == R * C);
        for (int r = 0; r < R; ++r)
            std::copy_n(data.begin() + r * C, C, _m[r].begin());
    }

    int rows() const { return R; }
    int cols() const { return C; }

    template <int X>
    auto operator*(const matrix_t<C, X, T>& rhs) const {
        matrix_t<R, X, T> ret;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < rhs.cols(); ++c) {
                T x{};
                for (int i = 0; i < C; ++i)
                    x += _m[r][i] * rhs[i][c];
                ret[r][c] = x;
            }
        return ret;
    }

    const auto& operator[] (const int row) const {
        assert(0 <= row && row < R);
        return _m[row];
    }

    auto& operator[](const int row) {
        assert(0 <= row && row < R);
        return _m[row];
    }

    void print() const {
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c)
                std::cout << _m[r][c] << ' ';
            std::cout << '\n';
        }
        std::cout << std::endl;
    }

    /**
     * @brief In the last column of this matix we have RHS of linear equation
     * @see https://cp-algorithms.com/linear_algebra/linear-system-gauss.html
     * @param ans Placeholder for the solution
     * @return int Solution existence: -1:NO, 0:single, 1:INF
     */
    int gauss(std::vector<T>& ans) {
        const int anssz = C - 1;
        std::vector<int> where(anssz, -1);
        for (int c = 0, r = 0; c < anssz && r < R; ++c) {
            int sel{r};
            for (int i = r; i < R; ++i)
                if (std::abs(_m[i][c]) > std::abs(_m[sel][c]))
                    sel = i;
            if (std::abs(_m[sel][c]) < EPS)
                continue;
            std::swap(_m[sel], _m[r]);
            where[c] = r;

            for (int i = 0; i < R; ++i)
                if (i != r) {
                    const auto f = _m[i][c] / _m[r][c];
                    for (int j = c; j < C; ++j)
                        _m[i][j] -= _m[r][j] * f;
                }
            ++r;
        }

        ans.assign(anssz, 0);
        for (int c = 0; c < anssz; ++c) {
            const int w = where[c];
            if (w != -1)
                ans[c] = _m[w][anssz] / _m[w][c];
        }
        for (int r = 0; r < R; ++r) {
            T sum{};
            for (int c = 0; c < anssz; ++c)
                sum += ans[c] * _m[r][c];
            if (std::abs(sum - _m[r][anssz]) > EPS)
                return 0;
        }
        for (int i = 0; i < anssz; ++i)
            if (where[i] == -1)
                return INF;
        return 1;
    }

  private:
    std::array<std::array<T, C>, R> _m;
};

int main(int, char**)
{
    matrix_t<3, 2> m{7, 2, 9, 0, 6, 1};
    m.print();
    matrix_t<2, 4> m2{2, 2, 2, 2, 3, 3, 3, 3};
    m2.print();

    const auto mulret = m * m2;
    mulret.print();

    matrix_t<3, 4, double> feed_ratios{1, 3, 2, 3, 2, 7, 1, 4, 3, 1, 2, 5};
    std::vector<double> ans;
    assert(feed_ratios.gauss(ans) == 1);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 matrix.cpp -o matrix.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address matrix.cpp -o matrix

Output:


*/
