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

    Ball();
    void reset(bool side);
};

#endif // BALL_H
