#ifndef POINT_H
#define POINT_H

#include "vector2.h"
#include "color.h"

typedef struct {
    Color color;
    Vector2 position;
} Point;

typedef struct PointList {
    Point value;
    struct PointList *next;
} PointList;

void printPoint(Point point);

Point* createPointList(int size);

#endif