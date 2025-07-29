#include <stdio.h> 
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>

struct Vector2;

const char BG = ' ';

// 1000 x 250 
// 100 x 18

int n = 100; //x
int m = 18; //y

struct Vector2 viewport;

struct Vector2 camera;

//float rotation = 0.785398;
float rotation = 0;

float zoom = 1;

bool running = true;

struct Color {
    int style;
    int color;
    int bg;
};

//0 Reset All Attributes (return to normal mode)
//1 Bright (usually turns on BOLD)
//2 Dim
//3 Underline
//5 Blink
//7 Reverse
//8 Hidden

// 30/40 Black
// 31/41 Red
// 32/42 Green
// 33/43 Yellow
// 34/44 Blue
// 35/45 Magenta
// 36/46 Cyan
// 37/47 White

struct Color createColor(int instyle, int incolor, int inbg){
    struct Color color;
    color.style = instyle; 
    color.color = incolor;
    color.bg = inbg;
    return color;
}



struct Vector2 {
    float x;
    float y;
};

void printVector2(struct Vector2 vector){
    printf("%f %f\n", vector.x, vector.y);
}

struct Vector2 add(struct Vector2 vector1, struct Vector2 vector2){
    vector1.x += vector2.x;
    vector1.y += vector2.y;
    return vector1;
}

struct Vector2 sub(struct Vector2 vector1, struct Vector2 vector2){
    vector1.x -= vector2.x;
    vector1.y -= vector2.y;
    return vector1;
}

struct Vector2 scale(struct Vector2 vector1, float value){
    vector1.x *= value;
    vector1.y *= value;
    return vector1;
}

struct Vector2 set(float value1, float value2){
    struct Vector2 vector = {value1, value2};
    return vector;
}

//struct Vector2* createVector2(float x, float y){
//    struct Vector2* vector = malloc(sizeof(struct Vector2));
//    vector->x = x;
//    vector->y = y;
//    return vector;
//}

//struct Vector2* copyVector2(struct Vector2* vector1){
//    if (!vector1){
//        fprintf(stderr, "copyVector2: input vector is NULL\n");
//        exit(EXIT_FAILURE);
//    }
//    struct Vector2* vector2 = malloc(sizeof(struct Vector2));
//    if (!vector2){
//        fprintf(stderr, "Failed to allocate memory for Vector2\n");
//        exit(EXIT_FAILURE);
//    }
//    vector2->x = vector1->x;
//    vector2->y = vector1->y;
//    return vector2;
//}

struct Vector2 apply2x2Matrix(struct Vector2 vector0, struct Vector2 vector1, struct Vector2 vector2){
    return set((vector1.x * vector0.x) + (vector2.x * vector0.y), (vector1.y * vector0.x) + (vector2.y * vector0.y));
}

struct Rect {
    struct Vector2 p1;
    struct Vector2 p2;
    struct Vector2 p3;
    struct Vector2 p4;
    int color;
    int fillType;
    struct Rect* next;
};

struct Rect* firstRect = NULL;
struct Rect* lastRect = NULL;

struct Rect* createRect(int inputsx, int inputsy, int inputex, int inputey, char inputcolor, int inputfillType){
    struct Rect* pointer = malloc(sizeof(struct Rect));
    pointer->p1 = set(inputsx, inputsy);
    pointer->p2 = set(inputex, inputsy);
    pointer->p3 = set(inputex, inputey);
    pointer->p4 = set(inputsx, inputey);
    pointer->color = inputcolor;
    pointer->fillType = inputfillType;
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

void printPos(char (*arr)[n], int x, int y){
    printf("%d", arr[y][x]);
}

void printEntire(void *inputArr){
    char *arr = inputArr;
    printf("\e[1;1H\e[2J");
    printf("%s\n", arr);
}

void oldPrint(char (*arr)[n]){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            printf("%c", arr[i][j]);
        }
    }
    printf("\n");
}

void setPos(char (*arr)[n], int x, int y, char value){
    if (x < n - 1 && x >= 0 && y < m && y >= 0) { arr[y][x] = value; }
}

void setEntire(char (*arr)[n], char value) {
    char endChar = '\n';
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n - 1; j++){
            setPos(arr, j, i, value);
        }
        if (i == m - 1){
            endChar = '\0';
        }
        arr[i][n - 1] = endChar;
    }
}

float edgeFunction(struct Vector2 a, struct Vector2 b, struct Vector2 c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void drawTriangle(char (*arr)[n], struct Vector2 A, struct Vector2 B, struct Vector2 C){
    const float ABC = edgeFunction(A, B, C);

    if (ABC < 0){
        return;
    }

    struct Vector2 P = {0, 0};

    const float minX = min(min(A.x, B.x), C.x);
    const float minY = min(min(A.y, B.y), C.y);
    const float maxX = max(max(A.x, B.x), C.x);
    const float maxY = max(max(A.y, B.y), C.y);

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

            setPos(arr, P.x, P.y, 'P');
        }
    }
}

void setAAline(char (*arr)[n], int startx, int starty, int endx, int endy, char value) {
    int start;
    int end;
    int same;
    int temp;
    bool vertical;
    if (startx == endx){
        start = starty;
        end = endy;
        same = startx;
        vertical = false;
    }
    else {
        start = startx;
        end = endx;
        same = starty;
        vertical = true;
    }
    if (start > end){
        temp = start;
        start = end;
        end = temp;
    }
    for(int i = start; i < end + 1; i++) {
        (vertical) ? setPos(arr, i, same, value) : setPos(arr, same, i, value);
    }
}

