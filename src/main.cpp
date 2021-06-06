#include <conio.h>

#include "chess.hpp"

static std::string conf_filename = "config";

void play();
bool rook_style(chess::Point& p) {
    return (p.x == 0 && p.y != 0) || (p.x != 0 && p.y == 0);
}
bool mv2(chess::Point& p) {
    return false;
}
bool mv3(chess::Point& p) {
    return true;
}

int main() {
    bool (*styles[])(chess::Point& point) = { rook_style, mv2, mv3 };
    chess::init(conf_filename, styles, 3);
    play();
    chess::close();
    return 0;
}

void play() {
    using namespace chess;
    char action = 'm';
    Point A, B;

    

    while (action != 'e') {
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
        chess::print(std::cout);
    }

}
