#include "mainwindow.h"
#include "simplecrypt.h"

#include <QCryptographicHash>
#include <QApplication>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    bool ok;
    QString strKey = QInputDialog::getText(this, tr("Senha"),
                                         tr("Entre com a senha:"), QLineEdit::Password,
                                         "", &ok);
    if (ok && !strKey.isEmpty())
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(strKey.toLatin1());

        QByteArray result = hash.result().toHex().mid(0, 16);

        bool ok;
        m_key = result.toULongLong(&ok, 16);
        if(!ok) m_key = 0;
    }
    else
    {
        m_key = 0;
    }

    m_textEdit = new QPlainTextEdit();
    m_btnSalvar = new QPushButton(tr("Salvar"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_textEdit);
    layout->addWidget(m_btnSalvar);
    setLayout(layout);

    connect(m_btnSalvar, SIGNAL(clicked()), this, SLOT(saveFile()));

    openFile();
}

MainWindow::~MainWindow()
{
}

QString MainWindow::getFileName()
{
#ifndef ANDROID
    static QString dataPath = qApp->applicationDirPath();
#else
    static QString dataPath = "/sdcard/FassiCrypt";
#endif

    QDir dir(dataPath);

    if(!dir.exists())
    {
        dir.mkpath(dataPath);
    }

    return dataPath + "/datafile.bin";
}

void MainWindow::openFile()
{
    if(!m_key) return;

    QFile file(getFileName());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, tr("Atenção"), tr("Um novo arquivo será criado"));
        return;
    }

    SimpleCrypt crypto(m_key);
    QString textCrypt(file.readAll());
    QString textDecript(crypto.decryptToString(textCrypt));

    bool validKey(false);
    if(textDecript.length() >= 16)
    {
        QString fileKey = textDecript.mid(textDecript.length()-16, 16);
        bool ok;
        if(fileKey.toULongLong(&ok, 16) == m_key)
        {
            validKey = ok;
        }
    }

    if(!validKey)
    {
        QMessageBox::critical(this, tr("Erro"), tr("Senha inválida!"));
        m_key = 0;
    }

    textDecript.remove(textDecript.length()-16, 16);

    m_textEdit->setPlainText(textDecript);
    file.close();
}

void MainWindow::saveFile()
{
    if(!m_key)
    {
        QMessageBox::critical(this, tr("Erro"), tr("Senha inválida!"));
        return;
    }

    QFile file(getFileName());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Erro"), tr("Não é possível criar ou salvar o arquivo"));
        return;
    }

    SimpleCrypt crypto(m_key);
    QTextStream out(&file);
    out << crypto.encryptToString(m_textEdit->toPlainText() + QString::number(m_key, 16));
    file.close();
}
