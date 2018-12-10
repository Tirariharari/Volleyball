/******************************************************************************
     * File: game_core.cpp
     * Description: The file contains the core of the game and it's realization
     * Created: 29.03.2018
     * Author: Gibshman Polina
     * Email: polina_gibshman@inbox.ru
******************************************************************************/

#include <math.h>
#include <QVector2D>
#include <QString>
#include <QMap>
#include "game_core.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
    //  Тут была выдача кораблей
    return true;
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


///  Дима, я не знаю, что ты пытался сказать этими комментариями, но они нечитабельны
///  Более того, get info нельзя. Можно только send или на худой конец give.
///  А get - это получать.

//Ship: Nickname number model x y rotation n/
//Bullet: x:y:owner n/

//  [server]3n/
//  Nick 1 1 1.73758 10.73728 278n/
//  Nost 2 5 2 5.48395 10n/
//  Rog 3 2 4.122 7.322 176n/
//  3.666:3.543:1 2.184:108.638:2n/

/*
QString Game_core::get_info_to_client()
{
    QString report = "[server]";
    report += QString::number(Ships.size());
    report += "\n";
    foreach (Ship i, Ships)
    {
        report += i.get_nickname();
        report += " ";
        report += QString::number(i.get_ship_number());
        report += " ";
        report += QString::number(i.get_model());
        report += " ";
        report += QString::number(i.get_pos_x());
        report += " ";
        report += QString::number(i.get_pos_y());
        report += " ";
        report += QString::number((int)i.get_rotation());
        report += "\n";
    }

    foreach (Bullet i, Bullets)
    {
        report += QString::number(i.get_pos_x());
        report += ":";
        report += QString::number(i.get_pos_y());
        report += ":";
        report += QString::number(i.owner_number);
        report += " ";
    }
    report += "\n";

    return report;
}
*/
