#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
    void startServer();

signals:
    void clientConnected(const QString &name, QTcpSocket *socket);
    void clientDisconnected(const QString &name, QTcpSocket *socket);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void readyRead();
    void clientDisconnectedInternal();

private:
    QMap<QTcpSocket*, QString> socketToName;
    QMap<QString, QTcpSocket*> nameToSocket;
};

#endif // SERVER_H
