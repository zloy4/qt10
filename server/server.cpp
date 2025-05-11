#include "server.h"
#include <QHostAddress>

Server::Server(QObject *parent) : QTcpServer(parent) {}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        emit logMessage(tr("Ошибка подключения: %1").arg(socket->errorString()));
        delete socket;
        return;
    }

    QString clientId = QString("Клиент #%1").arg(m_nextClientId++);
    m_clients.insert(socket, clientId);

    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);

    emit newConnection(clientId, socket);
    emit logMessage(tr("Новое подключение: %1").arg(clientId));
}

void Server::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    // Обработка входящих данных
    emit logMessage(tr("Данные от %1: %2").arg(m_clients.value(socket), QString::fromUtf8(data)));
}

void Server::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString clientInfo = m_clients.value(socket);
    m_clients.remove(socket);
    socket->deleteLater();

    emit clientDisconnected(socket);
    emit logMessage(tr("Отключение: %1").arg(clientInfo));
}
