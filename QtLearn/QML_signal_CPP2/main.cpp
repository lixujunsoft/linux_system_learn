#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "myobject.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    /* 测试代码start */
    // 工程中一般使用此方法进行自定义复杂模块的注册
    qmlRegisterType<MyObject>("MyObj", 1, 0, "MyObject"); // 需要在QML中创建对象
    qmlRegisterSingletonInstance("MyObj", 1, 0, "MyObjectSingletonInstance", MyObject::getInstance());
    /* 测试代码start */

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // 在QML端发送信号，在C++端进行信号和槽的绑定
    // engine 加载完成后
    auto list = engine.rootObjects();
    auto window = list.first();
    QObject::connect(MyObject::getInstance(), SIGNAL(cppSig(QVariant,QVariant)), window, SLOT(qmlSlot(QVariant,QVariant)));

    return app.exec();
}
