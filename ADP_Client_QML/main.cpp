#include <QApplication>
#include <QQmlApplicationEngine>
#include "TcpIpClient/signal_slot.h"
#include <QtDeclarative/QDeclarativeView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    QObject *topLevel = engine.rootObjects().value(0);
    QObject *slidebar = topLevel->findChild<QObject *>("slider_Dobra");

    TCP_Song a1;
    QObject::connect(topLevel, SIGNAL(qmlSignal(QString)), &a1, SLOT(cppSlot(QString)));
    QObject::connect(&a1, SIGNAL(valueSignal(QVariant)), topLevel, SLOT(qmlSlot(QVariant)));
    QObject::connect(&a1, SIGNAL(SongDurationSignal(QVariant)), topLevel, SLOT(qmlSongDuration(QVariant)));

    a1.requestNewConnection();
    return app.exec();
}
