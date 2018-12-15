#ifndef BALL_H
#define BALL_H

#include <QMap>
#include <QString>
#include <QList>
#include <QVector2D>

class Ball
{
public:
    float x;
    float y;
    float dx;
    float dy;
    bool floating;

    float minx;
    float maxx;
    float miny;
    float maxy;

    Ball();
    void reset(float side); // 0-left 1-right 0.5-middle
};

#endif // BALL_H
