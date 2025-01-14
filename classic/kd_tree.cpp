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

/**
 * Nodes are allocated in one call but referenced by explicit indexes
 */
class kd_tree_t {
  public:
    kd_tree_t(const std::vector<point_t>& points)
        : MAX_DIM(int(points[0]._coords.size()))
        , _node_idx(1)
        , _nodes(points.size() + 1)
    {
        build(points, 0);
    }

    std::vector<point_t> range_search(const point_t& lower, const point_t& upper) {
        std::vector<point_t> result;
        if (!_nodes.empty())
            range_search(0, lower, upper, result);
        return result;
    }

    point_t nearest_neighbor(const point_t& target) {
        if (_nodes.empty())
            return {};
        return nearest_neighbor(0, target, _nodes[0]._point, 0);
    }

  private:
    struct node_t {
        point_t _point{};
        int _dim{};
        uint32_t _left{};
        uint32_t _right{};
    };

    int build(std::vector<point_t> points, int depth) {
        if (points.empty())
            return 0;

        const auto median = points.size() / 2;
        const auto dim = depth % MAX_DIM;
        std::nth_element(points.begin(), points.begin() + median, points.end(), [dim](const point_t& a, const point_t& b) {
            return a._coords[dim] < b._coords[dim];
        });

        std::vector<point_t> left_points(points.begin(), points.begin() + median);
        std::vector<point_t> right_points(points.begin() + median + 1, points.end());

        const auto this_id = _node_idx++;
        auto& node = _nodes[this_id];
        node._point = points[median];
        node._dim = dim;
        node._left = build(std::move(left_points), ++depth);
        node._right = build(std::move(right_points), depth);

        return this_id;
    }

    void range_search(const uint32_t node_id, const point_t& lower, const point_t& upper, std::vector<point_t>& result) {
        if (node_id == 0)
            return;

        bool inside = true;
        auto low = lower._coords.begin();
        auto hi  = upper._coords.begin();
        const auto& node = _nodes[node_id];
        for (const auto c : node._point._coords)
            if (c < *low++ || *hi++ < c) {
                inside = false;
                break;
            }

        if (inside)
            result.push_back(node._point);

        const auto dim_target = node._point._coords[node._dim];
        if (0 < node._left && lower._coords[node._dim] <= dim_target)
            range_search(node._left, lower, upper, result);
        if (0 < node._right && dim_target <= upper._coords[node._dim])
            range_search(node._right, lower, upper, result);
    }

    point_t nearest_neighbor(const uint32_t node_id, const point_t& target, point_t best, int depth) {
        if (node_id == 0)
            return best;

        const auto& node = _nodes[node_id];
        auto best_dist = distance(best, target);
        if (distance(node._point, target) < best_dist) {
            best = node._point;
            best_dist = distance(best, target);
        }

        const auto dim = depth % MAX_DIM;
        const bool go_left = target._coords[dim] < node._point._coords[dim];
        const auto near = go_left ? node._left : node._right;

        best = nearest_neighbor(near, target, best, ++depth);

        if (std::abs(target._coords[dim] - node._point._coords[dim]) < best_dist) {
            const auto far = go_left ? node._right : node._left;
            best = nearest_neighbor(far, target, best, depth);
        }

        return best;
    }

    double distance(const point_t& a, const point_t& b) {
        return std::hypot(a._coords[0] - b._coords[0], a._coords[1] - b._coords[1]);
    }

    const int MAX_DIM;
    uint32_t _node_idx;
    std::vector<node_t> _nodes;
};

/**
 * Nodes are allocated in one call but referenced by pointers
 */
class kd_tree_t2 {
  public:
    kd_tree_t2(const std::vector<point_t>& points)
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

/**
 * Nodes are overallocated in one call and referenced implicit by indexes
 */
class kd_tree_t3 {
  public:
    kd_tree_t3(const std::vector<point_t>& points)
        : MAX_DIM(int(points[0]._coords.size()))
        , _nodes(points.size() * 4) // 2 is too small
    {
        build(points, 1, 0);
    }

    std::vector<point_t> range_search(const point_t& lower, const point_t& upper) {
        if (_nodes.empty())
            return {};
        std::vector<point_t> result;
        range_search(1, lower, upper, result);
        return result;
    }

    point_t nearest_neighbor(const point_t& target) {
        if (_nodes.empty())
            return {};
        return nearest_neighbor(1, target, _nodes[1]._point, 0);
    }

    int max_valid_id{};

  private:
    struct node_t {
        point_t _point{};
        int _dim{-1};
    };

