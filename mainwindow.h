#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPlainTextEdit;
class QPushButton;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile();
    void saveFile();

private:
    QString getFileName();  

    QPlainTextEdit *m_textEdit;
    QPushButton *m_btnSalvar;
    qulonglong m_key;
};

#endif // MAINWINDOW_H
