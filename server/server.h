#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

signals:
    void newConnection(const QString &clientInfo, QTcpSocket *socket);
    void clientDisconnected(QTcpSocket *socket);
    void logMessage(const QString &message);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QMap<QTcpSocket*, QString> m_clients;
    quint16 m_nextClientId = 1;
};
#endif // SERVER_H
