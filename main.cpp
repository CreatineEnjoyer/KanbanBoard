#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "DatabaseSQLite.h"
#include "kanbanmodel.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    IDatabaseManage *dbManager = new DatabaseSQLite();
    KanbanModel kanbanModel(dbManager);

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
