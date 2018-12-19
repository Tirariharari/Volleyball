#ifndef CLIENT_GRAPHICS_H
#define CLIENT_GRAPHICS_H

#include "client.h"
#include "settings.h"

class Client;

class ClientGraphics{
    float first_player_xy[2];
    float second_player_xy[2];
    float ball_xy[2];
public:
    ClientGraphics(){
        first_player_xy[0] = FIELD_MAX_X/4;
        first_player_xy[1] = BLOB_SIZE/2;
        second_player_xy[0] = 3*FIELD_MAX_X/4;
        second_player_xy[1] = BLOB_SIZE/2;
        ball_xy[0] = FIELD_MAX_X/2;
        ball_xy[1] = FIELD_MAX_Y/2 + BALL_SIZE/2;
    }
    friend class Client;
};

#endif // CLIENT_GRAPHICS_H
