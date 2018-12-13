#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <QMap>
#include <QString>
#include <QList>
#include <QVector2D>

class Game_core
{
    bool game_started;

public:
    Game_core();

    void iterate();
    bool is_game_started() { return game_started; }
    void conection_lost() {}
    bool start_game();
    bool stop_game();
    bool new_user(int);

    QString get_info_to_client();
};

#endif // GAME_CORE_H