void drawLine(char (*arr)[n], int startx, int starty, int endx, int endy, char value){
    int dx = abs(endx - startx);
    int dy = abs(endy - starty);
    int sx = (startx < endx) ? 1 : -1;
    int sy = (starty < endy) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPos(arr, startx, starty, value);
        if (startx == endx && starty == endy) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; startx += sx; }
        if (e2 < dx)  { err += dx; starty += sy; }
    }
}

void setAAwireframeRect(char (*arr)[n], int startx, int starty, int endx, int endy, char value){
    setAAline(arr, startx, starty, endx, starty, value);
    setAAline(arr, endx, starty+1, endx, endy, value);
    setAAline(arr, startx, endy, endx - 1, endy, value);
    setAAline(arr, startx, starty + 1, startx, endy - 1, value);
}

void setAArect(char (*arr)[n], int startx, int starty, int endx, int endy, char value) {
    for (int i = startx; i < endx + 1; i++){
        for(int j = starty; j < endy + 1; j++){
            setPos(arr, i, j, value);
        }
    }
}

void drawRect(char (*arr)[n], struct Rect* rect, char value){
    struct Vector2 pointList[4];
    pointList[0] = rect->p1;
    pointList[1] = rect->p2;
    pointList[2] = rect->p3;
    pointList[3] = rect->p4;

    float rotationSin = sin(-rotation);
    float rotationCos = cos(-rotation);

    struct Vector2 matrixX = {rotationCos, rotationSin};
    struct Vector2 matrixY = {-rotationSin, rotationCos};
    struct Vector2 tempCamera = camera;
    struct Vector2 tempViewport = viewport;

    tempViewport = scale(tempViewport, 0.5);

    for(int i = 0; i < 4; i++){
        pointList[i] = sub(pointList[i], tempCamera);
        pointList[i] = apply2x2Matrix(pointList[i], matrixX, matrixY);
        pointList[i] = scale(pointList[i], zoom);
        pointList[i] = add(pointList[i], tempViewport);
    }
    
    for(int i = 0; i < 4; i++){
        drawLine(arr, round(pointList[i].x), round(pointList[i].y), round(pointList[(i + 1) % 4].x), round(pointList[(i + 1) % 4].y), value);
    }
};

void setRects(char (*arr)[n]){
    struct Rect* currentRect = firstRect;
    while (currentRect != NULL){
        int fillColor = currentRect->color;
        int startx = round(currentRect->p1.x);
        int starty = round(currentRect->p1.y);
        int endx = round(currentRect->p3.x);
        int endy = round(currentRect->p3.y);
        switch (currentRect->fillType){
            case 1:
                setAArect(arr, startx - camera.x, starty - camera.y, endx - camera.x, endy - camera.y, fillColor);
                break;
            case 2:
                setAAwireframeRect(arr, startx - camera.x, starty - camera.y, endx - camera.x, endy - camera.y, fillColor);
                break;
            case 3:
                drawRect(arr, currentRect, fillColor);
                break;
        }
        currentRect = currentRect->next;
    }
}

//void moveRect(struct Rect* rect, int dx, int dy){
//    struct Vector2 pointList[4]; 
//    pointList[0] = rect->p1;
//    pointList[1] = rect->p2;
//    pointList[2] = rect->p3;
//    pointList[3] = rect->p4;
//
//    for (int i = 0; i < 4; i++){
//        pointList[i].x += dx;
//        pointList[i].y += dy;
//    }
//}

void getUserInput(){
    _getch();
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;

    if (GetAsyncKeyState(VK_LEFT) & 0x8000) rotation -= 0.1;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) rotation += 0.1;
    if (GetAsyncKeyState(VK_UP) & 0x8000) zoom += 0.1;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000){ 
        zoom -= 0.1;
        if (zoom < 0) {
            zoom = 0;
        }
    }

    struct Vector2 move = {0, 0};
    struct Vector2 forward = {cosf(rotation), sinf(rotation)};
    struct Vector2 right = {-forward.y, forward.x};

    if (GetAsyncKeyState('A') & 0x8000) move = sub(move, forward);
    if (GetAsyncKeyState('D') & 0x8000) move = add(move, forward);
    if (GetAsyncKeyState('W') & 0x8000) move = sub(move, right);
    if (GetAsyncKeyState('S') & 0x8000) move = add(move, right);

    move = scale(move, 1);
    camera = add(camera, move);
}

void* generateWorld(void* inputWorld){
    char (*world)[n] = inputWorld;
    if (inputWorld == NULL) {
        world = malloc(sizeof(char[m][n]));
    }

    if (world == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    setEntire(world, BG);

    setAAwireframeRect(world, 0, 0, n - 2, m - 1, '#');

    setPos(world, (n - 2) / 2, (m - 1) / 2, '%');

    struct Vector2 P1 = {70, 7};
    struct Vector2 P2 = {90, 10};
    struct Vector2 P3 = {80, 12};

    drawTriangle(world, P1, P2, P3);

    //setAAline(world, 0, 0, n - 2, 0, '#');

    setRects(world);

    return world;
}

int main(){
    n++;

    camera = set(0, 0);
    viewport = set(n - 2, m - 1);

    //struct Rect* head = createRect(2, 2, 4, 4, '0', 1);
    //struct Rect* second = createRect(27, 3, 35, 8, '0', 3);
    struct Rect* first = createRect(-3, -3, 3, 3, 'e', 3);
    struct Rect* second = createRect(12, 12, 18, 18, 'e', 3);

    //printf("%d", head->next->next->starty);

    char (*world)[n] = NULL;
    
    while (running){
        world = generateWorld(world);

        //oldPrint(world);
        printEntire(world);

        getUserInput();

        Sleep(50);
    }

    free(world);
    freeRects();

    system("cls");
    return 0;
}

