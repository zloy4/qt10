// client/mainwindow.cpp

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QTcpSocket(this))
{
    ui->setupUi(this);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::on_sendButton_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::on_readyRead);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::on_disconnectButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    if (isConnected) return;

    QString username = ui->usernameEdit->text().trimmed();
    if (username.isEmpty()) {
        ui->chatEdit->append("Введите имя перед подключением.");
        return;
    }

    socket->connectToHost("127.0.0.1", 1234);
    if (socket->waitForConnected(1000)) {
        socket->write((username + "\n").toUtf8());
        isConnected = true;
        ui->chatEdit->append("Подключено как: " + username);
    } else {
        ui->chatEdit->append("Ошибка подключения к серверу.");
    }
}

void MainWindow::on_sendButton_clicked()
{
    if (!isConnected) {
        ui->chatEdit->append("Сначала подключитесь к серверу.");
        return;
    }

    QString message = ui->messageEdit->text().trimmed();
    QString recipient = ui->recipientEdit->text().trimmed();

    if (message.isEmpty()) return;

    if (!recipient.isEmpty()) {
        socket->write(QString("/to:%1:%2\n").arg(recipient, message).toUtf8());
    } else {
        socket->write(message.toUtf8() + "\n");
    }

    ui->messageEdit->clear();
}

void MainWindow::on_readyRead()
{
    QByteArray data = socket->readAll();
    ui->chatEdit->append(QString::fromUtf8(data));
}
void MainWindow::on_disconnectButton_clicked()
{
    if (!isConnected) {
        ui->chatEdit->append("Нет активного подключения.");
        return;
    }

    socket->disconnectFromHost();
    socket->close();
    isConnected = false;

    ui->chatEdit->append("Отключено от сервера.");
}
