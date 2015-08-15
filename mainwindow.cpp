#include "mainwindow.h"
#include "simplecrypt.h"

#include <QGuiApplication>
#include <QCryptographicHash>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QObject *parent)
    : QObject(parent)
{
}

MainWindow::~MainWindow()
{
}

bool MainWindow::login(const QString &pwd)
{
    bool ok(false);

    if (!pwd.isEmpty())
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(pwd.toLatin1());

        QByteArray result = hash.result().toHex().mid(0, 16);

        m_key = result.toULongLong(&ok, 16);
        if(!ok)
            m_key = 0;
        else
            ok &= openFile();
    }
    else
    {
        m_key = 0;
    }

    return ok;
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

bool MainWindow::openFile()
{
    if(!m_key)
    {
        m_textError = tr("Senha não definida!");
        return false;
    }

    QFile file(getFileName());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_textWarning = tr("Um novo arquivo será criado");
        return true;
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
        m_textError = tr("Senha inválida!");
        m_key = 0;
    }

    textDecript.remove(textDecript.length()-16, 16);

    m_textDecript = textDecript;
    emit textDecriptChanged();
    file.close();

    return validKey;
}

bool MainWindow::saveFile()
{
    if(!m_key)
    {
        m_textError = tr("Senha não definida!");
        return false;
    }

    QFile file(getFileName());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_textError = tr("Não é possível criar ou salvar o arquivo");
        return false;
    }

    SimpleCrypt crypto(m_key);
    QTextStream out(&file);
    out << crypto.encryptToString(m_textDecript + QString::number(m_key, 16));
    file.close();

    return true;
}
