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
    ball.reset(0.5);
    game_started = false;
}

void Game_core::iterate()
{
    if (game_started)
    {
        user1.dy -= GRAVITY;
        user1.y += user1.dy;
        user2.dy -= GRAVITY;
        user2.y += user2.dy;


        // USER1
        if((user1.x + user1.dx) < user1.maxx && (user1.x + user1.dx) > user1.minx)
        {
            user1.x += user1.dx;
            user1.dx = 0;
        }
        if(user1.y > user1.maxy)
        {
            user1.y = user1.maxy;
            user1.dy = 0;
        }
        if(user1.y < user1.miny)
        {
            user1.y = user1.miny;
            user1.dy = 0;
        }

        //USER2
        if((user2.x + user2.dx) < user2.maxx && (user2.x + user2.dx) > user2.minx)
        {
            user2.x += user2.dx;
            user2.dx = 0;
        }
        if(user2.y > user2.maxy)
        {
            user2.y = user2.maxy;
            user2.dy = 0;
        }
        if(user2.y < user2.miny)
        {
            user2.y = user2.miny;
            user2.dy = 0;
        }

        //BALL
        float bufx, bufy, bufl;
        bufx = ball.x - user1.x;
        bufy = ball.y - user1.y;
        bufl = sqrt(bufx*bufx + bufy*bufy);
        if(bufl < BALL_SIZE/2+BLOB_SIZE/2)
        {
            ball.floating = false;
            ball.dx = bufx/bufl*POWER;
            ball.dy = bufy/bufl*POWER;
        }

        bufx = ball.x - user2.x;
        bufy = ball.y - user2.y;
        bufl = sqrt(bufx*bufx + bufy*bufy);
        if(bufl < BALL_SIZE/2+BLOB_SIZE/2)
        {
            ball.floating = false;
            ball.dx = bufx/bufl*POWER;
            ball.dy = bufy/bufl*POWER;
        }

        if(ball.y < ball.miny)
        {
            ball.y = ball.miny;
            ball.dy *= -1;
        }
        if(ball.x < ball.minx)
        {
            ball.x = ball.minx;
            ball.dx *= -1;
        }
        if(ball.x > ball.maxx)
        {
            ball.x = ball.maxx;
            ball.dx *= -1;
        }

        if(ball.y < FIELD_MAX_Y/2)//Обработка сетки
        {
            //
        }

        if(!ball.floating)
        {
            ball.x += ball.dx;
            ball.y += ball.dy;
            ball.dy -= BALL_GRAVITY;
        }
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
    request += "*";
    return  request;
}
