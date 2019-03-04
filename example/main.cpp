#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QZXingNu>
#include <QZXingNuFilter>
#include <QDebug>

#ifdef QT_QML_LIB
static void registerQZXingNu()
{
    qRegisterMetaType<QZXingNu::QZXingNuDecodeResult>("QZXingNuDecodeResult");
    qRegisterMetaType<QZXingNu::DecodeStatus>("DecodeStatus");
    qRegisterMetaType<QZXingNu::BarcodeFormat>("BarcodeFormat");
    qRegisterMetaType<QZXingNu::QZXingNuDecodeResult>("QZXingNuDecodeResult");
    qmlRegisterUncreatableMetaObject(QZXingNu::staticMetaObject, "com.github.swex.QZXingNu", 1, 0,
                                     "QZXingNu", "Error: only enums allowed");
    qmlRegisterType<QZXingNu::QZXingNuFilter>("com.github.swex.QZXingNu", 1, 0, "QZXingNuFilter");
    qmlRegisterType<QZXingNu::QZXingNu>("com.github.swex.QZXingNu", 1, 0, "QZXingNu");
}
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationName("QZXingNu_example");
    QQmlApplicationEngine engine;
#ifdef QT_QML_LIB
    registerQZXingNu();
#endif
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
