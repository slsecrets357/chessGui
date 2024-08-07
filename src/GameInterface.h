#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>
#include <QList>
#include "Position.h"
#include <QDebug>
#include "Game.h"
#include "Board.h"
#include <QString>
#include <QTimer>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

class GameInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> legalMoves READ legalMoves WRITE setLegalMoves NOTIFY legalMovesChanged FINAL)
    Q_PROPERTY(int selectedGrid READ selectedGrid WRITE setSelectedGrid NOTIFY selectedGridChanged FINAL)
    Q_PROPERTY(QList<QString> dummyBoard READ dummyBoard WRITE setDummyBoard NOTIFY dummyBoardChanged FINAL)
    Q_PROPERTY(QList<QString> capturedWhitePiecesString READ capturedWhitePiecesString WRITE setCapturedWhitePiecesString NOTIFY capturedWhitePiecesStringChanged FINAL)
    Q_PROPERTY(QList<QString> capturedBlackPiecesString READ capturedBlackPiecesString WRITE setCapturedBlackPiecesString NOTIFY capturedBlackPiecesStringChanged FINAL)
    Q_PROPERTY(QString currentTimer READ currentTimer WRITE setCurrentTimer NOTIFY currentTimerChanged FINAL)
    Q_PROPERTY(QString whiteTimer READ whiteTimer WRITE setWhiteTimer NOTIFY whiteTimerChanged FINAL)
    Q_PROPERTY(QString blackTimer READ blackTimer WRITE setBlackTimer NOTIFY blackTimerChanged FINAL)

public:
    explicit GameInterface(QObject *parent = nullptr);

    QList<int> legalMoves() const;
    int selectedGrid() const;

    QList<QString> dummyBoard() const;
    Game gameEngine;
    int startGrid = -1;
    int destGrid = -1;
    Position from;
    Position to;
    std::shared_ptr<Piece> selectedPiece;
    bool successfulMove = false;
    bool paused = true;
    std::vector<QList<QString>> boardHistory;
    std::vector<QString> pieceRepresentations = {"wr", "wn", "wb", "wq", "wk", "wp", "br", "bn", "bb", "bq", "bk", "bp", ""};
    bool useAI = false;

    QString currentTimer() const;
    void setCurrentTimer(const QString &newCurrentTimer);

    QString whiteTimer() const;
    void setWhiteTimer(const QString &newWhiteTimer);

    QString blackTimer() const;
    void setBlackTimer(const QString &newBlackTimer);

    QList<QString> capturedWhitePiecesString() const;
    void setCapturedWhitePiecesString(const QList<QString> &newCapturedWhitePiecesString);

    QList<QString> capturedBlackPiecesString() const;
    void setCapturedBlackPiecesString(const QList<QString> &newCapturedBlackPiecesString);

