#include "client.h"
#include "ui_client.h"
#include "client_functions.cpp"
#include "settings.h"

#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    connected = false;

    startTimer(10);
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
    int idusersocs=clientSocket->socketDescriptor();
    QString request = clientSocket->readAll();

    request = request.section("*", 0, 0);
    request = request.section("[s] ", 1, 1);
    //ui->textBrowser->append(request);
    cg.first_player_xy[0] = request.section(" ", 0, 0).toFloat();
    cg.first_player_xy[1] = request.section(" ", 1, 1).toFloat();
    cg.second_player_xy[0] = request.section(" ", 2, 2).toFloat();
    cg.second_player_xy[1] = request.section(" ", 3, 3).toFloat();
    cg.ball_xy[0] = request.section(" ", 4, 4).toFloat();
    cg.ball_xy[1] = request.section(" ", 5, 5).toFloat();
    //ui->textBrowser->append(QString::number(cg.second_player_xy[1]));
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
            //QMessageBox::information(0, "Info", "Connected to server");
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
    p.setBrush(Qt::black);

    // Поле
    QPointF fieldCenter;
    fieldCenter.setX(this->width()/2);
    fieldCenter.setY(this->height()/2);
    QPointF bottomCenter;
    bottomCenter.setX(this->width()/2);
    bottomCenter.setY(this->height());

    p.drawLine(fieldCenter, bottomCenter);

    // МБ с радиусом что-то не то
    // Игроки
    p.drawEllipse(cg.first_player_xy[0], this->height()-cg.first_player_xy[1]-BLOB_SIZE/4, BLOB_SIZE/2, BLOB_SIZE/2);
    p.drawEllipse(cg.second_player_xy[0], this->height()-cg.second_player_xy[1]-BLOB_SIZE/4, BLOB_SIZE/2, BLOB_SIZE/2);

    // Мяч
     p.drawEllipse(cg.ball_xy[0], this->height()-cg.ball_xy[1]-BALL_SIZE/4, BALL_SIZE/2, BALL_SIZE/2);
}

/*
void Widget::paintEvent(QPaintEvent *ev)
{
    QVector2D fieldCenter;
    fieldCenter.setX((this->width()-256)/2);
    fieldCenter.setY(this->height()/2);

    int shipSize = 10;

    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(Qt::black);
    p.drawEllipse(fieldCenter.x()-BORDER, fieldCenter.y()-BORDER, BORDER*2, BORDER*2);
    p.setBrush(Qt::yellow);
    p.drawEllipse(fieldCenter.x()-shipSize/2, fieldCenter.y()-shipSize/2, shipSize, shipSize);

    foreach (int key, game_core.Ships.keys()) {
        shipSize = game_core.Ships[key].get_size();
        p.setBrush(Qt::white);
        p.drawEllipse(fieldCenter.x()-shipSize/2 + game_core.Ships[key].get_pos_x(),
                      fieldCenter.y()-shipSize/2 + game_core.Ships[key].get_pos_y(), shipSize, shipSize);
        p.setBrush(Qt::red);

        QVector2D gof;
        gof.setX(cos(game_core.Ships[key].get_rotation()/180*M_PI)*(-shipSize));
        gof.setY(sin(game_core.Ships[key].get_rotation()/180*M_PI)*(shipSize));
        gof*=(-1);
        p.drawEllipse(fieldCenter.x()-shipSize/8 + game_core.Ships[key].get_pos_x() + gof.x(),
                      fieldCenter.y()-shipSize/8 + game_core.Ships[key].get_pos_y() + gof.y(),
                      shipSize/4, shipSize/4);

    }

    shipSize = 4;
    p.setPen(Qt::black);
    p.setBrush(Qt::red);

    QList<Bullet>::iterator it = game_core.Bullets.begin();
    for ( ; it != game_core.Bullets.end(); ++it ) {
        Bullet& bull = *it;
        p.drawEllipse(fieldCenter.x()-shipSize/2 + bull.get_pos_x(),
                      fieldCenter.y()-shipSize/2 + bull.get_pos_y(), shipSize, shipSize);
    }
}
*/
