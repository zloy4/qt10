#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_server = new Server(this);
    connect(m_server, &Server::clientConnected, this, &MainWindow::addClient);
    connect(m_server, &Server::clientDisconnected, this, &MainWindow::removeClient);

    m_server->startServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addClient(const QString &name, QTcpSocket *socket)
{
    Q_UNUSED(socket);
    ui->listWidget->addItem(name + " подключён");
}

void MainWindow::removeClient(const QString &name, QTcpSocket *socket)
{
    Q_UNUSED(socket);
    ui->listWidget->addItem(name + " отключён");
}
