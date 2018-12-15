#ifndef BLOB_H
#define BLOB_H

#include <QMap>
#include <QString>
#include <QList>
#include <QVector2D>

class Blob
{
public:
    int idusersoc;
    float dx;
    float x;
    float dy;
    float y;
    float minx;
    float maxx;
    float miny;
    float maxy;

    Blob();
    void init(bool side, int _idusersoc);
    void reset();
};

#endif // BLOB_H
