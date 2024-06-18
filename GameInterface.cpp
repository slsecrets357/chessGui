#include "GameInterface.h"
#include <iostream>
#include <string>
#include <QDateTime>

GameInterface::GameInterface(QObject *parent)
    : QObject{parent}, m_selectedGrid(-1)
{
    m_currentTimeTimer = new QTimer(this);
    m_currentTimeTimer->setInterval(500);
    m_currentTimeTimer->setSingleShot(true);
    connect( m_currentTimeTimer, &QTimer::timeout, this, &GameInterface::currentTimeTimerTimeout);
    currentTimeTimerTimeout();

    m_whiteTimeTimer = new QTimer(this);
    m_whiteTimeTimer->setInterval(50);
    connect(m_whiteTimeTimer, &QTimer::timeout, this, &GameInterface::whiteTimeTimerTimeout);

    m_whiteTimeRemaining = QTime(0, whitePlayerTimeMinutes, 0);
    setWhiteTimer(m_whiteTimeRemaining.toString("mm:ss.zzz"));
    m_whiteTimeTimer->start();

    m_blackTimeTimer = new QTimer(this);
    m_blackTimeTimer->setInterval(50);
    connect(m_blackTimeTimer, &QTimer::timeout, this, &GameInterface::blackTimeTimerTimeout);

    m_blackTimeRemaining = QTime(0, blackPlayerTimeMinutes, 0);
    setBlackTimer(m_blackTimeRemaining.toString("mm:ss.zzz"));
    m_blackTimeTimer->start();


    printBoard();
    boardHistory.push_back(m_dummyBoard);
}

QList<int> GameInterface::legalMoves() const
{
    return m_legalMoves;
}

void GameInterface::setLegalMoves(const QList<int> &newLegalMoves)
{
    if (m_legalMoves == newLegalMoves)
        return;
    m_legalMoves = newLegalMoves;
    emit legalMovesChanged();
}

int GameInterface::selectedGrid() const
{
    return m_selectedGrid;
}

void GameInterface::setSelectedGrid(int newSelectedGrid)
{
    if (m_selectedGrid == newSelectedGrid)
        return;
    m_selectedGrid = newSelectedGrid;
    emit selectedGridChanged();
}

QList<QString> GameInterface::dummyBoard() const
{
    return m_dummyBoard;
}

void GameInterface::setDummyBoard(const QList<QString> &newDummyBoard)
{
    if (m_dummyBoard == newDummyBoard)
        return;
    m_dummyBoard = newDummyBoard;
    emit dummyBoardChanged();
}

void GameInterface::currentTimeTimerTimeout()
{
    QDateTime dateTime;
    QString currentTime = dateTime.currentDateTime().toString( "hh:mm:ssap");
    setCurrentTimer(currentTime);
    m_currentTimeTimer->start();
}

void GameInterface::whiteTimeTimerTimeout()
{
    m_whiteTimeRemaining = m_whiteTimeRemaining.addMSecs(-50);
    setWhiteTimer("white: " + m_whiteTimeRemaining.toString("mm:ss.zzz"));

    if (m_whiteTimeRemaining == QTime(0, 0, 0)) {
        m_whiteTimeTimer->stop();
        // Handle timer end logic, e.g., notify the end of the game for the white player
        std::cout << "white timer ended. black wins!" << std::endl;
    }
}

void GameInterface::blackTimeTimerTimeout()
{
    m_blackTimeRemaining = m_blackTimeRemaining.addMSecs(-50);
    setBlackTimer("black: " + m_blackTimeRemaining.toString("mm:ss.zzz"));

    if (m_blackTimeRemaining == QTime(0, 0, 0)) {
        m_blackTimeTimer->stop();
        // Handle timer end logic, e.g., notify the end of the game for the black player
        std::cout << "black timer ended. white wins!" << std::endl;
    }
}

QString GameInterface::currentTimer() const
{
    return m_currentTimer;
}

void GameInterface::setCurrentTimer(const QString &newCurrentTimer)
{
    if (m_currentTimer == newCurrentTimer)
        return;
    m_currentTimer = newCurrentTimer;
    emit currentTimerChanged();
}

QString GameInterface::whiteTimer() const
{
    return m_whiteTimer;
}

void GameInterface::setWhiteTimer(const QString &newWhiteTimer)
{
    if (m_whiteTimer == newWhiteTimer)
        return;
    m_whiteTimer = newWhiteTimer;
    emit whiteTimerChanged();
}

QString GameInterface::blackTimer() const
{
    return m_blackTimer;
}

void GameInterface::setBlackTimer(const QString &newBlackTimer)
{
    if (m_blackTimer == newBlackTimer)
        return;
    m_blackTimer = newBlackTimer;
    emit blackTimerChanged();
}
