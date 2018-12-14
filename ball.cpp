#include "widget.h"
#include "ui_widget.h"

#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QTimerEvent>
#include <QPainter>

#include "settings.h"
#include "ball.h"

Ball::Ball()
{
    x = 0;
    y = 0;
    dx = 0;
    dy = 0;
    floating = true;
}
