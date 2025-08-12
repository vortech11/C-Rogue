#ifndef COLOR_H
#define COLOR_H

typedef struct {
    int R;
    int G;
    int B;
} Color;

extern const Color WALL;
extern const Color PLAYER;

Color createColor(int R, int G, int B);
void printColor(Color color);
char* formatColor(Color color, char *var);

#endif