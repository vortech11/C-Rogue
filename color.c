#include "color.h"
#include <stdio.h>

Color createColor(int R, int G, int B){
    Color color;
    color.R = R;
    color.G = G;
    color.B = B;
    return color;
}

const Color WALL = {255, 255, 255};
const Color PLAYER = {252, 186, 3};

void printColorValues(Color color){
    printf("%d %d %d\n", color.R, color.G, color.B);
}

char* formatColor(Color color, char *var){
    snprintf(var, 22, "\e[48;2;%03d;%03d;%03dm  ", color.R, color.G, color.B);
}