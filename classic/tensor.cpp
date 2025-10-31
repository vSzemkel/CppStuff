#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <span>

// Multidimentional array and array view
// Kudos to: ecnerwala

template <typename T, int NDIMS>
struct tensor_view
{
    static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

  public:
    tensor_view() : _shape{nullptr}, _strides{nullptr}, _data(nullptr) {}

  protected:
    tensor_view(std::span<int, NDIMS> shape, std::span<int, NDIMS> strides, T* data)
        : _shape(shape), _strides(strides), _data(data) {}

    int flatten_index(std::array<int, NDIMS> idx) const
    {
        int res{};
        for (int i = 0; i < NDIMS; ++i)
            res += idx[i] * _strides[i];
        
        return res;
    }
    int flatten_index_checked(std::array<int, NDIMS> idx) const
    {
        int res = 0;
        for (int i = 0; i < NDIMS; i++) {
            assert(0 <= idx[i] && idx[i] < _shape[i]);
            res += idx[i] * _strides[i];
        }
        return res;
    }

  public:
    T& operator[](std::array<int, NDIMS> idx) const
    {
        return _data[flatten_index(idx)];
    }
    T& at(std::array<int, NDIMS> idx) const
    {
        return _data[flatten_index_checked(idx)];
    }

    template <int D = NDIMS>
    typename std::enable_if<(0 < D), tensor_view<T, NDIMS - 1>>::type operator[](int idx) const
    {
        std::span<int, NDIMS - 1> nshape(std::next(_shape.begin()), NDIMS - 1);
        std::span<int, NDIMS - 1> nstrides(std::next(_strides.begin()), NDIMS - 1);
        T* ndata = std::next(_data, _strides[0] * idx);
        return tensor_view<T, NDIMS - 1>(nshape, nstrides, ndata);
    }
    template <int D = NDIMS>
    typename std::enable_if<(0 < D), tensor_view<T, NDIMS - 1>>::type at(int idx) const
    {
        assert(0 <= idx && idx < _shape[0]);
        return operator[](idx);
    }

    template <int D = NDIMS>
    typename std::enable_if<(0 == D), T&>::type operator*() const
    {
        return *_data;
    }

    template <typename U, int D>
    friend struct tensor_view;
    template <typename U, int D>
    friend struct tensor;

  private:
    std::span<int, NDIMS> _shape;
    std::span<int, NDIMS> _strides;
    T* _data;
};

template <typename T, int NDIMS>
struct tensor
{
    static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

  public:
    tensor() : _shape{}, _strides{}, _data(nullptr), _len(0) {}
    ~tensor() { delete[] _data; }

    explicit tensor(std::array<int, NDIMS> shape, const T& t = T())
    {
        _shape = shape;
        _len = 1;
        for (int i = NDIMS - 1; i >= 0; --i) {
            _strides[i] = _len;
            _len *= _shape[i];
        }
        _data = new T[_len];
        std::fill_n(_data, _len, t);
    }

    tensor(const tensor& o) : _shape(o._shape), _strides(o._strides), _len(o._len), _data(new T[_len])
    {
        std::copy_n(o._data, _len, _data);
    }

    tensor& operator=(tensor&& o) noexcept
    {
        std::swap(_shape, o._shape);
        std::swap(_strides, o._strides);
        std::swap(_len, o._len);
        std::swap(_data, o._data);
        return *this;
    }

    tensor(tensor&& o) : tensor()
    {
        *this = std::move(o);
    }

    tensor& operator=(const tensor& o)
    {
        return *this = tensor(o);
    }

    using view_t = tensor_view<T, NDIMS>;
    view_t view()
    {
        return tensor_view<T, NDIMS>(_shape, _strides, _data);
    }
    operator view_t()
    {
        return view();
    }

    using const_view_t = tensor_view<const T, NDIMS>;
    const_view_t view() const
    {
        return tensor_view<const T, NDIMS>(_shape, _strides, _data);
    }

    operator const_view_t() const
    {
        return view();
    }

    T& operator[](std::array<int, NDIMS> idx) { return view()[idx]; }
    T& at(std::array<int, NDIMS> idx) { return view().at(idx); }
    const T& operator[](std::array<int, NDIMS> idx) const { return view()[idx]; }
    const T& at(std::array<int, NDIMS> idx) const { return view().at(idx); }

    template <int D = NDIMS>
    typename std::enable_if<(0 < D), tensor_view<T, NDIMS - 1>>::type operator[](int idx)
    {
        return view()[idx];
    }
    template <int D = NDIMS>
    typename std::enable_if<(0 < D), tensor_view<T, NDIMS - 1>>::type at(int idx)
    {
        return view().at(idx);
    }

    template <int D = NDIMS>
    typename std::enable_if<(0 < D), tensor_view<const T, NDIMS - 1>>::type operator[](int idx) const
    {
        return view()[idx];
    }
    template <int D = NDIMS>
    typename std::enable_if<(0 < D), tensor_view<const T, NDIMS - 1>>::type at(int idx) const
    {
        return view().at(idx);
    }

    template <int D = NDIMS>
    typename std::enable_if<(0 == D), T&>::type operator*()
    {
        return *view();
    }
    template <int D = NDIMS>
    typename std::enable_if<(0 == D), const T&>::type operator*() const
    {
        return *view();
    }

  private:
    std::array<int, NDIMS> _shape;
    std::array<int, NDIMS> _strides;
    T* _data;
    int _len;
};

int main()
{
    int v = -1;
    tensor<int, 3> cube({3, 3, 3});
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k) {
                cube[{i, j, k}] = ++v;
                assert((*cube[i][j][k] == cube[{i, j, k}]));
            }

    const int slice = 2;
    int val = 3 * 3 * slice - 1;
    auto middle = cube.view()[slice];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            assert(*middle[i][j] == ++val);
            std::cout << middle[{i, j}] << '\n';
        }

    assert((cube[{0, 0, 0}] == 0));
    assert((cube[{0, 0, 1}] == 1));
    assert((cube[{0, 0, 1}] == 1));
    assert((cube[{0, 1, 0}] == 3));
    assert((cube[{1, 0, 0}] == 9));
    assert((cube[{2, 2, 1}] == 25));
    assert((cube[{2, 2, 2}] == 26));
}

/*
clang++.exe -Wall -Wextra -g -O0 -std=c++20 tensor.cpp
*/
