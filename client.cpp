#include "client.h"
#include "ui_client.h"
#include "client_functions.cpp"
#include "settings.h"
#include "textures/back_texture.xpm"
#include "textures/ball.xpm"
#include "textures/player_1.xpm"
#include "textures/player_2.xpm"

#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>
#include <QPixmap>

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    connected = false;

    startTimer(10);

    back = QPixmap (_back_texture);
    back = back.scaled (FIELD_MAX_X, FIELD_MAX_Y);

    player_1 = QPixmap (_player_1);
    player_1 = player_1.scaled(BLOB_SIZE, BLOB_SIZE);

    player_2 = QPixmap (_player_2);
    player_2 = player_2.scaled(BLOB_SIZE, BLOB_SIZE);

    ball = QPixmap(_ball);
    ball = ball.scaled(BALL_SIZE, BALL_SIZE);
}

Client::~Client()
{
    delete ui;
}

void Client::timerEvent(QTimerEvent *)
{
    createRequest();

    if (tcpSocket.waitForConnected(10) && movement_report != "[user] *")
        tcpSocket.write(movement_report.toStdString().c_str());
    else
    {
        if (connected && movement_report != "[user] *")
        {
            connected = false;
            connection_lost();
        }
    }
}

void Client::createRequest()
{
    //  Отчёт серверу о действиях пользователя
    movement_report = "[user] ";

    if(keys_pressed[0])
        movement_report += "lf ";

    if(keys_pressed[1])
        movement_report += "rt ";

    if(keys_pressed[2])
        movement_report += "up ";

    if(keys_pressed[3])
        movement_report += "dw ";

    movement_report += "*";

}

void Client::readTcpData()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    QString request = clientSocket->readAll();

    request = request.section("*", 0, 0);
    request = request.section("[s] ", 1, 1);
    cg.first_player_xy[0] = request.section(" ", 0, 0).toFloat();
    cg.first_player_xy[1] = request.section(" ", 1, 1).toFloat();
    cg.second_player_xy[0] = request.section(" ", 2, 2).toFloat();
    cg.second_player_xy[1] = request.section(" ", 3, 3).toFloat();
    cg.ball_xy[0] = request.section(" ", 4, 4).toFloat();
    cg.ball_xy[1] = request.section(" ", 5, 5).toFloat();
    update();
}

void Client::connection_lost()
{
    QMessageBox::information(0, "Error", "Connection lost");
}

void Client::keyPressEvent(QKeyEvent *event)
{
    //  Считывание нажатий пользователя
    switch (event->key())
    {

    case Qt::Key_A:
    case Qt::Key_Left:
        keys_pressed[0] = 1;
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        keys_pressed[1] = 1;
        break;

    case Qt::Key_W:
    case Qt::Key_Up:
        keys_pressed[2] = 1;
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        keys_pressed[3] = 1;
        break;

    case Qt::Key_Space:
    case Qt::Key_K:
        keys_pressed[4] = 1;
        break;

    default:
        break;
    }
}

void Client::keyReleaseEvent(QKeyEvent *event)
{
    //  Считывание "отжатий" пользователя
    switch (event->key())
    {

    case Qt::Key_A:
    case Qt::Key_Left:
        keys_pressed[0] = 0;
        break;

    case Qt::Key_D:
    case Qt::Key_Right:
        keys_pressed[1] = 0;
        break;

    case Qt::Key_W:
    case Qt::Key_Up:
        keys_pressed[2] = 0;
        break;

    case Qt::Key_S:
    case Qt::Key_Down:
        keys_pressed[3] = 0;
        break;

    case Qt::Key_Space:
    case Qt::Key_K:
        keys_pressed[4] = 0;
        break;

    default:
        break;
    }
}

void Client::on_connectButton_clicked()
{
    if (connected)
        return;

    QString ipText, portText;
    parsing_ip(ui->lineEdit->text(), ipText, portText);
    if(ipText == "Err")
    {
        QMessageBox::information(0, "Error", "Invalid ip");
        return;
    }
    tcpSocket.connectToHost(ipText, portText.toInt());

    if( tcpSocket.waitForConnected(100)) {

            connected = true;
    }else{
        QMessageBox::information(0, "Error", "Connection failed");
    }

    connect( &tcpSocket, SIGNAL(readyRead()), SLOT(readTcpData()));

    //  Общение с сервером
    QString request = "[need_ticket] ";
    tcpSocket.write(request.toStdString().c_str());
}

void Client::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(Qt::white);

    // Поле
    p.drawPixmap(0, 0, back);

    p.drawPolygon(QPolygon()<<QPoint(this->width()/2 - 5, height()/2)\
                  <<QPoint(this->width()/2 + 5, height()/2)\
                  <<QPoint(this->width()/2 - 5, this->height())\
                  <<QPoint(this->width()/2 + 5, this->height()));

    /*
    QPointF fieldCenter;
    fieldCenter.setX(this->width()/2);
    fieldCenter.setY(this->height()/2);
    QPointF bottomCenter;
    bottomCenter.setX(this->width()/2);
    bottomCenter.setY(this->height());

    p.drawLine(fieldCenter, bottomCenter);
    */

    // Игроки
    p.drawPixmap(cg.first_player_xy[0]-BLOB_SIZE/2, this->height()-cg.first_player_xy[1]-BLOB_SIZE/2, player_1);
    p.drawPixmap(cg.second_player_xy[0]-BLOB_SIZE/2, this->height()-cg.second_player_xy[1]-BLOB_SIZE/2, player_2);

    // Мяч
    p.drawPixmap(cg.ball_xy[0]-BALL_SIZE/2, this->height()-cg.ball_xy[1]-BALL_SIZE/2, ball);
}
