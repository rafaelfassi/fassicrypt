#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVariant>

#define DECLARE_PROPERTY(type, name) \
public: \
    Q_PROPERTY(type name READ name WRITE set_##name NOTIFY name##Changed) \
    \
    void set_##name( type var ) \
    {\
        m_##name = var;\
    }\
    \
    type name( void ) const \
    {\
        return m_##name; \
    }\
Q_SIGNALS: \
    void name##Changed(); \
private: \
    type m_##name;


class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow(QObject *parent = 0);
    ~MainWindow();

public slots:
    bool login(const QString &pwd);
    bool openFile();
    bool saveFile();

private:
    QString getFileName();  

    qulonglong m_key;
    DECLARE_PROPERTY(QString, textDecript)
    DECLARE_PROPERTY(QString, textError)
    DECLARE_PROPERTY(QString, textWarning)
};

#endif // MAINWINDOW_H
