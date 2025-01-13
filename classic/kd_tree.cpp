#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <iostream>
#include <random>
#include <vector>

// Naive KDTree implementation with single point leaves

struct point_t {
    point_t(std::initializer_list<double> init) {
        std::copy(init.begin(), init.end(), _coords.begin());
    }
    std::array<double, 2> _coords;
};

class kd_tree_t {
  public:
    kd_tree_t(const std::vector<point_t>& points)
        : MAX_DIM(int(points[0]._coords.size()))
        , _node_idx(0)
        , _nodes(points.size())
        , _root(build(points, 0)) {}

    std::vector<point_t> range_search(const point_t& lower, const point_t& upper) {
        std::vector<point_t> result;
        range_search(_root, lower, upper, result);
        return result;
    }

    point_t nearest_neighbor(const point_t& target) {
        return nearest_neighbor(_root, target, _root->_point, 0);
    }

  private:
    struct node_t {
        node_t* _left{};
        node_t* _right{};
        point_t _point{};
        int _dim{};
    };

    node_t* build(std::vector<point_t> points, int depth) {
        if (points.empty())
            return {};

        const auto median = points.size() / 2;
        const auto dim = depth % MAX_DIM;
        std::nth_element(points.begin(), points.begin() + median, points.end(), [dim](const point_t& a, const point_t& b) {
            return a._coords[dim] < b._coords[dim];
        });

        std::vector<point_t> left_points(points.begin(), points.begin() + median);
        std::vector<point_t> right_points(points.begin() + median + 1, points.end());

        auto& node = _nodes[_node_idx++];
        node._point = points[median];
        node._dim = dim;
        node._left = build(std::move(left_points), ++depth);
        node._right = build(std::move(right_points), depth);

        return std::addressof(node);
    }

    void range_search(const node_t* node, const point_t& lower, const point_t& upper, std::vector<point_t>& result) {
        if (!node)
            return;

        bool inside = true;
        auto low = lower._coords.begin();
        auto hi  = upper._coords.begin();
        for (const auto c : node->_point._coords)
            if (c < *low++ || *hi++ < c) {
                inside = false;
                break;
            }

        if (inside)
            result.push_back(node->_point);

        const auto dim_target = node->_point._coords[node->_dim];
        if (node->_left && lower._coords[node->_dim] <= dim_target)
            range_search(node->_left, lower, upper, result);
        if (node->_right && dim_target <= upper._coords[node->_dim])
            range_search(node->_right, lower, upper, result);
    }

    point_t nearest_neighbor(const node_t* node, const point_t& target, point_t best, int depth) {
        if (!node)
            return best;

        auto best_dist = distance(best, target);
        if (distance(node->_point, target) < best_dist) {
            best = node->_point;
            best_dist = distance(best, target);
        }

        const auto dim = depth % MAX_DIM;
        const bool go_left = target._coords[dim] < node->_point._coords[dim];
        const auto& near = go_left ? node->_left : node->_right;

        best = nearest_neighbor(near, target, best, ++depth);

        if (std::abs(target._coords[dim] - node->_point._coords[dim]) < best_dist) {
            const auto& far = go_left ? node->_right : node->_left;
            best = nearest_neighbor(far, target, best, depth);
        }

        return best;
    }

    double distance(const point_t& a, const point_t& b) {
        return std::hypot(a._coords[0] - b._coords[0], a._coords[1] - b._coords[1]);
    }

    const int MAX_DIM;
    int _node_idx;
    std::vector<node_t> _nodes;
    node_t* _root;
};

template <typename T = double>
static T rand_in_range(const T ubound) {
    static std::random_device seed;
    static std::mt19937 gen{seed()};
    static std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist(gen);
};

constinit const int NUM_OF_POINTS = 2'000'000;
constinit const double MANHATTAN_RADIUS = 2.5;

int main() {
    std::vector<point_t> points;
    points.reserve(NUM_OF_POINTS);
    std::generate_n(std::back_inserter(points), NUM_OF_POINTS, [&]() -> point_t {
        return {double(rand_in_range(NUM_OF_POINTS)) / 2'000, double(rand_in_range(NUM_OF_POINTS)) / 2'000};
    });

    kd_tree_t tree(points);
    const point_t target = {871.0, 352.0};
    const point_t lower = {target._coords[0] - MANHATTAN_RADIUS, target._coords[1] - MANHATTAN_RADIUS};
    const point_t upper = {target._coords[0] + MANHATTAN_RADIUS, target._coords[1] + MANHATTAN_RADIUS};
    std::cout << std::format("Range search results around ({:5.1f}, {:5.1f}):\n", target._coords[0], target._coords[1]);
    const auto range_result = tree.range_search(lower, upper);
    for (const auto& p : range_result) {
        const auto dx = p._coords[0] - target._coords[0];
        const auto dy = p._coords[1] - target._coords[1];
        std::cout << std::format("({:5.1f}, {:5.1f}) distance: {:4.1f}\n", p._coords[0], p._coords[1], std::hypot(dx, dy));
    }

    const auto nearest = tree.nearest_neighbor(target);
    std::cout << std::format("Nearest neighbor: ({:5.1f}, {:5.1f})\n", nearest._coords[0], nearest._coords[1]);

    return 0;
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 kd_tree.cpp -o kd_tree.exe
*/