    void build(std::vector<point_t> points, const int node_id, int depth) {
        if (points.empty())
            return;
        if (max_valid_id < node_id)
            max_valid_id = node_id;

        const auto median = points.size() / 2;
        const auto dim = depth % MAX_DIM;
        std::nth_element(points.begin(), points.begin() + median, points.end(), [dim](const point_t& a, const point_t& b) {
            return a._coords[dim] < b._coords[dim];
        });

        auto& node = _nodes[node_id];
        node._point = points[median];
        node._dim = dim;

        std::vector<point_t> left_points(points.begin(), points.begin() + median);
        std::vector<point_t> right_points(points.begin() + median + 1, points.end());
        build(std::move(left_points), 2 * node_id, ++depth);
        build(std::move(right_points), 2 * node_id + 1, depth);
    }

    void range_search(const int node_id, const point_t& lower, const point_t& upper, std::vector<point_t>& result) {
        const auto& node = _nodes[node_id];
        if (node._dim == -1)
            return;

        bool inside = true;
        auto low = lower._coords.begin();
        auto hi  = upper._coords.begin();
        for (const auto c : node._point._coords)
            if (c < *low++ || *hi++ < c) {
                inside = false;
                break;
            }

        if (inside)
            result.push_back(node._point);

        const auto& left = _nodes[2 * node_id];
        const auto& right = _nodes[2 * node_id + 1];
        const auto dim_target = node._point._coords[node._dim];
        if (0 <= left._dim && lower._coords[node._dim] <= dim_target)
            range_search(2 * node_id, lower, upper, result);
        if (0 <= right._dim && dim_target <= upper._coords[node._dim])
            range_search(2 * node_id + 1, lower, upper, result);
    }

    point_t nearest_neighbor(const int node_id, const point_t& target, point_t best, int depth) {
        const auto& node = _nodes[node_id];
        if (node._dim == -1)
            return best;

        auto best_dist = distance(best, target);
        if (distance(node._point, target) < best_dist) {
            best = node._point;
            best_dist = distance(best, target);
        }

        const auto dim = depth % MAX_DIM;
        const bool go_left = target._coords[dim] < node._point._coords[dim];
        const auto near = go_left ? 2 * node_id : 2 * node_id + 1;

        best = nearest_neighbor(near, target, best, ++depth);

        if (std::abs(target._coords[dim] - node._point._coords[dim]) < best_dist)
            best = nearest_neighbor(near ^ 1, target, best, depth);

        return best;
    }

    double distance(const point_t& a, const point_t& b) {
        return std::hypot(a._coords[0] - b._coords[0], a._coords[1] - b._coords[1]);
    }

    const int MAX_DIM;
    std::vector<node_t> _nodes;
};

template <typename T = double>
static T rand_in_range(const T ubound) {
    static std::random_device seed;
    static std::mt19937 gen{seed()};
    static std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist(gen);
};

constinit const int BITS = 21;
constinit const int NUM_OF_POINTS = (1 << BITS) + 1;
constinit const double MANHATTAN_RADIUS = 2;

int main() {
    std::vector<point_t> points;
    points.reserve(NUM_OF_POINTS);
    std::generate_n(std::back_inserter(points), NUM_OF_POINTS, [&]() -> point_t {
        return {double(rand_in_range(NUM_OF_POINTS)) / (1 << (BITS - 10)), double(rand_in_range(NUM_OF_POINTS)) / (1 << (BITS - 10))};
    });

    kd_tree_t3 tree(points);
    const point_t target = {871.0, 352.0};
    const point_t lower = {target._coords[0] - MANHATTAN_RADIUS, target._coords[1] - MANHATTAN_RADIUS};
    const point_t upper = {target._coords[0] + MANHATTAN_RADIUS, target._coords[1] + MANHATTAN_RADIUS};
    auto range_result = tree.range_search(lower, upper);
    std::sort(range_result.begin(), range_result.end(), [&target](const point_t& a, const point_t& b) {
        return std::hypot(a._coords[0] - target._coords[0], a._coords[1] - target._coords[1]) < std::hypot(b._coords[0] - target._coords[0], b._coords[1] - target._coords[1]);
    });
    std::cout << std::format("Range search results around ({:5.1f}, {:5.1f}):\n", target._coords[0], target._coords[1]);
    for (const auto& p : range_result) {
        const auto dx = p._coords[0] - target._coords[0];
        const auto dy = p._coords[1] - target._coords[1];
        std::cout << std::format("({:5.1f}, {:5.1f}) distance: {:4.1f}\n", p._coords[0], p._coords[1], std::hypot(dx, dy));
    }

    const auto nearest = tree.nearest_neighbor(target);
    std::cout << std::format("Nearest neighbor: ({:5.1f}, {:5.1f})\n", nearest._coords[0], nearest._coords[1]);

    std::cerr << "Number of points: " << NUM_OF_POINTS << '\n';
    std::cerr << "Last valid node index: " << tree.max_valid_id << '\n';
    std::cerr << "Last referenced node index: " << 2 * tree.max_valid_id + 1 << '\n';
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 kd_tree.cpp -o kd_tree.exe
*/