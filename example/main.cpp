#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qzxingnu.h>
#include <qzxingnufilter.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationName("QZXingNu_example");
    QQmlApplicationEngine engine;
    qmlRegisterType<QZxingNuFilter>("com.github.swex.QZXingNu", 1, 0, "QZXingNuFilter");
    qmlRegisterType<QZXingNu>("com.github.swex.QZXingNu", 1, 0, "QZXingNu");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
