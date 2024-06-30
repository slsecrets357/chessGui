#include "Piece.h"
#include "Board.h"

std::ostream& operator<<(std::ostream& os, Color color) {
    if (color == Color::WHITE) {
        os << "WHITE";
    } else {
        os << "BLACK";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Piece& piece) {
    os << piece.getPieceType() <<  " at " << piece.getPosition() << " with color " << piece.getColor();
    return os;
}

std::vector<Position> Piece::filterCheckMoves(const Board& board, std::vector<Position>& moves) {
    // std::cout << "filterCheckMoves: filtering check moves" << std::endl;
    // Filter out moves that would leave the king in check
    std::vector<Position> filteredMoves;
    Color currentColor = color;
    std::cout << "filterCheckMoves: moves before filtering: " << moves.size() << std::endl;
    for (auto move : moves) {
        //const cast board
        Board& boardRef = const_cast<Board&>(board);
        if(!boardRef.movePiece(position, move, false)) {
            std::cout << "filterCheckMoves: move failed" << std::endl;
            continue;
        }
        if (!boardRef.isCheck(currentColor)) {
            std::cout << "filterCheckMoves: move valid. color: " << currentColor << std::endl;
            filteredMoves.push_back(move);
        } else {
            std::cout << "filterCheckMoves: filtered move: " << move << std::endl;
        }
        boardRef.undoMove();
    }
    std::cout << "filterCheckMoves: valid moves after filtering: " << filteredMoves.size() << std::endl;
    return filteredMoves;
}

// Implementation of King methods
King::King(Color color, Position pos) : Piece(PieceType::KING, color, pos) {}

bool King::isCastling(Position to) {
    if(hasMoved) {
        std::cout << "King::isCastling: king has moved, return false" << std::endl;
        return false;
    }
    if (castlingMoves.empty()) {
        std::cout << "King::isCastling: castlingMoves is empty, return false" << std::endl;
        return false;
    }
    if (std::find(castlingMoves.begin(), castlingMoves.end(), to) != castlingMoves.end()) {
        return true;
    }
    return false;
}

bool King::isValidMove(const Board& board, Position from, Position to) const {
    int rowDiff = std::abs(from.row - to.row);
    int colDiff = std::abs(from.col - to.col);
    // King moves one square in any direction
    if (rowDiff > 1 || colDiff > 1) {
        return false;
    }
    // Ensure destination is either empty or contains an opponent's piece
    std::shared_ptr<Piece> destinationPiece = board.getPiece(to);
    if (destinationPiece == nullptr || destinationPiece->getColor() != color) {
        return true;
    }
    return false;
}

char King::getSymbol() const {
    return (color == Color::WHITE) ? 'K' : 'k';
}

std::string King::getPieceType() const {
    return "King";
}

std::vector<Position>& King::generatePossibleMoves(const Board& board) {
    moves.clear();
    for (int rowDiff = -1; rowDiff <= 1; ++rowDiff) {
        for (int colDiff = -1; colDiff <= 1; ++colDiff) {
            if (rowDiff != 0 || colDiff != 0) {
                Position newPos(position.row + rowDiff, position.col + colDiff);
                if (board.isValidPosition(newPos)) {
                    std::shared_ptr<Piece> pieceAtNewPos = board.getPiece(newPos);
                    if (pieceAtNewPos == nullptr || pieceAtNewPos->getColor() != color) {
                        moves.push_back(newPos);
                    }
                }
            }
        }
    }

    // add castling moves
    bool castlingValid = false; 
    if (!hasMoved && !board.getIsInCheck()) {
        // Castling
        if (color == Color::WHITE) {
            if (board.getPiece({0, 5}) == nullptr && board.getPiece({0, 6}) == nullptr) {
                std::shared_ptr<Piece> rook = board.getPiece({0, 7});
                if (rook != nullptr && rook->getType() == PieceType::ROOK && rook->getColor() == Color::WHITE && !rook->hasMoved) {
                    if (true) {
                        moves.push_back({0, 6});
                        castlingMoves.push_back({0, 6});
                        castlingValid = true;
                    }
                }
            }
            if (board.getPiece({0, 3}) == nullptr && board.getPiece({0, 2}) == nullptr && board.getPiece({0, 1}) == nullptr) {
                std::shared_ptr<Piece> rook = board.getPiece({0, 0});
                if (rook != nullptr && rook->getType() == PieceType::ROOK && rook->getColor() == Color::WHITE && !rook->hasMoved) {
                    if (true) {
                        moves.push_back({0, 2});
                        castlingMoves.push_back({0, 2});
                        castlingValid = true;
                    }
                }
            }
        } else {
            if (board.getPiece({7, 5}) == nullptr && board.getPiece({7, 6}) == nullptr) {
                std::shared_ptr<Piece> rook = board.getPiece({7, 7});
                if (rook != nullptr && rook->getType() == PieceType::ROOK && rook->getColor() == Color::BLACK && !rook->hasMoved) {
                    if (true) {
                        moves.push_back({7, 6});
                        castlingMoves.push_back({7, 6});
                        castlingValid = true;
                    }
                }
            }
            if (board.getPiece({7, 3}) == nullptr && board.getPiece({7, 2}) == nullptr && board.getPiece({7, 1}) == nullptr) {
                std::shared_ptr<Piece> rook = board.getPiece({7, 0});
                if (rook != nullptr && rook->getType() == PieceType::ROOK && rook->getColor() == Color::BLACK && !rook->hasMoved) {
                    if (true) {
                        moves.push_back({7, 2});
                        castlingMoves.push_back({7, 2});
                        castlingValid = true;
                    }
                }
            }
        }
    }
    std::cout << "castlingValid: " << castlingValid << std::endl;
    if (!castlingValid) {
        castlingMoves.clear();
    }

    if (true) {
        moves = filterCheckMoves(board, moves);
    }

    return moves;
}

// Implementation of Queen methods
Queen::Queen(Color color, Position pos) : Piece(PieceType::QUEEN, color, pos) {}

bool Queen::isValidMove(const Board& board, Position from, Position to) const {
    int rowDiff = std::abs(from.row - to.row);
    int colDiff = std::abs(from.col - to.col);
    // Check if move is in a straight line or diagonal
    if (rowDiff != colDiff && rowDiff != 0 && colDiff != 0) {
        return false;
    }
    // Determine step direction
    int rowStep = (to.row - from.row) == 0 ? 0 : (to.row - from.row) / rowDiff;
    int colStep = (to.col - from.col) == 0 ? 0 : (to.col - from.col) / colDiff;
    // Check for obstacles
    for (int i = 1; i < std::max(rowDiff, colDiff); ++i) {
        Position intermediatePos = { from.row + i * rowStep, from.col + i * colStep };
        if (board.getPiece(intermediatePos) != nullptr) {
            return false;
        }
    }
    // Ensure destination is either empty or contains an opponent's piece
    std::shared_ptr<Piece> destinationPiece = board.getPiece(to);
    if (destinationPiece == nullptr || destinationPiece->getColor() != color) {
        return true;
    }
    return false;
}

char Queen::getSymbol() const {
    return (color == Color::WHITE) ? 'Q' : 'q';
}

std::string Queen::getPieceType() const {
    return "Queen";
}

std::vector<Position>& Queen::generatePossibleMoves(const Board& board) {
    moves.clear();
    // Directions: horizontal, vertical, and diagonal
    static std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };
    for (const auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            Position newPos = { position.row + i * dir.first, position.col + i * dir.second };
            if (!board.isValidPosition(newPos)) {
                break;
            }
            std::shared_ptr<Piece> pieceAtNewPos = board.getPiece(newPos);
            if (pieceAtNewPos == nullptr) {
                moves.push_back(newPos);
            } else {
                if (pieceAtNewPos->getColor() != color) {
                    moves.push_back(newPos);
                }
                break; // Stop if another piece is encountered
            }
        }
    }

    if (true) {
        moves = filterCheckMoves(board, moves);
    }

    return moves;
}

