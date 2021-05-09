#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGamepad/QGamepad>
#include <QList>

#include <QQmlEngine>
#include <QQmlComponent>
#include <QDebug>

#include "crpijoyemu.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<CRpiJoyEmu>("RpiJoyEmu", 1, 0, "RpiJoyEmu");

    QGamepadManager *gpad_manager = QGamepadManager::instance();
    QList<int> gpads;

    // workaround for getting joystick controls
    for (int i=0; i<1000; i++) {
        app.processEvents();
        gpads = gpad_manager->connectedGamepads();
        if (!gpads.isEmpty()) {
            qDebug() << "get gamepads";
            break;
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
