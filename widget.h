#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QTimerEvent>
#include <QPaintEvent>

#include "game_core.h"

namespace Ui {
class Widget;
}

class QTcpServer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_startServer_pushButton_clicked();
    void on_stopServer_pushButton_clicked();
    void on_clearInfo_pushButton_clicked();
    void on_clients_pushButton_clicked();
    void on_start_game_pushButton_clicked();
    void on_stop_game_pushButton_clicked();

    void newuser();
    void slotReadClient();
    void slotDisconnetClient();

    //void paintEvent(QPaintEvent *ev);
    void timerEvent(QTimerEvent *ev);

private:
    bool server_status;
    int users;


    Ui::Widget *ui;
    QTcpServer *tcpServer;
    QMap <int,QTcpSocket *> SClients;

    Game_core game_core;
    //void parsing ();
};

#endif // WIDGET_H
