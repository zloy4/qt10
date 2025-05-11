#include "server.h"
#include <QDebug>

Server::Server(QObject *parent)
    : QTcpServer(parent)
{}

void Server::startServer()
{
    if (listen(QHostAddress::Any, 1234)) {
        qDebug() << "Сервер запущен на порту 1234";
    } else {
        qDebug() << "Не удалось запустить сервер";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    socketToName[socket] = "???";  // имя клиента будет получено позже

    connect(socket, &QTcpSocket::readyRead, this, &Server::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisconnectedInternal);
}

void Server::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    QString &storedName = socketToName[socket];

    if (storedName == "???") {
        // Первое сообщение — имя клиента
        storedName = message;
        nameToSocket[storedName] = socket;
        qDebug() << storedName << " подключён.";
        emit clientConnected(storedName, socket);
        return;
    }

    QString senderName = storedName;

    if (message.startsWith("/to:")) {
        QStringList parts = message.split(':', Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            QString receiverName = parts[1].trimmed();
            QString msgContent = parts.mid(2).join(":").trimmed();
            QTcpSocket *receiverSocket = nameToSocket.value(receiverName, nullptr);

            if (receiverSocket) {
                QString formatted = QString("[ЛС от %1] %2\n").arg(senderName, msgContent);
                receiverSocket->write(formatted.toUtf8());
                socket->write(QString("[Вы -> %1] %2\n").arg(receiverName, msgContent).toUtf8());
            } else {
                socket->write("[Сервер] Получатель не найден\n");
            }
        }
    } else {
        QString fullMessage = QString("[%1]: %2\n").arg(senderName, message);
        for (QTcpSocket *sock : socketToName.keys()) {
            sock->write(fullMessage.toUtf8());
        }
    }
}

void Server::clientDisconnectedInternal()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString name = socketToName.value(socket);
    nameToSocket.remove(name);
    socketToName.remove(socket);

    emit clientDisconnected(name, socket);

    qDebug() << name << " отключён.";
    socket->deleteLater();
}
