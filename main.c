#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>

#include "vector2.h"
#include "color.h"
#include "render.h"
#include "point.h"
#include "rooms.h"
#include "player.h"

// 1000 x 250 
// 100 x 18

RenderEngine renderEngine;

Player player;

bool running = true;

const Color BG = {0, 0, 0};

void getUserInput(){
    _getch();
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;

    if (GetAsyncKeyState(VK_LEFT) & 0x8000) renderEngine.rotation -= 0.1;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) renderEngine.rotation += 0.1;
    if (GetAsyncKeyState(VK_UP) & 0x8000) renderEngine.zoom += 0.1;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000){ 
        renderEngine.zoom -= 0.1;
        if (renderEngine.zoom < 0) {
            renderEngine.zoom = 0;
        }
    }

    Vector2 move = {0, 0};
    Vector2 forward = {cosf(renderEngine.rotation), sinf(renderEngine.rotation)};
    Vector2 right = {-forward.y, forward.x};

    if (GetAsyncKeyState('A') & 0x8000) move = sub(move, forward);
    if (GetAsyncKeyState('D') & 0x8000) move = add(move, forward);
    if (GetAsyncKeyState('W') & 0x8000) move = sub(move, right);
    if (GetAsyncKeyState('S') & 0x8000) move = add(move, right);

    move = scale(move, 1);
    movePlayer(move);
}

void* generateWorld(void* inputWorld){
    char (*world)[renderEngine.x][z] = inputWorld;
    if (inputWorld == NULL) {
        world = malloc(sizeof(char[renderEngine.y][renderEngine.x][z]));
    }

    if (world == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    char colorValue[z];
    formatColor(BG, colorValue);

    setEntire(world, colorValue);

    renderEngine.cachedSin = sin(-renderEngine.rotation);
    renderEngine.cachedCos = cos(-renderEngine.rotation);

    drawRooms(world);

    drawPlayer(world);

    return world;
}

int main(){
    renderEngine.x = 100; 
    renderEngine.y = 25;

    renderEngine.rotation = 0;
    renderEngine.zoom = 1;

    renderEngine.camera = set(0, 0);
    renderEngine.viewport = set(renderEngine.x - 2, renderEngine.y - 1);

    player.position.x = 0;
    player.position.y = 0;

    Room* room1 = createRoom((Vector2){10, 10}, 5, 10, 0);
    Room* room2 = createRoom((Vector2){-10, 20}, 3, 15, 2);

    renderEngine.x++;
    
    char (*world)[renderEngine.x][z] = NULL;
    
    while (running){
        world = generateWorld(world);

        //oldPrint(world);
        fastPrint(world);

        getUserInput();

        Sleep(25);
    }

    free(world);

    freeRooms();
    system("cls");
    return 0;
}

