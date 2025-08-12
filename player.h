#ifndef PLAYER_H
#define PLAYER_H

#include "point.h"

typedef struct {
    Vector2 position;
} Player;

extern Player player;

void movePlayer(Vector2 direction);

void drawPlayer();

#endif