#include "onlyapp.h"
#include <QCoreApplication>
#include <QLocalSocket>
#include <QFile>
#include <QDebug>

OnlyApp::OnlyApp(QObject *parent) : QObject(parent)
{

}

OnlyApp::~OnlyApp()
{
    m_localServer->close();
    m_localServer->deleteLater();
}

bool OnlyApp::isOnly()
{
    QString serverName = "34382e82a01cd8b5269d7f7d4fe438d8";
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (socket.waitForConnected(500)) {
        QTextStream stream(&socket);
        stream << "hellow";
        stream.flush();
        socket.waitForBytesWritten();

        return false;
    }

    m_localServer = new QLocalServer(this);
    connect(m_localServer, SIGNAL(newConnection()), this, SLOT(newLocalSocketConnection()));
    if (!m_localServer->listen(serverName)) {
        if (m_localServer->serverError() == QAbstractSocket::AddressInUseError
            && QFile::exists(m_localServer->serverName())) {
            QFile::remove(m_localServer->serverName());
            m_localServer->listen(serverName);
        }
    }

    return true;
}

void OnlyApp::newLocalSocketConnection()
{
    if (m_localServer) {
        return ;
    }

    QLocalSocket *socket = m_localServer->nextPendingConnection();
    if (!socket)
        return;
    socket->waitForReadyRead(1000);
    QTextStream stream(socket);
    stream.readAll();

    delete socket;
}
