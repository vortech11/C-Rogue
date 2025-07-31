#ifndef COLOR_H
#define COLOR_H

typedef struct {
    int R;
    int G;
    int B;
} Color;

Color createColor(int R, int G, int B);
char* formatColor(Color color, char *var);

#endif