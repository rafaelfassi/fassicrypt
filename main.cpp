#include "mainwindow.h"
#include <QKeyEvent>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class CEventMonitor : public QObject
{
    bool eventFilter(QObject *obj, QEvent *event)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if(keyEvent->key() == Qt::Key_Back)
            {
                qApp->quit();
                return true;
            }


        }
        return QObject::eventFilter(obj, event);
    }

};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    CEventMonitor monitor;
    app.installEventFilter(&monitor);

    MainWindow window;

    engine.rootContext()->setContextProperty("mainWindow", &window);
    engine.load(QUrl("qrc:/qml/MainWindow.qml"));


    return app.exec();
}
