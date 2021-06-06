#ifndef CHESS_HPP
#define CHESS_HPP

#include <iostream>

namespace chess {

    struct Point {
        uint32_t x;
        uint32_t y;

        Point operator-(Point& other);
    };

    void init(const std::string& config_filename, bool (**move_styles)(Point& point), uint32_t move_count);
    bool move(Point& A, Point& B);
    void print(std::ostream& out);
    void close();
}

#endif