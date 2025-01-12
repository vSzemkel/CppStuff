#include <algorithm>
#include <cmath>
#include <memory>
#include <iostream>
#include <vector>

// Basic KDTree implementation with single point leaves

struct point_t {
    std::vector<double> _coords;
    point_t(std::initializer_list<double> init) : _coords(init) {}
};

struct node_t {
    std::unique_ptr<node_t> _left{};
    std::unique_ptr<node_t> _right{};
    const point_t _point;
    const int _dim;

    node_t(point_t p, int d) : _point(p), _dim(d) {}
};

class KDTree {
  public:
    KDTree(const std::vector<point_t>& points)
        : MAX_DIM(int(points[0]._coords.size())) {
        _root = build(points, 0);
    }

    std::unique_ptr<node_t> build(std::vector<point_t> points, int depth) {
        if (points.empty()) return nullptr;

        const int dim = depth % points[0]._coords.size();
        std::sort(points.begin(), points.end(), [dim](const point_t& a, const point_t& b) {
            return a._coords[dim] < b._coords[dim];
        });

        const int median = points.size() / 2;
        std::vector<point_t> left_points(points.begin(), points.begin() + median);
        std::vector<point_t> right_points(points.begin() + median + 1, points.end());

        auto node = std::make_unique<node_t>(points[median], dim);
        node->_left = build(left_points, depth + 1);
        node->_right = build(right_points, depth + 1);

        return node;
    }

    // Range search method
    std::vector<point_t> range_search(const point_t& lower, const point_t& upper) {
        std::vector<point_t> result;
        range_search(_root.get(), lower, upper, result);
        return result;
    }

    point_t nearest_neighbor(const point_t& target) {
        return nearest_neighbor(_root.get(), target, _root->_point, 0);
    }

  private:
    const int MAX_DIM;;
    std::unique_ptr<node_t> _root;

    void range_search(const node_t* node, const point_t& lower, const point_t& upper, std::vector<point_t>& result) {
        if (!node)
            return;

        bool inside = true;
        for (int i = 0; i < MAX_DIM; ++i) {
            if (node->_point._coords[i] < lower._coords[i] || node->_point._coords[i] > upper._coords[i]) {
                inside = false;
                break;
            }
        }

        if (inside)
            result.push_back(node->_point);

        if (node->_left && node->_point._coords[node->_dim] >= lower._coords[node->_dim]) 
            range_search(node->_left.get(), lower, upper, result);

        if (node->_right && node->_point._coords[node->_dim] <= upper._coords[node->_dim])
            range_search(node->_right.get(), lower, upper, result);
    }

    point_t nearest_neighbor(const node_t* node, const point_t& target, point_t best, int depth) {
        if (!node)
            return best;

        const double best_dist = distance(best, target);
        const double node_dist = distance(node->_point, target);

        if (node_dist < best_dist)
            best = node->_point;

        const int dim = depth % target._coords.size();
        const bool go_left = target._coords[dim] < node->_point._coords[dim];
        const auto& near = go_left ? node->_left : node->_right;
        const auto& far = go_left ? node->_right : node->_left;

        best = nearest_neighbor(near.get(), target, best, depth + 1);

        if (std::abs(target._coords[dim] - node->_point._coords[dim]) < distance(best, target))
            best = nearest_neighbor(far.get(), target, best, depth + 1);

        return best;
    }

    double distance(const point_t& a, const point_t& b) {
        double dist{};
        for (int i = 0; i < MAX_DIM; ++i)
            dist += (a._coords[i] - b._coords[i]) * (a._coords[i] - b._coords[i]);

        return std::sqrt(dist);
    }
};

int main() {
    std::vector<point_t> points = {{2.0, 3.0}, {5.0, 4.0}, {9.0, 6.0}, {4.0, 7.0}, {8.0, 1.0}, {7.0, 2.0}};
    KDTree tree(points);

    point_t lower = {3.0, 2.0};
    point_t upper = {8.0, 5.0};
    auto range_result = tree.range_search(lower, upper);
    std::cout << "Range search results:\n";
    for (const auto& p : range_result) {
        std::cout << "(" << p._coords[0] << ", " << p._coords[1] << ")\n";
    }

    point_t target = {6.0, 3.0};
    auto nearest = tree.nearest_neighbor(target);
    std::cout << "Nearest neighbor: (" << nearest._coords[0] << ", " << nearest._coords[1] << ")\n";

    return 0;
}

// clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 kd_tree.cpp -o kd_tree.exe