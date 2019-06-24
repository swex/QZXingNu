#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QZXingNu>
#include <QZXingNuFilter>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationName("QZXingNu_example");
    QQmlApplicationEngine engine;
#ifdef QT_QML_LIB
    QZXingNu::registerQMLTypes();
#endif
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
