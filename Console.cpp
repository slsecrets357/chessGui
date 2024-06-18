// console.cpp
#include "Console.h"

Console::Console(QObject *parent)
    : QObject(parent), m_outStream(new std::ostringstream)
{
    std::cout.rdbuf(m_outStream->rdbuf());

    connect(&m_timer, &QTimer::timeout, this, &Console::updateOutput);
    m_timer.start(100); // Adjust interval as needed
}

QString Console::output() const
{
    return m_output;
}

void Console::updateOutput()
{
    QString newOutput = QString::fromStdString(m_outStream->str());
    if (newOutput != m_output)
    {
        m_output = newOutput;
        emit outputChanged();
    }
}

void Console::addConsoleOutput(const QString &message)
{
    std::cout << message.toStdString() << std::endl;
    updateOutput(); // Manually trigger an update
}
