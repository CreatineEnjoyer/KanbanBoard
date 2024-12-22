#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "kanbanmodel.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    KanbanModel kanbanModel;

    engine.rootContext()->setContextProperty("kanbanModel", &kanbanModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("KanbanBoard", "Main");

    return app.exec();
}