// Implementation of Bishop methods
Bishop::Bishop(Color color, Position pos) : Piece(PieceType::BISHOP, color, pos) {}

bool Bishop::isValidMove(const Board& board, Position from, Position to) const {
    int rowDiff = std::abs(from.row - to.row);
    int colDiff = std::abs(from.col - to.col);

    // Bishop moves diagonally
    if (rowDiff != colDiff) {
        return false;
    }

    // Check if rowDiff or colDiff is zero to prevent division by zero
    if (rowDiff == 0 || colDiff == 0) {
        return false;
    }

    // Determine step direction
    int rowStep = (to.row - from.row) / rowDiff;
    int colStep = (to.col - from.col) / colDiff;

    // Check for obstacles
    for (int i = 1; i < rowDiff; ++i) {
        Position intermediatePos = { from.row + i * rowStep, from.col + i * colStep };
        if (board.getPiece(intermediatePos) != nullptr) {
            return false;
        }
    }

    // Ensure destination is either empty or contains an opponent's piece
    std::shared_ptr<Piece> destinationPiece = board.getPiece(to);
    if (destinationPiece == nullptr || destinationPiece->getColor() != color) {
        return true;
    }

    return false;
}

char Bishop::getSymbol() const {
    return (color == Color::WHITE) ? 'B' : 'b';
}

