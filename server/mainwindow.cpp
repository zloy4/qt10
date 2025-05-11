#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h" // Добавляем include для Server
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_server(new Server(this)) // Инициализация m_server
{
    ui->setupUi(this);

    // Проверка элементов UI
    if (!ui->logTextEdit || !ui->clientList) {
        qCritical("Critical UI elements not found!");
        return;
    }

    // Настройка сервера
    if (!m_server->listen(QHostAddress::Any, 12345)) {
        logMessage("Server error: " + m_server->errorString());
    } else {
        logMessage("Server started on port " + QString::number(m_server->serverPort()));
    }

    // Подключение сигналов
    connect(m_server, &Server::newConnection, this, &MainWindow::addClient);
    connect(m_server, &Server::clientDisconnected, this, &MainWindow::removeClient);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClient);
    connect(ui->disconnectAllButton, &QPushButton::clicked, this, &MainWindow::onDisconnectAll);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logMessage(const QString &message)
{
    ui->logTextEdit->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ") + message);
}

void MainWindow::addClient(const QString &id, QTcpSocket *socket)
{
    QString clientInfo = QString("%1 (%2:%3)")
                        .arg(id)
                        .arg(socket->peerAddress().toString())
                        .arg(socket->peerPort());

    QListWidgetItem *item = new QListWidgetItem(clientInfo, ui->clientList);
    item->setData(Qt::UserRole, QVariant::fromValue(socket));
    m_clients[socket] = clientInfo;

    logMessage("Client connected: " + clientInfo);
}

void MainWindow::removeClient(QTcpSocket *socket)
{
    if (m_clients.contains(socket)) {
        logMessage("Client disconnected: " + m_clients.value(socket));
        m_clients.remove(socket);

        for (int i = 0; i < ui->clientList->count(); ++i) {
            if (ui->clientList->item(i)->data(Qt::UserRole).value<QTcpSocket*>() == socket) {
                delete ui->clientList->takeItem(i);
                break;
            }
        }
    }
}

void MainWindow::onDisconnectClient()
{
    QListWidgetItem *item = ui->clientList->currentItem();
    if (!item) return;

    QTcpSocket *socket = item->data(Qt::UserRole).value<QTcpSocket*>();
    if (socket) {
        socket->disconnectFromHost();
    }
}

void MainWindow::onDisconnectAll()
{
    if (m_clients.isEmpty()) {
        logMessage("No clients connected");
        return;
    }

    if (QMessageBox::question(this, "Disconnect all",
                            "Disconnect all clients?",
                            QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        for (QTcpSocket *socket : m_clients.keys()) {
            socket->disconnectFromHost();
        }
        logMessage("Disconnected all clients (" + QString::number(m_clients.size()) + ")");
    }
}
