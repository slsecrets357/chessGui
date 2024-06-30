// console.h
#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <sstream>
#include <iostream>
#include <memory>

class Console : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString output READ output NOTIFY outputChanged)

public:
    explicit Console(QObject *parent = nullptr);

    QString output() const;

signals:
    void outputChanged();

public slots:
    void addConsoleOutput(const QString &message);

private slots:
    void updateOutput();

private:
    QString m_output;
    std::unique_ptr<std::ostringstream> m_outStream;
    QTimer m_timer;
};

#endif // CONSOLE_H
