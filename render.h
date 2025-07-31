#ifndef RENDER_H
#define RENDER_H

#define z 21

#include "vector2.h"
#include "color.h"


typedef struct {
    int x; //x
    int y; //y

    Vector2 viewport;

    Vector2 camera;

    float rotation;

    float zoom;


} RenderEngine;

extern RenderEngine renderEngine;

void fastPrint(void* inarr);
 
void oldPrint(char (*arr)[renderEngine.x][z]);
 
void setPos(char (*arr)[renderEngine.x][z], int px, int py, char *value);
 
void setPosColor(char (*arr)[renderEngine.x][z], int px, int py, Color color);
 
void setEntire(char (*arr)[renderEngine.x][z], char *value);

float edgeFunction(Vector2 a, Vector2 b, Vector2 c);
 
void drawTriangle(char (*arr)[renderEngine.x][z], Vector2 A, Vector2 B, Vector2 C, Color CA, Color CB, Color CC);
 
void drawLine(char (*arr)[renderEngine.x][z], int startx, int starty, int endx, int endy, Color value);
 
 
#endif