#ifndef PLAYER_H
#define PLAYER_H

#include "point.h"
#include "render.h"

typedef struct {
    Vector2 position;
} Player;

extern Player player;

void movePlayer(Vector2 direction);

void drawPlayer(char (*world)[renderEngine.x][z]);

#endif