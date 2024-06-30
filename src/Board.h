#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Piece.h"
#include "Move.h"
#include <memory>

class Board {
public:
    std::shared_ptr<Piece> whiteKing;
    std::shared_ptr<Piece> blackKing;
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    std::vector<std::string> stringBoard = {
        "wr", "wn", "wb", "wq", "wk", "wb", "wn", "wr",
        "wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp",
        "br", "bn", "bb", "bq", "bk", "bb", "bn", "br"
    };
    std::vector<std::pair<int, int>> changedPositions;
    std::string piece2string(std::shared_ptr<Piece> piece) const {
        if (piece == nullptr) {
            return "  ";
        }
        std::string color = piece->getColor() == Color::WHITE ? "w" : "b";
        std::string type = "";
        if (piece->getType() == PieceType::PAWN) {
            type = "p";
        } else if (piece->getType() == PieceType::ROOK) {
            type = "r";
        } else if (piece->getType() == PieceType::KNIGHT) {
            type = "n";
        } else if (piece->getType() == PieceType::BISHOP) {
            type = "b";
        } else if (piece->getType() == PieceType::QUEEN) {
            type = "q";
        } else if (piece->getType() == PieceType::KING) {
            type = "k";
        }
        return color + type;
    }
    std::vector<std::shared_ptr<Piece>> whitePieces;
    std::vector<std::shared_ptr<Piece>> blackPieces;
    std::vector<Move*> moves;
    Color sideToMove;
    bool isInCheck = false;

    // Constructor
    Board();

    // Destructor
    ~Board();

    // Initialize the board with pieces in the starting positions
    void initialize();

    // Print the board (for debugging purposes)
    void printBoard() const;

    // Move a piece from one position to another
    enum class MoveType { FAILED, NORMAL, CASTLING, PROMOTION };
    bool movePiece(Position from, Position to, bool safetyCheck = true, bool undo = false);
    bool undoMove();

    // Get the piece at a specific position
    std::shared_ptr<Piece> getPiece(Position pos) const;

    // Add a piece to a specific position
    void addPiece(std::shared_ptr<Piece> piece, Position pos);

    // Remove a piece from a specific position
    void removePiece(std::shared_ptr<Piece> piece);

    // Promote a pawn to another piece
    void promotePiece(Position pos, std::shared_ptr<Piece> newPiece);

    // Demote a piece back to a pawn (for undoing promotions)
    void demotePiece(Position pos, std::shared_ptr<Piece> pawnPiece);

    // Check if a move puts the player in check
    bool isCheck(Color color) const;

    // Check if the current player is in checkmate
    bool isCheckmate(Color color) const;

    // Get the current side to move
    Color getSideToMove() const;
    Color getOppositeColor() const {
        return sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;
    }

    void printSideToMove() const {
        if (sideToMove == Color::WHITE) {
            std::cout << "white player's turn now." << std::endl;
        } else {
            std::cout << "dark player's turn now." << std::endl;
        }
    }

    // Switch the side to move
    void switchSideToMove();

    bool isValidPosition(Position pos) const;

    // Generate all possible moves for the current player
    std::vector<Move> generateAllPossibleMoves(Color color) const;

    void updateCheckStatus() {
        isInCheck = isCheck(sideToMove);
    }

    bool getIsInCheck() const {
        return isInCheck;
    }

};

#endif // BOARD_H
