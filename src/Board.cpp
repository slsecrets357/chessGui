#include "Board.h"
#include "Piece.h"
#include <iostream>

Board::Board() : board(8, std::vector<std::shared_ptr<Piece>>(8, nullptr)), 
                sideToMove(Color::WHITE), whiteKing(nullptr), blackKing(nullptr)
{}

// Destructor
Board::~Board() {
    // commented out: no need to delete pieces since they are managed by shared_ptr
    // for (auto& row: board) {
    //     for (std::shared_ptr<Piece> piece: row) {
    //         delete piece;
    //     }
    // }
}

// Initialize the board with pieces in the starting positions
void Board::initialize() {
    // Clear the board
    for (auto& row : board) {
        std::fill(row.begin(), row.end(), nullptr);
    }

    // Add pawns
    for (int col = 0; col < 8; ++col) {
        auto whitePawn = std::make_shared<Pawn>(Color::WHITE, Position(1, col));
        auto blackPawn = std::make_shared<Pawn>(Color::BLACK, Position(6, col));
        board[1][col] = whitePawn;
        board[6][col] = blackPawn;
        whitePieces.push_back(whitePawn);
        blackPieces.push_back(blackPawn);
    }

    // Add rooks
    auto whiteRook1 = std::make_shared<Rook>(Color::WHITE, Position(0, 0));
    auto whiteRook2 = std::make_shared<Rook>(Color::WHITE, Position(0, 7));
    auto blackRook1 = std::make_shared<Rook>(Color::BLACK, Position(7, 0));
    auto blackRook2 = std::make_shared<Rook>(Color::BLACK, Position(7, 7));
    board[0][0] = whiteRook1;
    board[0][7] = whiteRook2;
    board[7][0] = blackRook1;
    board[7][7] = blackRook2;
    whitePieces.push_back(whiteRook1);
    whitePieces.push_back(whiteRook2);
    blackPieces.push_back(blackRook1);
    blackPieces.push_back(blackRook2);

    // Add knights
    auto whiteKnight1 = std::make_shared<Knight>(Color::WHITE, Position(0, 1));
    auto whiteKnight2 = std::make_shared<Knight>(Color::WHITE, Position(0, 6));
    auto blackKnight1 = std::make_shared<Knight>(Color::BLACK, Position(7, 1));
    auto blackKnight2 = std::make_shared<Knight>(Color::BLACK, Position(7, 6));
    board[0][1] = whiteKnight1;
    board[0][6] = whiteKnight2;
    board[7][1] = blackKnight1;
    board[7][6] = blackKnight2;
    whitePieces.push_back(whiteKnight1);
    whitePieces.push_back(whiteKnight2);
    blackPieces.push_back(blackKnight1);
    blackPieces.push_back(blackKnight2);

    // Add bishops
    auto whiteBishop1 = std::make_shared<Bishop>(Color::WHITE, Position(0, 2));
    auto whiteBishop2 = std::make_shared<Bishop>(Color::WHITE, Position(0, 5));
    auto blackBishop1 = std::make_shared<Bishop>(Color::BLACK, Position(7, 2));
    auto blackBishop2 = std::make_shared<Bishop>(Color::BLACK, Position(7, 5));
    board[0][2] = whiteBishop1;
    board[0][5] = whiteBishop2;
    board[7][2] = blackBishop1;
    board[7][5] = blackBishop2;
    whitePieces.push_back(whiteBishop1);
    whitePieces.push_back(whiteBishop2);
    blackPieces.push_back(blackBishop1);
    blackPieces.push_back(blackBishop2);

    // Add queens
    auto whiteQueen = std::make_shared<Queen>(Color::WHITE, Position(0, 3));
    auto blackQueen = std::make_shared<Queen>(Color::BLACK, Position(7, 3));
    board[0][3] = whiteQueen;
    board[7][3] = blackQueen;
    whitePieces.push_back(whiteQueen);
    blackPieces.push_back(blackQueen);

    // Add kings
    whiteKing = std::make_shared<King>(Color::WHITE, Position(0, 4));
    blackKing = std::make_shared<King>(Color::BLACK, Position(7, 4));
    board[0][4] = whiteKing;
    board[7][4] = blackKing;
    whitePieces.push_back(whiteKing);
    blackPieces.push_back(blackKing);

    // Set the side to move
    sideToMove = Color::WHITE;
}

// Print the board (for debugging purposes)
void Board::printBoard() const {
    std::cout << "  A B C D E F G H" << std::endl;
    for (int row = 7; row >= 0; --row) {
        std::cout << row + 1 << " ";
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] != nullptr) {
                std::cout << board[row][col]->getSymbol() << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
}

