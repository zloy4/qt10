#include "client.h"
#include <QMessageBox>

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &Client::connected);
    connect(socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        emit connectionError(socket->errorString());
    });
}

void Client::connectToServer(const QString &name)
{
    userName = name;
    socket->connectToHost("localhost", 12345);
}

void Client::sendMessage(const QString &message)
{
    if(socket->state() == QTcpSocket::ConnectedState) {
        QString formattedMessage = QString("[%1]: %2").arg(userName, message);
        socket->write(formattedMessage.toUtf8());
    }
}

void Client::onReadyRead()
{
    QByteArray data = socket->readAll();
    emit messageReceived(QString::fromUtf8(data));
}
