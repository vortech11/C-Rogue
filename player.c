#include <stdio.h>
#include <math.h>

#include "player.h"
#include "render.h"

void movePlayer(Vector2 displacement) {
    player.position = add(player.position, displacement);
    renderEngine.camera = player.position;
}

void drawPlayer(char (*world)[renderEngine.x][z]){
    Vector2 point = transformPoint(player.position);
    setPosColor(world, (int)roundf(point.x), (int)roundf(point.y), PLAYER);
}