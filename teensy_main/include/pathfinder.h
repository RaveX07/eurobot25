#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstring>

class Position {
public:
    int x, y;
    int theta;

    Position(int x, int y, int theta = 0) : x(x), y(y), theta(theta) {
        limit();
    }

    void limit() {
        if (x > 2999) x = 2999;
        else if (x < 0) x = 0;

        if (y > 1999) y = 1999;
        else if (y < 0) y = 0;
    }
};

int add_with_limits(int a, int b, int max_value) {
    int result = a + b;
    if (result > max_value) return max_value;
    if (result < 0) return 0;
    return result;
}

class Pathfinder {
private:
    Position start;
    Position target;
    std::vector<std::vector<int>> obstacle_map;
    int map_width, map_height;
    int max_iters;
    int bot_width;

    int distance(const Position& node1, const Position& node2) {
        return static_cast<int>(std::sqrt(std::pow(node1.x - node2.x, 2) + std::pow(node1.y - node2.y, 2)));
    }

    std::pair<Position, Position> calculate_parallel_points(const Position& pos1, const Position& pos2, int distance) {
        int dx = pos2.x - pos1.x;
        int dy = pos2.y - pos1.y;

        double norm = std::sqrt(dx * dx + dy * dy);
        double nx = -dy / norm;
        double ny = dx / norm;

        int shift_x = static_cast<int>(distance * nx);
        int shift_y = static_cast<int>(distance * ny);

        Position p1(pos1.x + shift_x, pos1.y + shift_y);
        Position p2(pos2.x + shift_x, pos2.y + shift_y);

        return {p1, p2};
    }

    bool collission(const Position& node1, const Position& node2) {
        int dist = distance(node1, node2);

        for (int i = 0; i <= dist; ++i) {
            double t = static_cast<double>(i) / dist;
            int x = static_cast<int>(node1.x + t * (node2.x - node1.x));
            int y = static_cast<int>(node1.y + t * (node2.y - node1.y));

            if (obstacle_map[y][x] != 0) return true;
        }

        return false;
    }

    bool collission_with_bot(const Position& node1, const Position& node2) {
        auto [right1, right2] = calculate_parallel_points(node1, node2, bot_width);
        auto [left1, left2] = calculate_parallel_points(node1, node2, -bot_width);

        return collission(node1, node2) || collission(right1, right2) || collission(left1, left2);
    }

public:
    Pathfinder(int map_width = 3000, int map_height = 2000, int max_iters = 1250, int bot_width = 180)
        : map_width(map_width), map_height(map_height), max_iters(max_iters), bot_width(bot_width),
          start(100, 100), target(2900, 1900) {
        obstacle_map = std::vector<std::vector<int>>(map_height, std::vector<int>(map_width, 0));

        add_stack({900, 900}, {1300, 1000});
        add_stack({1700, 900}, {2100, 1000});
        add_stack({1950, 1700}, {2350, 1800});
        add_stack({650, 1700}, {1050, 1800});
        add_stack({550, 0}, {950, 100});
        add_stack({2050, 0}, {2450, 100});
        add_stack({2600, 450}, {2999, 550});
        add_stack({0, 450}, {400, 550});
        add_stack({2600, 1000}, {2999, 1100});
        add_stack({0, 1000}, {400, 1100});

        add_obstacle({650, 1800}, {2350, 1999});
        add_obstacle({1050, 1500}, {1950, 1800});
        add_obstacle({0, 1550}, {150, 1999});
        add_obstacle({2850, 1550}, {2999, 1999});
    }

    void set_start_target(const Position& start, const Position& target) {
        this->start = start;
        this->target = target;
    }

    void add_stack(const Position& pos1, const Position& pos2) {
        for (int y = pos1.y; y < pos2.y; ++y) {
            for (int x = pos1.x; x < pos2.x; ++x) {
                obstacle_map[y][x] = 2;
            }
        }
    }

    void add_obstacle(const Position& pos1, const Position& pos2) {
        for (int y = pos1.y; y < pos2.y; ++y) {
            for (int x = pos1.x; x < pos2.x; ++x) {
                obstacle_map[y][x] = 1;
            }
        }
    }

    std::vector<Position> plan(const Position& start, const Position& target) {
        if (!collission_with_bot(start, target)) return {target};

        std::vector<Position> possibilities;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_x(0, map_width - 1);
        std::uniform_int_distribution<> dis_y(0, map_height - 1);

        for (int i = 0; i < max_iters; ++i) {
            Position pos(dis_x(gen), dis_y(gen));
            if (!collission_with_bot(start, pos) && !collission_with_bot(pos, target)) {
                possibilities.push_back(pos);
            }
        }

        if (possibilities.empty()) {
            std::cout << "No path found." << std::endl;
            return {};
        }

        Position best_pos = possibilities[0];
        int best_distance = distance(start, best_pos) + distance(best_pos, target);

        for (const auto& pos : possibilities) {
            int dist = distance(start, pos) + distance(pos, target);
            if (dist < best_distance) {
                best_pos = pos;
                best_distance = dist;
            }
        }

        return {best_pos, target};
    }
};

int main() {
    Pathfinder pf;
    Position start(100, 100), target(2900, 1900);
    auto path = pf.plan(start, target);

    for (const auto& pos : path) {
        std::cout << "Position: (" << pos.x << ", " << pos.y << ")\n";
    }

    return 0;
}
