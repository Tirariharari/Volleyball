#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <QMap>
#include <QString>
#include <QList>
#include <QVector2D>

#include "blob.h"
#include "ball.h"

class Game_core
{
    bool game_started;

public:
    Blob user1;
    Blob user2;
    Ball ball;

    Game_core();

    void iterate();
    bool is_game_started() { return game_started; }
    void conection_lost() {}
    bool start_game();
    bool stop_game();
    bool new_user(int id);

    QString collect_info_for_client();
};

#endif // GAME_CORE_H
