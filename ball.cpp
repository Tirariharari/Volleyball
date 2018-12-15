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

    maxx = FIELD_MAX_X - BALL_SIZE/2;
    minx = BALL_SIZE/2;
    maxy = FIELD_MAX_Y - BALL_SIZE/2;
    miny = BALL_SIZE/2;

    floating = true;
}

void Ball::reset(float side)
{
    if(side == 0)
    {
        x = FIELD_MAX_X/4;
        y = FIELD_MAX_Y/2 + BALL_SIZE/2;
        dx = 0;
        dy = 0;
        floating = true;
    }
    if(side == 1)
    {
        x = 3*FIELD_MAX_X/4;
        y = FIELD_MAX_Y/2 + BALL_SIZE/2;
        dx = 0;
        dy = 0;
        floating = true;
    }
    if(side == 0.5)
    {
        x = FIELD_MAX_X/2;
        y = FIELD_MAX_Y/2 + BALL_SIZE/2;
        dx = 0;
        dy = 0;
        floating = true;
    }

}
