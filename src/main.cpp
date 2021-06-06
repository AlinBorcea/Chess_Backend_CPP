#include <conio.h>

#include "chess.hpp"

static std::string conf_filename = "config";

void play();

inline uint32_t max(uint32_t a, uint32_t b) {
    return a >= b ? a : b;
}

/// Style 0
bool rook_style(chess::Point& p) {
    return (p.x == 0 && p.y != 0) || (p.x != 0 && p.y == 0);
}
/// Style 1
bool knight_style(chess::Point& p) {
    return (p.x == 1 && p.y == 2) || (p.x == 2 && p.y == 1);
}
/// Style 2
bool bishop_style(chess::Point& p) {
    return p.x == p.y;
}
/// Style 3
bool queen_style(chess::Point& p) {
    return rook_style(p) || bishop_style(p);
}
/// Style 4
bool king_style(chess::Point& p) {
    return queen_style(p) && max(p.x, p.y) == 1;
}
/// Style 5
bool stupid_pawn_style(chess::Point& p) {
    return p.x == 1 && p.y == 0;
}

int main() {
    bool (*styles[])(chess::Point& point) = {
         rook_style, knight_style, bishop_style, 
         queen_style, king_style, 
         stupid_pawn_style,
    };

    chess::init(conf_filename, styles, 6);
    play();
    chess::close();
    return 0;
}

void play() {
    using namespace chess;
    char action = 'm';
    Point A, B;

    while (action != 'e') {
        chess::print(std::cout);
        std::cout << "Action..\n";
        action = _getch();

        switch (action) {
            case 'm':
            std::cout << "x = ";
            std::cin >> A.x;
            std::cout << "y = ";
            std::cin >> A.y;
            std::cout << "x = ";
            std::cin >> B.x;
            std::cout << "y = ";
            std::cin >> B.y;
            chess::move(A, B);
            break;

            default:
            return;
        }
    }
}
