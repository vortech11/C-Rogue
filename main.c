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


// 1000 x 250 
// 100 x 18

RenderEngine renderEngine;

bool running = true;

const Color BG = {0, 0, 0};
const Color PlayerColor = {252, 186, 3};
const Color wallColor = {255, 255, 255};

struct Rect {
    Vector2 p1;
    Vector2 p2;
    Vector2 p3;
    Vector2 p4;
    Color color;
    struct Rect* next;
};

struct Rect* firstRect = NULL;
struct Rect* lastRect = NULL;

struct Rect* createRect(int inputsx, int inputsy, int inputex, int inputey, Color inputcolor){
    struct Rect* pointer = malloc(sizeof(struct Rect));
    pointer->p1 = set(inputsx, inputsy);
    pointer->p2 = set(inputex, inputsy);
    pointer->p3 = set(inputex, inputey);
    pointer->p4 = set(inputsx, inputey);
    pointer->color = inputcolor;
    pointer->next = NULL;
    if (firstRect == NULL){
        firstRect = pointer;
        lastRect = pointer;
    }
    else {
        lastRect->next = pointer;
    }
    lastRect = pointer;
    return pointer;
}

void recFreeRects(struct Rect* currentRect){
    if (currentRect != NULL){
        recFreeRects(currentRect->next);
        free(currentRect);
    }
}

void freeRects(){
    recFreeRects(firstRect);
}



void drawRect(char (*arr)[renderEngine.x][z], struct Rect* rect, Color value){
    Vector2 pointList[4];
    pointList[0] = rect->p1;
    pointList[1] = rect->p2;
    pointList[2] = rect->p3;
    pointList[3] = rect->p4;

    float rotationSin = sin(-renderEngine.rotation);
    float rotationCos = cos(-renderEngine.rotation);

    Vector2 matrixX = {rotationCos, rotationSin};
    Vector2 matrixY = {-rotationSin, rotationCos};
    Vector2 tempCamera = renderEngine.camera;
    Vector2 tempViewport = renderEngine.viewport;

    tempViewport = scale(tempViewport, 0.5);

    for(int i = 0; i < 4; i++){
        pointList[i] = sub(pointList[i], tempCamera);
        pointList[i] = apply2x2Matrix(pointList[i], matrixX, matrixY);
        pointList[i] = scale(pointList[i], renderEngine.zoom);
        pointList[i] = add(pointList[i], tempViewport);
    }
    
    for(int i = 0; i < 4; i++){
        drawLine(arr, round(pointList[i].x), round(pointList[i].y), round(pointList[(i + 1) % 4].x), round(pointList[(i + 1) % 4].y), value);
    }
};

void setRects(char (*arr)[renderEngine.x][z]){
    struct Rect* currentRect = firstRect;
    while (currentRect != NULL){
        Color fillColor = currentRect->color;
        int startx = round(currentRect->p1.x);
        int starty = round(currentRect->p1.y);
        int endx = round(currentRect->p3.x);
        int endy = round(currentRect->p3.y);
        drawRect(arr, currentRect, fillColor);

        currentRect = currentRect->next;
    }
}

////void moveRect(struct Rect* rect, int dx, int dy){
////    Vector2 pointList[4]; 
////    pointList[0] = rect->p1;
////    pointList[1] = rect->p2;
////    pointList[2] = rect->p3;
////    pointList[3] = rect->p4;
////
////    for (int i = 0; i < 4; i++){
////        pointList[i].x += dx;
////        pointList[i].y += dy;
////    }
////}

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
    renderEngine.camera = add(renderEngine.camera, move);
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

    setPosColor(world, (renderEngine.x - 2) / 2, (renderEngine.y - 1) / 2, PlayerColor);

    /*
    Vector2 P1 = {70, 7};
    Vector2 P2 = {90, 10};
    Vector2 P3 = {80, 16};

    Color C1 = {255, 0, 0};
    Color C2 = {0, 255, 0};
    Color C3 = {0, 0, 255};
    
    drawTriangle(world, P1, P2, P3, C1, C2, C3);
    */

    setRects(world);

    return world;
}

int main(){
    renderEngine.x = 100; 
    renderEngine.y = 18;
    
    renderEngine.x++;

    renderEngine.rotation = 0;
    renderEngine.zoom = 1;

    renderEngine.camera = set(0, 0);
    renderEngine.viewport = set(renderEngine.x - 2, renderEngine.y - 1);

    struct Rect* second = createRect(27, 3, 35, 8, wallColor);
    struct Rect* first = createRect(-3, -3, 3, 3, wallColor);
    //struct Rect* second = createRect(12, 12, 18, 18, wallColor, 3);

    char (*world)[renderEngine.x][z] = NULL;
    
    while (running){
        world = generateWorld(world);

        //oldPrint(world);
        fastPrint(world);

        getUserInput();

        Sleep(25);
    }

    free(world);
    freeRects();

    system("cls");
    return 0;
}

