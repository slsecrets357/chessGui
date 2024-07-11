#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>

class Position {
public:
    int row;
    int col;

    // Constructors
    Position(int r, int c) : row(r), col(c) {}
    Position() : row(0), col(0) {}
    Position(int n) : row(n / 8), col(n % 8) {}
    Position(const std::string& str) : row(str[1] - '1') {
        if (str[0] >= 'a' && str[0] <= 'z') {
            col = str[0] - 'a';
        } else {
            col = str[0] - 'A';
        }
    }

    // Comparison operators
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        char colChar = 'A' + pos.col;
        os << colChar << pos.row + 1;
        return os;
    }
};

#endif // POSITION_H
