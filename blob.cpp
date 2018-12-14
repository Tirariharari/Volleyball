#include "widget.h"
#include "ui_widget.h"

#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QTimerEvent>
#include <QPainter>

#include "settings.h"
#include "blob.h"

Blob::Blob()
{
    idusersoc = 0;
    x = 0;
    y = 0;
    minx = 0;
    maxx = 0;
    miny = 0;
    maxy = 0;

}

void Blob::init(bool side, int _idusersoc)
{
    idusersoc = _idusersoc;
    if(side)
    {   //left side
        x = FIELD_MAX_X/4;
        dy = 0;
        y = BLOB_SIZE/2;
        minx = 0;
        maxx = FIELD_MAX_X/2;
        miny = 0;
        maxy = FIELD_MAX_Y;
    }else
    {   //right side
        x = 3*FIELD_MAX_X/4;
        dy = 0;
        y = BLOB_SIZE/2;
        minx = FIELD_MAX_X/2;
        maxx = FIELD_MAX_X;
        miny = 0;
        maxy = FIELD_MAX_Y;
    }
}
