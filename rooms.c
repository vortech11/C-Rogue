#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define _USE_MATH_DEFINES

#include "rooms.h"
#include "point.h"
#include "render.h"

RenderEngine renderEngine;

Room* firstRoom = NULL;
Room* lastRoom = NULL;

Room* createRoom(Vector2 position, int pointNum, float radius, float rotation){
    Room* room = (Room*)malloc(sizeof(Room));
    room->points = createPointList(pointNum);

    float pointRotate;
    float angleIncrement = (2.0f * M_PI) / pointNum;
    Point point = {WALL, {radius, 0}};
    for(int i = 0; i < pointNum; i++){
        pointRotate = -i * angleIncrement + rotation;
        point.position.x = radius * cos(pointRotate) + position.x;
        point.position.y = radius * sin(pointRotate) + position.y;

        room->points[i] = point;
    }

    room->pointNum = pointNum;
    room->next = NULL;
    if (firstRoom == NULL){
        firstRoom = room;
        lastRoom = room;
    } else {
        lastRoom->next = room;
        lastRoom = room;
    }
    return room;
}

void drawRoom(char (*arr)[renderEngine.x][z], Room* room){
    Point* points = room->points;
    Point* transformedPoints = createPointList(room->pointNum);

    float rotationSin = renderEngine.cachedSin;
    float rotationCos = renderEngine.cachedCos;

    Vector2 matrixX = {rotationCos, rotationSin};
    Vector2 matrixY = {-rotationSin, rotationCos};
    Vector2 tempCamera = renderEngine.camera;
    Vector2 tempViewport = renderEngine.viewport;
    
    tempViewport = scale(tempViewport, 0.5);
    
    Point point;

    for(int i = 0; i < room->pointNum; i++){
        point = points[i];
        point.position = sub(point.position, tempCamera);
        point.position = apply2x2Matrix(point.position, matrixX, matrixY);
        point.position = scale(point.position, renderEngine.zoom);
        point.position = add(point.position, tempViewport);
        transformedPoints[i] = point;
    }

    Vector2 point1;
    Vector2 point2;
    for(int i = 0; i < room->pointNum; i++){
        point1 = transformedPoints[i].position;
        point2 = transformedPoints[(i + 1) % room->pointNum].position;
        drawLine(arr, round(point1.x), round(point1.y), round(point2.x), round(point2.y), transformedPoints[i].color);
    }
}

void drawRooms(char (*arr)[renderEngine.x][z]){
    Room* currentRoom = firstRoom;
    while (currentRoom != NULL){
        drawRoom(arr, currentRoom);
        currentRoom = currentRoom->next;
    }
}

void freeRoomTree(Room* room){
    if (room == NULL) return;
    free(room->points);
    freeRoomTree(room->next);
    free(room);
}

void freeRooms(){
    freeRoomTree(firstRoom);
    firstRoom = NULL;
    lastRoom = NULL;
}
