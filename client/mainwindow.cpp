#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::deleteLater);
    nextBlockSize = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit_3->text() != "")
    {
        username = ui->lineEdit_3->text();
        socket->connectToHost(ui->lineEdit_2->text(), 2323);
        if (socket->waitForConnected(3000))
        {
            SendToServerConnect(username);

            QMessageBox box;
            box.setText("ПОДКЛЮЧЕНО");
            box.exec();
        }
        else
        {
            QMessageBox::warning(0, "ОШИБКА", "Неверный IP адрес");
        }
    }
    else
    {
        QMessageBox::warning(0, "ОШИБКА", "Введите имя пользователя");
    }
}

void MainWindow::SendToServer(QString mssng)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << username + ":\n-" + mssng;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
    mssg_buf = ui->lineEdit->text();
    ui->lineEdit->clear();
}

void MainWindow::SendToServerConnect(QString mssng)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << mssng;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);

    if (in.status() == QDataStream::Ok)
    {
        for (;;)
        {
            if (nextBlockSize == 0)
            {
                if (socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }

            if (socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }

            QString mssng;
            in >> mssng;
            nextBlockSize = 0;

            if (mssng == username + ":\n-" + mssg_buf)
            {
                ui->textBrowser->setAlignment(Qt::AlignRight);
                ui->textBrowser->append("Вы:");
                ui->textBrowser->append("-" + mssg_buf + "\n");
            }
            else if (mssng == username)
            {
                ui->textBrowser->setAlignment(Qt::AlignCenter);
                ui->textBrowser->append("Вы подключились\n");
            }
            else if (mssng != username && mssng.indexOf(":\n") >= 0)
            {
                ui->textBrowser->setAlignment(Qt::AlignLeft);
                ui->textBrowser->append(mssng + "\n");
            }
            else
            {
                ui->textBrowser->setAlignment(Qt::AlignCenter);
                ui->textBrowser->append(mssng + " подключился\n");
            }
        }
    }
    else
    {
        ui->textBrowser->append("Ошибка чтения...");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_2_returnPressed()
{
    if (ui->lineEdit_3->text() != "")
    {
        username = ui->lineEdit_3->text();
        socket->connectToHost(ui->lineEdit_2->text(), 2323);
        if (socket->waitForConnected(3000))
        {
            SendToServerConnect(username);

            QMessageBox box;
            box.setText("ПОДКЛЮЧЕНО");
            box.exec();
        }
        else
        {
            QMessageBox::warning(0, "ОШИБКА", "Неверный IP адрес");
        }
    }
    else
    {
        QMessageBox::warning(0, "ОШИБКА", "Введите имя пользователя");
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
}

