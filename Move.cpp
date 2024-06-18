#include "Move.h"
#include "Board.h"

void Move::execute(Board& board) {
    // Move the piece on the board
    board.movePiece(from, to);

    // Handle capture
    if (capturedPiece) {
        board.removePiece(capturedPiece);
    }

    // Handle promotion
    if (isPromotion && promotionPiece) {
        board.promotePiece(to, promotionPiece);
    }

    // Handle castling
    if (isCastling) {
        // @todo: Specific castling logic
    }
}

void Move::undo(Board& board) {
    // Undo the move on the board
    board.movePiece(to, from, false, true);

    // Restore the captured piece
    if (capturedPiece) {
        board.addPiece(capturedPiece, to);
    }

    // Handle promotion undo
    if (isPromotion && promotionPiece) {
        board.demotePiece(to, movedPiece);
    }

    // Handle castling undo
    if (isCastling) {
        // @todo: Specific castling undo logic
    }
}
