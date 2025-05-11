#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHash>

class Server; // Предварительное объявление

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void logMessage(const QString &message);
    void addClient(const QString &id, QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);

private slots:
    void onDisconnectClient();
    void onDisconnectAll();

private:
    Ui::MainWindow *ui;
    Server *m_server;  // Используем префикс m_ для членов класса
    QHash<QTcpSocket*, QString> m_clients; // Хранение клиентов
};
#endif // MAINWINDOW_H
