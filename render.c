#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "render.h"
#include "point.h"


void fastPrint(void* inarr){
    char *arr = inarr;
    printf("\e[1;1H\e[2J%s\n", arr);
}

void printColor(char color[z]){
    char *toBePrinted = malloc(22 * sizeof(char));
    strncpy(toBePrinted, color, z);
    toBePrinted[21] = '\0';
    printf("%s", toBePrinted);
    printf("\0[00;0;000;000;000m \n");
    free(toBePrinted);
}

void printColorChars(char color[z]){
    for(int i = 0; i < z; i++){
        printf("%c ", color[i]);
    }
    printf("\n");
}

void oldPrint(char (*arr)[renderEngine.x][z]){
    for(int i = 0; i < renderEngine.y; i++){
        for(int j = 0; j < renderEngine.x; j++){
            //for(int k = 0; k < z; k++){
            //    printf("%c ", arr[i][j][k]);
            //}
            printf("%s", arr[i][j]);
        }
        //printf("\n");
    }
    printf("\n");
}

void setPos(char (*arr)[renderEngine.x][z], int px, int py, char *value){
    if (px < renderEngine.x - 1 && px >= 0 && py < renderEngine.y && py >= 0) { strncpy(arr[py][px], value, z); }
}

void setPosColor(char (*arr)[renderEngine.x][z], int px, int py, Color color){
    char colorValue[z];
    formatColor(color, colorValue);
    setPos(arr, px, py, colorValue);
}

void setEntire(char (*arr)[renderEngine.x][z], char *value) {
    char endChar[] = "\033[00;0;000;000;000m \n";
    for(int i = 0; i < renderEngine.y; i++){
        for(int j = 0; j < renderEngine.x - 1; j++){
            setPos(arr, j, i, value);
        }
        if (i == renderEngine.y - 1){
            strncpy(endChar, "\033[00;0;000;000;000m \0", z);
        }
        strncpy(arr[i][renderEngine.x - 1], endChar, z);
    }
}

float edgeFunction(Vector2 a, Vector2 b, Vector2 c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void drawTriangle(char (*arr)[renderEngine.x][z], Vector2 A, Vector2 B, Vector2 C, Color CA, Color CB, Color CC){
    const float ABC = edgeFunction(A, B, C);

    if (ABC < 0){
        return;
    }

    Vector2 P = {0, 0};

    const float minX = fmin(fmin(A.x, B.x), C.x);
    const float minY = fmin(fmin(A.y, B.y), C.y);
    const float maxX = fmax(fmax(A.x, B.x), C.x);
    const float maxY = fmax(fmax(A.y, B.y), C.y);

    float ABP, BCP, CAP;
    float weightA, weightB, weightC;

    for(P.y = minY; P.y < maxY; P.y++){
        for(P.x = minX; P.x < maxX; P.x++){
            ABP = edgeFunction(A, B, P);
            BCP = edgeFunction(B, C, P);
            CAP = edgeFunction(C, A, P);

            if(ABP < 0 || BCP < 0 || CAP < 0) continue;

            weightA = BCP / ABC;
            weightB = CAP / ABC;
            weightC = ABP / ABC;

            float r = CA.R * weightA + CB.R * weightB + CC.R * weightC;
            float g = CA.G * weightA + CB.G * weightB + CC.G * weightC;
            float b = CA.B * weightA + CB.B * weightB + CC.B * weightC;
            Color newColor = {r, g, b};

            setPosColor(arr, P.x, P.y, newColor);
        }
    }
}

void drawLine(char (*arr)[renderEngine.x][z], int startx, int starty, int endx, int endy, Color value){
    int dx = abs(endx - startx);
    int dy = abs(endy - starty);
    int sx = (startx < endx) ? 1 : -1;
    int sy = (starty < endy) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPosColor(arr, startx, starty, value);
        if (startx == endx && starty == endy) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; startx += sx; }
        if (e2 < dx)  { err += dx; starty += sy; }
    }
}

Vector2 transformPoint(Vector2 pointPos){
    pointPos = sub(pointPos, renderEngine.camera);
    pointPos = apply2x2Matrix(pointPos, (Vector2){renderEngine.cachedCos, renderEngine.cachedSin}, (Vector2){-renderEngine.cachedSin, renderEngine.cachedCos});
    pointPos = scale(pointPos, renderEngine.zoom);
    pointPos = add(pointPos, scale(renderEngine.viewport, 0.5));
    return pointPos;
}