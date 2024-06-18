#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <Position.h>

class Board;

enum class PieceType { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN };
enum class Color { WHITE, BLACK };

class Piece {
protected:
    PieceType type;
    Color color;
    Position position;

public:
    // Constructor
    Piece(PieceType type, Color color, Position pos) : type(type), color(color), position(pos)
    {
        moves.reserve(64);
    }

    std::vector<Position> moves;

    // Destructor
    virtual ~Piece() {}

    // Getters
    PieceType getType() const {
        return type;
    }
    Color getColor() const {
        return color;
    }
    Position getPosition() const {
        return position;
    }

    // Setters
    void setPosition(Position pos) {
        position = pos;
    }

    std::vector<Position> filterCheckMoves(const Board& board, std::vector<Position>& moves);

    // Check if a move is valid
    virtual bool isValidMove(const Board& board, Position from, Position to) const = 0;

    // Get the symbol for printing the piece
    virtual char getSymbol() const = 0;

    // Generate all possible moves for the piece
    virtual std::vector<Position>& generatePossibleMoves(const Board& board) = 0;
};

// Derived classes for each type of piece

class King : public Piece {
public:
    King(Color color, Position pos);

    bool isValidMove(const Board& board, Position from, Position to) const override;

    char getSymbol() const override;

    std::vector<Position>& generatePossibleMoves(const Board& board) override;
};

class Queen : public Piece {
public:
    Queen(Color color, Position pos);

    bool isValidMove(const Board& board, Position from, Position to) const override;

    char getSymbol() const override;

    std::vector<Position>& generatePossibleMoves(const Board& board) override;
};

class Bishop : public Piece {
public:
    Bishop(Color color, Position pos);

    bool isValidMove(const Board& board, Position from, Position to) const override;

    char getSymbol() const override;

    std::vector<Position>& generatePossibleMoves(const Board& board) override;
};

class Knight : public Piece {
public:
    Knight(Color color, Position pos);

    bool isValidMove(const Board& board, Position from, Position to) const override;

    char getSymbol() const override;

    std::vector<Position>& generatePossibleMoves(const Board& board) override;
};

class Rook : public Piece {
public:
    Rook(Color color, Position pos);

    bool isValidMove(const Board& board, Position from, Position to) const override;

    char getSymbol() const override;

    std::vector<Position>& generatePossibleMoves(const Board& board) override;
};

class Pawn : public Piece {
public:
    Pawn(Color color, Position pos);

    bool isValidMove(const Board& board, Position from, Position to) const override;

    char getSymbol() const override;

    std::vector<Position>& generatePossibleMoves(const Board& board) override;
};

#endif // PIECE_H
