#include "widget.h"
#include "ui_widget.h"

#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QTimerEvent>
#include <QPainter>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    server_status = false;
    users = 0;
    startTimer(1);
}

Widget::~Widget()
{
    delete ui;
}

//  Настройка сервера
void Widget::on_startServer_pushButton_clicked()
{
    if (server_status)
    {
        ui->info_textBrowser->append("Already work");
        return;
    }
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (!tcpServer->listen(QHostAddress::Any, 7777) && server_status==0)
    {
        ui->info_textBrowser->append(tcpServer->errorString());
    }
    else
    {
        server_status = true;
        QList <QHostAddress> addresses = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
        QString ip = "";
        ui->info_textBrowser->append("");
        foreach ( const QHostAddress &a, addresses)
        {
            switch ( a.protocol() )
            {
                case QAbstractSocket::IPv4Protocol:
                    ip = a.toString();
                    ui->info_textBrowser->append(ip + ":" + QString::number(tcpServer->serverPort()));
                    break;
                case QAbstractSocket::IPv6Protocol:
                    break;
                default:
                    break;
             }
         }
    }
}

//  Подключение нового клиента
void Widget::newuser()
{
    if (server_status && users<2)
    {
        //  Создание профиля клиента
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        users+=1;

        //  Настройка сигналов и слотов
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
        connect(SClients[idusersocs],SIGNAL(disconnected()),this, SLOT(slotDisconnetClient()));

        //  Вывод информации о количестве клиентов
        ui->info_textBrowser->append("Clients: " + QString::number(SClients.keys().size()));

        game_core.new_user(idusersocs);
    }
    else{
        //  Отправка ошибки клиенту
    }
}

//  Остановка сервера
void Widget::on_stopServer_pushButton_clicked()
{
    if(server_status==true)
    {
        //  Закрываем каждого клиента
        foreach(int i,SClients.keys())
        {
            SClients[i]->close();
        }
        //  Выключаем сервер
        tcpServer->close();
        ui->info_textBrowser->append(QString::fromUtf8("Server was stopped"));
        server_status=false;
    }
}

void Widget::on_clearInfo_pushButton_clicked()
{
    ui->info_textBrowser->clear();
}

void Widget::on_clients_pushButton_clicked()
{
    ui->info_textBrowser->append( "Clients : " + QString::number( SClients.size() ) );

    foreach(int i,SClients.keys())
    {
        ui->info_textBrowser->append(""+ QString::number(i));
    }

    ui->info_textBrowser->append(game_core.collect_info_for_client());
    //  Вывод ip пользователей
}

void Widget::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    QString request = clientSocket->readAll();

    ui->info_textBrowser->append(QString::number(idusersocs) +" "+ request);

    //  Обработка запроса пользователя
}

void Widget::slotDisconnetClient()
{
    foreach(int i,SClients.keys()){
        if(!SClients[i]->waitForConnected(100))
        {
            SClients[i]->close();
            SClients.remove(i);
        }
    }
    if(server_status==1)
    {
        ui->info_textBrowser->append("Clients: " + QString::number(SClients.keys().size()));
    }
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

void Widget::timerEvent(QTimerEvent *ev)
{
    game_core.iterate();
    update();
}


void Widget::on_start_game_pushButton_clicked()
{
    if (!server_status)
    {
        ui->info_textBrowser->append("You should start the server first");
        return;
    }
    if (!game_core.start_game())  //  <-Функция автоматически запускает игру
        ui->info_textBrowser->append("Impossible to start the game. Check if it is already started");
}

void Widget::on_stop_game_pushButton_clicked()
{
    if (!game_core.stop_game())  //  <-Функция автоматически останавливает игру
        ui->info_textBrowser->append("Impossible to stop the game. Don't be so nervous");
}
