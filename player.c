


#include "player.h"
#include "render.h"

Player player;

const Color PLAYERCOLOR;

void movePlayer(Vector2 displacement) {
    player.position = add(player.position, displacement);
    renderEngine.camera = player.position;
    renderEngine.camera = sub(renderEngine.camera, scale(renderEngine.viewport, 0.5));
}

void drawPlayer(char (*world)[renderEngine.x][z]){
    setPosColor(world, player.position.x, player.position.y, PLAYERCOLOR);
}