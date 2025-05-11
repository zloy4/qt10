#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addClient(const QString &name, QTcpSocket *socket);
    void removeClient(const QString &name, QTcpSocket *socket);

private:
    Ui::MainWindow *ui;
    Server *m_server;
};

#endif // MAINWINDOW_H
