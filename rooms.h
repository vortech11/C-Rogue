#ifndef ROOMS_H
#define ROOMS_H

#include "point.h"
#include "render.h"

typedef struct Room {
    Point* points;
    int pointNum;
    struct Room* next;
} Room;

extern Room* fistRoom;
extern Room* lastRoom;

Room* createRoom(Vector2 position, int pointNum, float radius, float rotation);

void drawRoom(char (*arr)[renderEngine.x][z], Room* room);

void drawRooms(char (*arr)[renderEngine.x][z]);

void freeRoomTree(Room* room);

void freeRooms();


#endif