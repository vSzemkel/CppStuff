
#include <cassert>
#include <initializer_list>

// Positional_number
// Helper class for easy digits extraction
// After manipulating arbitrary digits, sync() call is required to have changes reflected in _value

template<int N>
class positional_number_t
{
  public:
    positional_number_t(const int value = 0) : _value(value) {
        syncn2d();
    }

    positional_number_t(const std::initializer_list<int> digits) {
        assert(digits.size() <= N);
        int i = int(digits.size());
        for (const int d : digits)
            _digits[--i] = d;
        sync();
    }

    positional_number_t& operator=(const int value) {
        _value = value;
        syncn2d();
        return *this;
    }

    operator int() const {
        return _value;
    }

    int operator[](const int index) const {
        return _digits[index];
    }

    int& operator[](const int index) {
        return _digits[index];
    }

    int reverse() const {
        int ret{};
        for (const int& d : _digits)
            ret = 10 * ret + d;

        return ret;
    }

    int push_back(const int digit) {
        assert(0 <= digit && digit < 10);
        _value = 10 * _value + digit;
        syncn2d();
        return _value;
    }

    int pop_back(int count) {
        assert(0 <= count && count <= N);
        for (; count; --count)
            _value /= 10;
        syncn2d();
        return _value;
    }

    void sync() {
        _value = 0;
        for (int i = N - 1; ~i; --i)
            _value = 10 * _value + _digits[i];
    }

  private:
    void syncn2d() {
        auto tmp = _value;
        for (int& d : _digits) {
            d = tmp % 10;
            tmp /= 10;
        }
    }

    int _digits[N] = {};
    int _value{};
};

int main(int, char**)
{
    using number_t = positional_number_t<8>;
    number_t a(123456);
    assert(a[0] == 6 && a[3] == 3 && a[6] == 0);
    if (a < 100 || a == 2838)
        a = 10;
    assert(a.reverse() == 65432100);
    a[1] = 7;
    a.sync();
    assert(a == 123476);
    a.push_back(5);
    assert(a == 1234765);
    a.pop_back(3);
    assert(a == 1234);

    number_t b{};
    b[3] = 5;
    b[0] = 1;
    b.sync();
    assert(b == 5001);
    assert(((b + 200) - 7) % 100 == 94);

    number_t c = {6, 2, 7, 0, 8};
    assert(c == 62708);
}

/*

Format:
clang-format -i positional_number.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 positional_number.cpp -o positional_number.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address positional_number.cpp -o positional_number

*/
