#include "mainwindow.h"
#include <QApplication>
#include <QKeyEvent>

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
    QApplication a(argc, argv);

    CEventMonitor monitor;
    a.installEventFilter(&monitor);

    MainWindow w;
    w.show();

    return a.exec();
}
