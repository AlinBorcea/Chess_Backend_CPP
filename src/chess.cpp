#include <fstream>

#include "chess.hpp"

using namespace chess;

/// Inline functions
inline bool in_range(uint32_t x, uint32_t a, uint32_t b) {
    return x >= a && x < b;
}

inline uint32_t abs_dif(uint32_t a, uint32_t b) {
    return a >= b ? a - b : b - a;
}

/// Data type definitions
enum Camp { NONE, UP, DOWN };

struct Piece {
    std::string name;
    Camp camp;
    char symbol;
    uint32_t move_style_index;

    friend std::istream& operator>>(std::istream& in, Piece& piece);
};

struct Block {
    Piece piece;

    bool is_empty() const;

    bool can_move_to(Block& other, Point& A, Point& B) const;

    void empty_block();

    void take_block(Block& other);

    friend std::istream& operator>>(std::istream& in, Block& block);

};

struct Chess {
    Block **table;
    uint32_t linec;
    uint32_t columnc;
    
    void open(const std::string& config_filename);

    void init_table(std::ifstream& infile);
    
    bool move(Point& A, Point& B);

    Chess(const std::string& config_filename);

    ~Chess();

    friend std::ostream& operator<<(std::ostream& out, Chess& table);
};

/// Static variables
static Camp camp_map[] = { NONE, UP, DOWN };
static bool (**move_style_map)(Point&);
static uint32_t movec;

/// Implementations

/// Piece structure
std::istream& operator>>(std::istream& in, Piece& piece) {
    uint32_t camp_val;

    in >> piece.name;
    in >> camp_val;
    in >> piece.symbol;
    in >> piece.move_style_index;

    if (camp_val >= 0U && camp_val <= 2U) {
        piece.camp = camp_map[camp_val];
    } else {
        throw "Invalid camp value in config file!";
    }

    return in;
}

/// Block structure
bool Block::is_empty() const {
    return piece.camp == NONE;
}

bool Block::can_move_to(Block& other, Point& A, Point& B) const {

    if (piece.camp == NONE) return false;
    if (piece.camp == other.piece.camp) return false;
    if (!in_range(piece.move_style_index, 0, movec)) return false;

    Point C = A - B;
    return move_style_map[piece.move_style_index](C);
}

void Block::empty_block() {
    piece.name = "Empty";
    piece.camp = NONE;
    piece.symbol = 'E';
}

void Block::take_block(Block& other) {
    this->piece.name = other.piece.name;
    this->piece.camp = other.piece.camp;
    this->piece.symbol = other.piece.symbol;
    this->piece.move_style_index = other.piece.move_style_index;
}

std::istream& operator>>(std::istream& in, Block& block) {
    in >> block.piece;
    return in;
}

/// Chess structure
Chess::Chess(const std::string& config_filename) {
    std::ifstream infile(config_filename);
    
    if (!infile) throw "Could not open configuration file!";

    infile >> linec;
    infile >> columnc;
    
    if (linec <= 0) throw "n must be a positive non nul integer";
    if (columnc <= 0) throw "m must be a positive non nul integer";
     
    init_table(infile);
}

void Chess::init_table(std::ifstream& infile) {
    table = new Block*[linec];
    for (uint32_t line = 0; line < linec; line++) {
        table[line] = new Block[columnc];
        for (uint32_t column = 0; column < columnc; column++) {
            infile >> table[line][column];
        }
    }
}

bool Chess::move(Point& A, Point& B) {

    if (!in_range(A.x, 0, linec) || !in_range(A.y, 0, columnc) ||
    !in_range(B.x, 0, linec) || !in_range(B.y, 0, columnc)) {
        return false;
    }

    Block *blockA = &table[A.x][A.y];
    Block *blockB = &table[B.x][B.y];

    if (blockA->can_move_to(*blockB, A, B)) {
        blockB->take_block(*blockA);
        blockA->empty_block();
    }

    return blockA->is_empty();
}

Chess::~Chess() {
    for (uint32_t line = 0; line < linec; line++) {
        delete[] table[line];
    }
    delete[] table;
}

std::ostream& operator<<(std::ostream& out, Chess& chess) {
    for (uint32_t line = 0; line < chess.linec; line++) {
        for (uint32_t column = 0; column < chess.columnc; column++) {
            out << chess.table[line][column].piece.symbol << ' ';
        }
        out << '\n';
    }
    out << '\n';
    return out;
}

namespace chess {
    Chess *chess = nullptr;

    void init(const std::string& config_filename, bool (**move_styles)(Point& point), uint32_t move_count) {
        if (!chess) {
            chess = new Chess(config_filename);
            move_style_map = move_styles;
            movec = move_count;
        }
    }

    bool move(Point& A, Point& B) {
        if (!chess) {
            throw "Chess game was not opened! Do not forget to call chess::init first!";
        }
        return chess->move(A, B);
    }

    void print(std::ostream& out) {
        if (!chess) {
            throw "Chess game was not opened! Do not forget to call chess::init first!";
        }
        out << *chess;
    }

    void close() {
        delete chess;
    }

    Point Point::operator-(Point& other) {
        return Point {
            abs_dif(x, other.x),
            abs_dif(y, other.y),
        };
    }

}
