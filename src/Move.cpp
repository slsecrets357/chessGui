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
    // Handle castling undo
    if (isCastling) {
        if(movedPiece->getColor() == Color::WHITE) {
            if (to == Position(0, 2)) {
                //move the rook back
                board.board[0][0] = board.board[0][3];
                board.board[0][3] = nullptr;
                std::string rookString = board.piece2string(board.board[0][3]);
                board.stringBoard[0*8 + 0] = rookString;
                board.stringBoard[0*8 + 3] = "  ";
                
            } else if (to == Position(0, 6)) {
                board.board[0][7] = board.board[0][5];
                board.board[0][5] = nullptr;
                std::string rookString = board.piece2string(board.board[0][5]);
                board.stringBoard[0*8 + 7] = rookString;
                board.stringBoard[0*8 + 5] = "  ";
            }
        } else {
            if (to == Position(7, 2)) {
                board.board[7][0] = board.board[7][3];
                board.board[7][3] = nullptr;
                std::string rookString = board.piece2string(board.board[7][3]);
                board.stringBoard[7*8 + 0] = rookString;
                board.stringBoard[7*8 + 3] = "  ";
            } else if (to == Position(7, 6)) {
                board.board[7][7] = board.board[7][5];
                board.board[7][5] = nullptr;
                std::string rookString = board.piece2string(board.board[7][5]);
                board.stringBoard[7*8 + 7] = rookString;
                board.stringBoard[7*8 + 5] = "  ";
            }
        }
    }
    // Undo the move on the board
    board.movePiece(to, from, false, true);

    // Restore the captured piece
    if (capturedPiece) {
        board.addPiece(capturedPiece, to);
        if (capturedPiece->getColor() == Color::WHITE) {
            board.whitePieces.push_back(capturedPiece);
        } else {
            board.blackPieces.push_back(capturedPiece);
        }
    }

    // Handle promotion undo
    if (isPromotion && promotionPiece) {
        board.demotePiece(to, movedPiece);
    }


}
