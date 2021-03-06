#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

#include "client_graphics.h"

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    void connection_lost();
    explicit Client(QWidget *parent = 0);
    ~Client();
    QTcpSocket tcpSocket;
    bool connected;

    QString movement_report;
    bool keys_pressed[10] = {0,0,0,0,0,0,0,0,0,0};

private slots:
    void readTcpData();
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void createRequest();
    void on_connectButton_clicked();
    void paintEvent(QPaintEvent *ev);

private:
    Ui::Client *ui;
    ClientGraphics cg;
    QPixmap back;
    QPixmap player_1;
    QPixmap player_2;
    QPixmap ball;
};

#endif // CLIENT_H
