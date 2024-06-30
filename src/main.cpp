#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include "GameInterface.h"
#include "Console.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    GameInterface m_gameInterface;
    Console m_console;

    QQmlApplicationEngine engine;

    QQmlContext * context(engine.rootContext());
    context->setContextProperty("gameInterface", &m_gameInterface);
    context->setContextProperty("console", &m_console);

    const QUrl url(QStringLiteral("qrc:/Chess2/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