public slots:
    void restart() {
        gameEngine.board.initialize();
        m_dummyBoard = {
            "wr", "wn", "wb", "wq", "wk", "wb", "wn", "wr",
            "wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp",
            "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "",
            "bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp",
            "br", "bn", "bb", "bq", "bk", "bb", "bn", "br"
        };
        emit dummyBoardChanged();
        resetCapturedPieces();
        
        m_currentTimeTimer->stop();
        m_whiteTimeTimer->stop();
        m_blackTimeTimer->stop();
        m_currentTimeTimer->start();
        m_whiteTimeRemaining = QTime(0, whitePlayerTimeMinutes, 0);
        setWhiteTimer("" + m_whiteTimeRemaining.toString("mm:ss.zzz"));
        m_blackTimeRemaining = QTime(0, blackPlayerTimeMinutes, 0);
        setBlackTimer("" + m_blackTimeRemaining.toString("mm:ss.zzz"));
        boardHistory.push_back(m_dummyBoard);
    }
    void setLegalMoves(const QList<int> &newLegalMoves);
    void setSelectedGrid(int newSelectedGrid);
    void setDummyBoard(const QList<QString> &newDummyBoard);
    void currentTimeTimerTimeout();
    void whiteTimeTimerTimeout();
    void blackTimeTimerTimeout();
    void addTimeToWhiteTimer(int seconds);
    void addTimeToBlackTimer(int seconds);
    void toggleAI(bool toggle) {
        this->useAI = toggle;
    }

    void print() {
        gameEngine.board.printBoard();
    }
    bool undo() {
        std::cout << "undoing move1..." << std::endl;
        //print board
        gameEngine.board.printBoard();
        if(boardHistory.size() <= 1) {
            return false;
        }
        boardHistory.pop_back();
        m_dummyBoard = boardHistory.back();
        emit dummyBoardChanged();
        std::cout << "after undoing move1..." << std::endl;
        gameEngine.board.printBoard();
        if(gameEngine.board.undoMove()) {
            resetCapturedPieces();
            return true;
        }
        return false;
    }
    bool checkStartingGrid(int grid) {
        return true;
    }
    bool checkDestinationGrid(int grid) {
        return true; // TODO: Implement this function
    }
    void executeMove() {
        if (gameEngine.board.movePiece(from, to)) {
            makeMove();
            gameEngine.board.updateCheckStatus();
            if(gameEngine.board.getIsInCheck()) {
                std::cout << "player in check!!" << std::endl;
            }
            if(gameEngine.board.getSideToMove() == Color::WHITE) {
                addTimeToBlackTimer(blackTimeIncrementPerMove);
                m_whiteTimeTimer->start();
                m_blackTimeTimer->stop();
            } else {
                addTimeToWhiteTimer(whiteTimeIncrementPerMove);
                m_blackTimeTimer->start();
                m_whiteTimeTimer->stop();
            }
        } else {
            std::cout << "invalid move from " << from << " to " << to << ". please try again. " << std::endl;
        }
        gameEngine.board.printSideToMove();
        clearLegalMoves();
        startGrid = -1;
        destGrid = -1;
    }
    void executeAIMove() {
        std::string fen = gameEngine.board.toFEN(); // Convert the current board state to FEN
        std::cout << "fen: " << fen << std::endl;

        // Run the Stockfish computation in a separate thread
        QFuture<std::string> future = QtConcurrent::run([this, fen]() {
            return gameEngine.stockfish->getBestMoveTimed(fen, 3000);
        });

        // Watch the future and call a slot when the computation is done
        QFutureWatcher<std::string> *watcher = new QFutureWatcher<std::string>(this);
        connect(watcher, &QFutureWatcher<std::string>::finished, this, [this, watcher]() {
            std::string bestMove = watcher->result();
            watcher->deleteLater();

            std::cout << "Stockfish suggests: " << bestMove << std::endl;
            if (!bestMove.empty()) {
                std::cout << bestMove.substr(0, 2) << " to " << bestMove.substr(2, 2) << std::endl;
                from = Position(bestMove.substr(0, 2));
                selectedPiece = gameEngine.board.getPiece(from);
                if(!computeLegalMoves(from.row * 8 + from.col)) return;
                to = Position(bestMove.substr(2, 2));
                std::cout << "moving from " << from << " to " << to << std::endl;
                executeMove();
            }
        });

        watcher->setFuture(future);
    }

    void processSelectedGrid(int input) {
        // std::cout << "processing input: " << input << std::endl;
        int row = 7 - input / 8;
        int col = input % 8;
        setSelectedGrid(row * 8 + col);
        if(useAI && gameEngine.board.getSideToMove() == Color::BLACK) {
            std::cout << "black player's turn. waiting for AI to make a move." << std::endl;
            return;
        } else {
            selectedPiece = gameEngine.board.getPiece(Position(input));
            if (startGrid == -1) {
                from = Position(input);
                if (!selectedPiece || selectedPiece->getColor() != gameEngine.board.getSideToMove()) {
                    std::cout << "invalid starting piece at " << from << ". please try again." << std::endl;
                    startGrid = -1;
                    return;
                }
                std::cout << "starting position selected: " << from << std::endl;
                startGrid = input;
                if(computeLegalMoves(input)) {
                    return;
                } else {
                    startGrid = -1;
                    return;
                }
            } else if (destGrid == -1) {
                destGrid = input;
                to = Position(input);
                auto destPiece = gameEngine.board.getPiece(to);
                if (!destPiece || destPiece->getColor() != gameEngine.board.getSideToMove()) {
                    std::cout << "destination position selected: " << to << ", proceeding to making the move now." << std::endl;

                } else {
                    std::cout << "changing starting position to " << to << "." << std::endl;
                    startGrid = destGrid; // since user selected a piece of the same color, assume it's the starting position
                    from = Position(input);
                    if (!selectedPiece || selectedPiece->getColor() != gameEngine.board.getSideToMove()) {
                        std::cout << "invalid starting piece at " << from << ". please try again." << std::endl;
                        startGrid = -1;
                        destGrid = -1;
                        return;
                    }
                    if(computeLegalMoves(input)) {
                        destGrid = -1;
                        return;
                    } else {
                        startGrid = -1;
                        destGrid = -1;
                        return;
                    }
                    destGrid = -1;
                    return;
                }
            }
        }
        executeMove();
        std::cout << "white move completed." << std::endl;
        if (useAI && gameEngine.board.getSideToMove() == Color::BLACK) {
            executeAIMove();
        }
    }
    void clearLegalMoves() {
        setLegalMoves(QList<int>());
    }
    bool computeLegalMoves(int grid) {
        auto newLegalMoves = QList<int>();
        std::shared_ptr<Piece> piece = gameEngine.board.getPiece(Position(grid));
        std::cout << "posiiton: " << piece->getPosition() << std::endl;
        std::vector<Position> legalMoves = gameEngine.board.getPiece(Position(grid))->generatePossibleMoves(gameEngine.board);
        if (legalMoves.empty()) {
            std::cout << "no legal moves for this piece." << std::endl;
            setLegalMoves(newLegalMoves);
            return false;
        }
        std::cout << "here are all the possible moves you could make: \n" << std::endl;
        for (auto pos : legalMoves) {
            std::cout << pos << ", ";
        }
        std::cout << std::endl;
        for (auto pos : legalMoves) {
            newLegalMoves.append((7-pos.row) * 8 + pos.col);
        }
        setLegalMoves(newLegalMoves);
        return true;
    }
    bool makeMove() {
        for (std::pair<int, int> pair: gameEngine.board.changedPositions) {
            m_dummyBoard[pair.first] = pieceRepresentations[pair.second];
        }
        // m_dummyBoard[destinationGrid] = m_dummyBoard[startingGrid];
        // m_dummyBoard[startingGrid] = "";
        boardHistory.push_back(m_dummyBoard);
        emit dummyBoardChanged();
        resetCapturedPieces();
        return true;
    }
    void resetCapturedPieces() {
        m_capturedWhitePiecesString.clear();
        m_capturedBlackPiecesString.clear();
        for(auto piece : gameEngine.board.capturedWhitePieces) {
            m_capturedWhitePiecesString.push_back(QString::fromStdString(piece->toString()));
        }
        emit capturedWhitePiecesStringChanged();
        for(auto piece : gameEngine.board.capturedBlackPieces) {
            std::cout << "captured black piece: " << piece->toString() << std::endl;
            m_capturedBlackPiecesString.push_back(QString::fromStdString(piece->toString()));
        }
        emit capturedBlackPiecesStringChanged();
    }
