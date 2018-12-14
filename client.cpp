#include "client.h"
#include "ui_client.h"
#include "client_functions.cpp"

#include <QKeyEvent>
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
        if (connected && movement_report != "[myship] *")
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
    //cs.appendInfo("We have data!");
}

void Client::connection_lost()
{
    //cs.appendInfo("Connection lost!");
    //cs.show();
}

void Client::keyPressEvent(QKeyEvent *event) // lf rt up dw // left right up down
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

void Client::keyReleaseEvent(QKeyEvent *event) // lf rt up dw // left right up down
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

    //connect( &tcpSocket, SIGNAL(readyRead()), SLOT(readTcpData()));

    //  Общение с сервером
    QString request = "[need_ticket] ";
    tcpSocket.write(request.toStdString().c_str());
}

