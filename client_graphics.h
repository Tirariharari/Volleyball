#ifndef CLIENT_GRAPHICS_H
#define CLIENT_GRAPHICS_H

#include "client.h"

class Client;

class ClientGraphics{
    float first_player_xy[2];
    float second_player_xy[2];
    float ball_xy[2];
public:
    ClientGraphics(){
        first_player_xy[0] = 0;
        first_player_xy[1] = 0;
        second_player_xy[0] = 0;
        second_player_xy[1] = 0;
        ball_xy[0] = 0;
        ball_xy[1] = 0;
    }
    friend Client;
};

#endif // CLIENT_GRAPHICS_H
