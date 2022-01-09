
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <vector>

// Matrix
// Basic operations implementation

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
        std::cout << '\n';
    }

  private:
    std::array<std::array<T, C>, R> _m;
};

int main(int, char**)
{
    matrix_t<3, 2> m({7, 2, 9, 0, 6, 1});
    m.print();
    matrix_t<2, 4> m2{2, 2, 2, 2, 3, 3, 3, 3};
    m2.print();

    const auto mulret = m * m2;
    mulret.print();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 matrix.cpp -o matrix.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address matrix.cpp -o matrix

Output:


*/