// Move a piece from one position to another
bool Board::movePiece(Position from, Position to, bool safetyCheck, bool undo) {
    // std::cout << "movePiece: from " << from << " to " << to << std::endl;
    if (safetyCheck && !undo) {
        changedPositions.clear();
    }
    std::shared_ptr<Piece> piece = getPiece(from);
    if (safetyCheck && !piece) {
        std::cout << "you attempted to move a non-existent piece... don't do that again." << std::endl;
        return false;
    }
    if (safetyCheck && piece->getColor() != sideToMove) {
        std::cout << "you can't move your opponent's piece... srsly?" << std::endl;
        return false;
    }

    // if (!safetyCheck || piece && piece->isValidMove(*this, from, to)) {
    if (!safetyCheck || piece) {
        std::vector<Position> possibleMoves = piece->moves;
        if (safetyCheck && std::find(possibleMoves.begin(), possibleMoves.end(), to) == possibleMoves.end()) {
            std::cout << "move not found in piece's valid moves: " << from << " to " << to << std::endl;
            return false;
        }

        // handle castling
        bool castling = false;
        if (piece->getType() == PieceType::KING && piece->isCastling(to)) {
            castling = true;
            std::cout << "castling detected" << std::endl;
            std::shared_ptr<Piece> rook = nullptr;
            Position rookInitialPos;
            Position rookFinalPos;
            if (sideToMove == Color::WHITE) {
                if (to.col == 2) {
                    rookInitialPos = Position(0, 0);
                    rookFinalPos = Position(0, 3);
                    rook = getPiece(rookInitialPos);
                } else if (to.col == 6) {
                    rookInitialPos = Position(0, 7);
                    rookFinalPos = Position(0, 5);
                    rook = getPiece(rookInitialPos);
                    if(!rook) {
                        std::cout << "no rook found for castling" << std::endl;
                        return false;
                    }
                }
            } else {
                if (to.col == 2) {
                    rookInitialPos = Position(7, 0);
                    rookFinalPos = Position(7, 3);
                    rook = getPiece(rookInitialPos);
                } else if (to.col == 6) {
                    rookInitialPos = Position(7, 7);
                    rookFinalPos = Position(7, 5);
                    rook = getPiece(rookInitialPos);
                }
            }
            if (!rook) {
                std::cout << "no rook found for castling" << std::endl;
                return false;
            }
            board[rookFinalPos.row][rookFinalPos.col] = rook;
            stringBoard[rookFinalPos.row * 8 + rookFinalPos.col] = piece2string(rook);
            board[rookInitialPos.row][rookInitialPos.col] = nullptr;
            stringBoard[rookInitialPos.row * 8 + rookInitialPos.col] = " ";
            rook->setPosition(rookFinalPos);
            if (safetyCheck && !undo) {
                changedPositions.emplace_back(std::make_pair(rookInitialPos.row*8 + rookInitialPos.col, static_cast<int>(pieceTypeWithColor::empty)));
                changedPositions.emplace_back(std::make_pair(rookFinalPos.row*8 + rookFinalPos.col, static_cast<int>(rook->getPieceTypeWithColor())));
            }
        }

        bool promotion = false;
        std::shared_ptr<Piece> promotionPiece = nullptr;
        if (safetyCheck && piece->getType() == PieceType::PAWN && (to.row == 0 || to.row == 7)) {
            promotion = true;
        }
        if (promotion) {
            promotionPiece = std::make_shared<Queen>(piece->getColor(), to);
            if (!undo) {
                moves.emplace_back(new Move(from, to, piece, nullptr, castling, promotion));
                if(safetyCheck) piece->hasMoved = true;
                std::cout << "move history size: " << moves.size() << std::endl;
            }
            // remove pawn from board
            removePiece(piece);
            // add queen to board
            if(promotionPiece->getColor() == Color::WHITE) {
                whitePieces.push_back(promotionPiece);
            } else {
                blackPieces.push_back(promotionPiece);
            }
            board[to.row][to.col] = promotionPiece;
            stringBoard[to.row * 8 + to.col] = piece2string(promotionPiece);

            if (safetyCheck && !undo) {
                changedPositions.emplace_back(std::make_pair(from.row*8 + from.col, static_cast<int>(pieceTypeWithColor::empty)));
                changedPositions.emplace_back(std::make_pair(to.row*8 + to.col, static_cast<int>(promotionPiece->getPieceTypeWithColor())));
            }
        } else {
            std::shared_ptr<Piece> targetPiece = getPiece(to);
            if (!undo) {
                moves.emplace_back(new Move(from, to, piece, targetPiece, castling, promotion));
                if(safetyCheck) piece->hasMoved = true;
                std::cout << "move history size: " << moves.size() << std::endl;
            }
            if (targetPiece) {
                removePiece(targetPiece);
            }
            board[to.row][to.col] = piece;
            stringBoard[to.row * 8 + to.col] = piece2string(piece);
            board[from.row][from.col] = nullptr;
            stringBoard[from.row * 8 + from.col] = " ";
            piece->setPosition(to);
            if (safetyCheck && !undo) {
                changedPositions.emplace_back(std::make_pair(from.row*8 + from.col, static_cast<int>(pieceTypeWithColor::empty)));
                changedPositions.emplace_back(std::make_pair(to.row*8 + to.col, static_cast<int>(piece->getPieceTypeWithColor())));
            }
        }
        switchSideToMove();
        return true;
    }
    return false;
}
bool Board::undoMove() {
    // std::cout << "undoing move..." << std::endl;
    if (moves.empty()) {
        return false;
    }
    // std::cout << "undoMove: move history size: " << moves.size() << std::endl;
    Move* lastMove = moves.back();
    moves.pop_back();
    lastMove->undo(*this);
    delete lastMove;
    // std::cout << "undoMove: move history size2: " << moves.size() << std::endl;
}

