#include "widget.h"
#include "ui_widget.h"
#include "settings.h"

#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QTimerEvent>
#include <QPainter>
#include <QString.h>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    server_status = false;
    users = 0;
    startTimer(2);
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
    game_core.stop_game();
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
    request = request.section("*", 0,0);
    Blob * buf = NULL;
    if(idusersocs == game_core.user1.idusersoc)
        buf = &game_core.user1;
    else if(idusersocs == game_core.user2.idusersoc)
        buf = &game_core.user2;


    if(buf){
    if ( request.contains("lf"))  //  left
        buf->dx = -BLOB_SPEED;
    if ( request.contains("rt"))  // right
        buf->dx = BLOB_SPEED;
    if ( request.contains("up")){ //  up
        if( buf->y < BLOB_SIZE/2+1)
            buf->dy = BLOB_JUMP_HIGH;
    }
    }else{
        ui->info_textBrowser->append("idusersoc err (restart game)");
    }
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

void Widget::timerEvent(QTimerEvent *ev)
{
    game_core.iterate();
    if(game_core.is_game_started())
    {
        //Рассылка информации о происходящем на сервере
        QString info = game_core.collect_info_for_client();
        foreach(int i,SClients.keys())
        {
            SClients[i]->write(info.toUtf8());
        }
    }
    update();
}


void Widget::on_start_game_pushButton_clicked()
{
    if (!server_status)
    {
        ui->info_textBrowser->append("You should start the server first");
        return;
    }
    if(game_core.user1.idusersoc == 0 || game_core.user2.idusersoc == 0)
    {
        ui->info_textBrowser->append("Need 2 players");
        return;
    }
    if (!game_core.start_game()){ //  <-Функция автоматически запускает игру
        ui->info_textBrowser->append("Impossible to start the game. Check if it is already started");
        foreach(int i,SClients.keys())
        {
            game_core.new_user(i);
        }
    }
}

void Widget::on_stop_game_pushButton_clicked()
{
    if (!game_core.stop_game())  //  <-Функция автоматически останавливает игру
        ui->info_textBrowser->append("Impossible to stop the game. Don't be so nervous");
}
