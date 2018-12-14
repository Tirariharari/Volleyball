#include <math.h>
#include <QVector2D>
#include <QString>
#include <QMap>
#include "game_core.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "settings.h"

Game_core::Game_core()
{
    game_started = false;
}

void Game_core::iterate()
{
    if (game_started)
    {
        //  Game
    }
}

bool Game_core::new_user(int id)
{
    if (game_started)
        return false;

    if(!user1.idusersoc)
    {
        user1.init(true , id);
        return true;
    }else if(!user2.idusersoc)
    {
        user2.init(false, id);
        return true;
    }
    return false;
}

bool Game_core::start_game()
{
    if (game_started)
        return false;
    game_started = true;
    return true;
}

bool Game_core::stop_game()
{
    if (!game_started)
        return false;  //  Err
    game_started = false;
    return true;  //  Game was successfully stoped
}

QString Game_core::collect_info_for_client()
{
    QString request = "[s] ";
    QString buf = "";
    request += buf.sprintf("%3.2f ", user1.x);
    request += buf.sprintf("%3.2f ", user1.y);
    request += buf.sprintf("%3.2f ", user2.x);
    request += buf.sprintf("%3.2f ", user2.y);
    request += buf.sprintf("%3.2f ", ball.x);
    request += buf.sprintf("%3.2f ", ball.y);
    return  request;
}