// Get the piece at a specific position
std::shared_ptr<Piece> Board::getPiece(Position pos) const {
    if (isValidPosition(pos)) {
        return board[pos.row][pos.col];
    }
    return nullptr;
}

// Add a piece to a specific position
void Board::addPiece(std::shared_ptr<Piece> piece, Position pos) {
    if (isValidPosition(pos)) {
        board[pos.row][pos.col] = piece;
        stringBoard[pos.row * 8 + pos.col] = piece2string(piece);
        piece->setPosition(pos);
        if (piece->getColor() == Color::WHITE) {
            whitePieces.push_back(piece);
        } else {
            blackPieces.push_back(piece);
        }
    }
}

// Remove a piece from a specific position
void Board::removePiece(std::shared_ptr<Piece> piece) {
    Position pos = piece->getPosition();
    if (isValidPosition(pos) && board[pos.row][pos.col] == piece) {
        board[pos.row][pos.col] = nullptr;
        stringBoard[pos.row * 8 + pos.col] = " ";
    }
    if (piece->getColor() == Color::WHITE) {
        capturedWhitePieces.push_back(piece);
        whitePieces.erase(std::remove(whitePieces.begin(), whitePieces.end(), piece), whitePieces.end());
    } else {
        capturedBlackPieces.push_back(piece);
        blackPieces.erase(std::remove(blackPieces.begin(), blackPieces.end(), piece), blackPieces.end());
    }
}

// Promote a pawn to another piece
void Board::promotePiece(Position pos, std::shared_ptr<Piece> newPiece) {
    if (isValidPosition(pos) && board[pos.row][pos.col] != nullptr) {
        removePiece(board[pos.row][pos.col]);
        addPiece(newPiece, pos);
    }
}

// Demote a piece back to a pawn (for undoing promotions)
void Board::demotePiece(Position pos, std::shared_ptr<Piece> pawnPiece) {
    if (isValidPosition(pos) && board[pos.row][pos.col] != nullptr) {
        removePiece(board[pos.row][pos.col]);
        addPiece(pawnPiece, pos);
    }
}

// Check if a move puts the player in check
bool Board::isCheck(Color color) const {
    Position kingPos = (color == Color::WHITE) ? whiteKing->getPosition() : blackKing->getPosition();

    // Check if any opponent's piece can move to the king's position
    std::vector<std::shared_ptr<Piece>> opponentPieces = (color == Color::WHITE) ? blackPieces : whitePieces;
    for (std::shared_ptr<Piece> piece : opponentPieces) {
        // std::cout << "checking piece: " << *piece << std::endl;
        if (piece->isValidMove(*this, piece->getPosition(), kingPos)) {
            return true;
        }
    }
    return false;
}

// Check if the current player is in checkmate
bool Board::isCheckmate(Color color) const {
    // @todo: Implement this function
    return false;
}

// Get the current side to move
Color Board::getSideToMove() const {
    return sideToMove;
}

// Switch the side to move
void Board::switchSideToMove() {
    sideToMove = (sideToMove == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

bool Board::isValidPosition(Position pos) const {
    return pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8;
}

std::vector<Move> Board::generateAllPossibleMoves(Color color) const {
    auto pieces = color == Color::WHITE ? whitePieces : blackPieces;
    std::vector<Move> allMoves;
    for (std::shared_ptr<Piece> piece : pieces) {
        std::vector<Position> possibleMoves = piece->generatePossibleMoves(*this);
        for (const Position& pos : possibleMoves) {
            allMoves.emplace_back(piece->getPosition(), pos, piece, getPiece(pos));
        }
    }
    return allMoves;
}