std::string Bishop::getPieceType() const {
    return "Bishop";
}

std::vector<Position>& Bishop::generatePossibleMoves(const Board& board) {
    moves.clear();

    // Directions: diagonal
    static std::vector<std::pair<int, int>> directions = {
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    for (const auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            Position newPos = { position.row + i * dir.first, position.col + i * dir.second };
            if (!board.isValidPosition(newPos)) {
                break;
            }
            std::shared_ptr<Piece> pieceAtNewPos = board.getPiece(newPos);
            if (pieceAtNewPos == nullptr) {
                moves.push_back(newPos);
            } else {
                if (pieceAtNewPos->getColor() != color) {
                    moves.push_back(newPos);
                }
                break; // Stop if another piece is encountered
            }
        }
    }

    if (true) {
        moves = filterCheckMoves(board, moves);
    }

    return moves;
}

Knight::Knight(Color color, Position pos) : Piece(PieceType::KNIGHT, color, pos) {}

bool Knight::isValidMove(const Board& board, Position from, Position to) const {
    int rowDiff = std::abs(from.row - to.row);
    int colDiff = std::abs(from.col - to.col);

    // Knight moves in an L-shape: two squares in one direction and one square perpendicular
    if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
        std::shared_ptr<Piece> destinationPiece = board.getPiece(to);
        // Ensure the destination is either empty or contains an opponent's piece
        if (destinationPiece == nullptr || destinationPiece->getColor() != color) {
            return true;
        }
    }
    return false;
}

char Knight::getSymbol() const {
    return (color == Color::WHITE) ? 'N' : 'n';
}

std::string Knight::getPieceType() const {
    return "Knight";
}

std::vector<Position>& Knight::generatePossibleMoves(const Board& board) {
    moves.clear();
    static std::vector<std::pair<int, int>> moveOffsets = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (const auto& offset : moveOffsets) {
        Position newPos = { position.row + offset.first, position.col + offset.second };
        if (board.isValidPosition(newPos)) {
            std::shared_ptr<Piece> pieceAtNewPos = board.getPiece(newPos);
            if (pieceAtNewPos == nullptr || pieceAtNewPos->getColor() != color) {
                moves.push_back(newPos);
            }
        }
    }

    if (true) {
        moves = filterCheckMoves(board, moves);
    }

    return moves;
}

Rook::Rook(Color color, Position pos) : Piece(PieceType::ROOK, color, pos) {}

bool Rook::isValidMove(const Board& board, Position from, Position to) const {
    int rowDiff = std::abs(from.row - to.row);
    int colDiff = std::abs(from.col - to.col);

    // Rook moves horizontally or vertically
    if (rowDiff != 0 && colDiff != 0) {
        return false;
    }

    // Determine step direction
    int rowStep = (to.row - from.row) == 0 ? 0 : (to.row - from.row) / rowDiff;
    int colStep = (to.col - from.col) == 0 ? 0 : (to.col - from.col) / colDiff;

    // Check for obstacles
    for (int i = 1; i < std::max(rowDiff, colDiff); ++i) {
        Position intermediatePos = { from.row + i * rowStep, from.col + i * colStep };
        if (board.getPiece(intermediatePos) != nullptr) {
            return false;
        }
    }

    // Ensure destination is either empty or contains an opponent's piece
    std::shared_ptr<Piece> destinationPiece = board.getPiece(to);
    if (destinationPiece == nullptr || destinationPiece->getColor() != color) {
        return true;
    }

    return false;
}

