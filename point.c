#include <stdlib.h>
#include <stdio.h>

#include "point.h"

void printPoint(Point point){
    printColorValues(point.color);
    printVector2(point.position);
    printf("\n");
}

Point* createPointList(int size){
    if (size == 0) return NULL;

    Point* list = calloc(size, sizeof(Point));

    return list;
}