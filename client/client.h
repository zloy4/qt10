#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void sendMessage(const QString &message);
    void connectToServer(const QString &name);

signals:
    void messageReceived(const QString &message);
    void connected();
    void connectionError(const QString &error);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;
    QString userName;
};

#endif // CLIENT_H