char Rook::getSymbol() const {
    return (color == Color::WHITE) ? 'R' : 'r';
}

std::string Rook::getPieceType() const {
    return "Rook";
}

std::vector<Position>& Rook::generatePossibleMoves(const Board& board) {
    moves.clear();

    // Directions: horizontal and vertical
    static std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    for (const auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            Position newPos = { position.row + i * dir.first, position.col + i * dir.second };
            if (!board.isValidPosition(newPos)) {
                break;
            }
            std::shared_ptr<Piece> pieceAtNewPos = board.getPiece(newPos);
            if (pieceAtNewPos == nullptr) {
                moves.push_back(newPos);
            } else {
                if (pieceAtNewPos->getColor() != color) {
                    moves.push_back(newPos);
                }
                break; // Stop if another piece is encountered
            }
        }
    }

    if (true) {
        moves = filterCheckMoves(board, moves);
    }

    return moves;
}

Pawn::Pawn(Color color, Position pos) : Piece(PieceType::PAWN, color, pos) {}

bool Pawn::isValidMove(const Board& board, Position from, Position to) const {
    int rowDiff = to.row - from.row;
    int colDiff = std::abs(from.col - to.col);
    if (color == Color::WHITE) {
        return (rowDiff == 1 && colDiff == 0 && board.getPiece(to) == nullptr) ||
               (rowDiff == 1 && colDiff == 1 && board.getPiece(to) != nullptr && board.getPiece(to)->getColor() == Color::BLACK) ||
               (from.row == 1 && rowDiff == 2 && colDiff == 0 && board.getPiece(to) == nullptr && board.getPiece({from.row + 1, from.col}) == nullptr);
    } else {
        return (rowDiff == -1 && colDiff == 0 && board.getPiece(to) == nullptr) ||
               (rowDiff == -1 && colDiff == 1 && board.getPiece(to) != nullptr && board.getPiece(to)->getColor() == Color::WHITE) ||
               (from.row == 6 && rowDiff == -2 && colDiff == 0 && board.getPiece(to) == nullptr && board.getPiece({from.row - 1, from.col}) == nullptr);
    }
}

char Pawn::getSymbol() const {
    return (color == Color::WHITE) ? 'P' : 'p';
}

std::string Pawn::getPieceType() const {
    return "Pawn";
}

std::vector<Position>& Pawn::generatePossibleMoves(const Board& board) {
    moves.clear();
    int direction = (color == Color::WHITE) ? 1 : -1;
    Position forwardPos = { position.row + direction, position.col };
    if (board.isValidPosition(forwardPos) && board.getPiece(forwardPos) == nullptr) {
        moves.push_back(forwardPos);
        if ((color == Color::WHITE && position.row == 1) || (color == Color::BLACK && position.row == 6)) {
            Position doubleForwardPos = { position.row + 2 * direction, position.col };
            if (board.isValidPosition(doubleForwardPos) && board.getPiece(doubleForwardPos) == nullptr) {
                moves.push_back(doubleForwardPos);
            }
        }
    }
    Position captureLeft = { position.row + direction, position.col - 1 };
    Position captureRight = { position.row + direction, position.col + 1 };
    if (board.isValidPosition(captureLeft) && board.getPiece(captureLeft) != nullptr && board.getPiece(captureLeft)->getColor() != color) {
        moves.push_back(captureLeft);
    }
    if (board.isValidPosition(captureRight) && board.getPiece(captureRight) != nullptr && board.getPiece(captureRight)->getColor() != color) {
        moves.push_back(captureRight);
    }

    if (true) {
        moves = filterCheckMoves(board, moves);
    }

    return moves;
}