signals:
    void legalMovesChanged();
    void selectedGridChanged();

    void dummyBoardChanged();

    void currentTimerChanged();

    void whiteTimerChanged();

    void blackTimerChanged();

    void capturedWhitePiecesStringChanged();

    void capturedBlackPiecesStringChanged();

private:
    QList<int> m_legalMoves;
    int m_selectedGrid = -1;
    int m_startingGrid = -1;
    int m_destinationGrid = -1;

    QList<QString> m_dummyBoard = {
        "wr", "wn", "wb", "wq", "wk", "wb", "wn", "wr",
        "wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "",
        "bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp",
        "br", "bn", "bb", "bq", "bk", "bb", "bn", "br"
    };
    void printBoard() const {
        for (int i = 0; i < 8; ++i) {
            QString row;
            for (int j = 0; j < 8; ++j) {
                row += m_dummyBoard[i * 8 + j] + " ";
            }
            qInfo() << row;
        }
        qInfo() << "";
    }
    QString m_currentTimer;
    QTimer *m_currentTimeTimer;
    QString m_whiteTimer;
    QTimer *m_whiteTimeTimer;
    QTime m_whiteTimeRemaining;
    QString m_blackTimer;
    QTimer *m_blackTimeTimer;
    QTime m_blackTimeRemaining;
    int whitePlayerTimeMinutes = 1;
    int blackPlayerTimeMinutes = 1;
    int whiteTimeIncrementPerMove = 5;
    int blackTimeIncrementPerMove = 5;
    QList<QString> m_capturedWhitePiecesString;
    QList<QString> m_capturedBlackPiecesString;
};

#endif // GAMEINTERFACE_H
