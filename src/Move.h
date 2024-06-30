#ifndef MOVE_H
#define MOVE_H

#include "Piece.h"
#include <memory>
#include <string>

// Forward declaration of Board to avoid circular dependency
class Board;

class Move {
private:
    Position from;
    Position to;
    std::shared_ptr<Piece> movedPiece;
    std::shared_ptr<Piece> capturedPiece;
    bool isCastling;
    bool isPromotion;
    std::shared_ptr<Piece> promotionPiece;

public:
    // Constructors
    Move(Position from, Position to, std::shared_ptr<Piece> movedPiece, std::shared_ptr<Piece> capturedPiece = nullptr,
         bool isCastling = false, bool isPromotion = false, std::shared_ptr<Piece> promotionPiece = nullptr)
        : from(from), to(to), movedPiece(movedPiece), capturedPiece(capturedPiece),
        isCastling(isCastling), isPromotion(isPromotion), promotionPiece(promotionPiece) {}

    // Accessors
    Position getFrom() const { return from; }
    Position getTo() const { return to; }
    std::shared_ptr<Piece> getMovedPiece() const { return movedPiece; }
    std::shared_ptr<Piece> getCapturedPiece() const { return capturedPiece; }
    bool getIsCastling() const { return isCastling; }
    bool getIsPromotion() const { return isPromotion; }
    std::shared_ptr<Piece> getPromotionPiece() const { return promotionPiece; }

    // Execute the move on the board
    void execute(Board& board);

    // Undo the move on the board
    void undo(Board& board);
};

#endif // MOVE_H
