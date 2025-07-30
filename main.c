#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>

struct Vector2;

// 1000 x 250 
// 100 x 18

int x = 1000; //x
int y = 250; //y
const int z = 21; //z

struct Vector2 viewport;

struct Vector2 camera;

//float rotation = 0.785398;
float rotation = 0;

float zoom = 1;

bool running = true;

struct Color {
    int R;
    int G;
    int B;
};

const struct Color BG = {0, 0, 0};
const struct Color PlayerColor = {252, 186, 3};
const struct Color wallColor = {255, 255, 255};

struct Color createColor(int R, int G, int B){
    struct Color color;
    color.R = R;
    color.G = G;
    color.B = B;
    return color;
}

char* formatColor(struct Color color, char *var){
    snprintf(var, z+1, "\e[48;2;%03d;%03d;%03dm  ", color.R, color.G, color.B);
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
    struct Color color;
    struct Rect* next;
};

struct Rect* firstRect = NULL;
struct Rect* lastRect = NULL;

struct Rect* createRect(int inputsx, int inputsy, int inputex, int inputey, struct Color inputcolor){
    struct Rect* pointer = malloc(sizeof(struct Rect));
    pointer->p1 = set(inputsx, inputsy);
    pointer->p2 = set(inputex, inputsy);
    pointer->p3 = set(inputex, inputey);
    pointer->p4 = set(inputsx, inputey);
    pointer->color = inputcolor;
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

void fastPrint(void* inarr){
    char *arr = inarr;
    printf("\e[1;1H\e[2J%s\n", arr);
}

void oldPrint(char (*arr)[x][z]){
    for(int i = 0; i < y; i++){
        for(int j = 0; j < x; j++){
            //for(int k = 0; k < z; k++){
            //    printf("%c ", arr[i][j][k]);
            //}
            printf("%s", arr[i][j]);
        }
        //printf("\n");
    }
    printf("\n");
}

void setPos(char (*arr)[x][z], int px, int py, char *value){
    if (px < x - 1 && px >= 0 && py < y && py >= 0) { strncpy(arr[py][px], value, z); }
}

void setPosColor(char (*arr)[x][z], int px, int py, struct Color color){
    char colorValue[z];
    formatColor(color, colorValue);
    setPos(arr, px, py, colorValue);
}

void setEntire(char (*arr)[x][z], char *value) {
    char endChar[] = "\033[00;0;000;000;000m \n";
    for(int i = 0; i < y; i++){
        for(int j = 0; j < x - 1; j++){
            setPos(arr, j, i, value);
        }
        if (i == y - 1){
            strncpy(endChar, "\033[00;0;000;000;000m \0", z);
        }
        strncpy(arr[i][x - 1], endChar, z);
    }
}

float edgeFunction(struct Vector2 a, struct Vector2 b, struct Vector2 c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void drawTriangle(char (*arr)[x][z], struct Vector2 A, struct Vector2 B, struct Vector2 C, struct Color CA, struct Color CB, struct Color CC){
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

            float r = CA.R * weightA + CB.R * weightB + CC.R * weightC;
            float g = CA.G * weightA + CB.G * weightB + CC.G * weightC;
            float b = CA.B * weightA + CB.B * weightB + CC.B * weightC;
            struct Color newColor = {r, g, b};

            setPosColor(arr, P.x, P.y, newColor);
        }
    }
}

void drawLine(char (*arr)[x][z], int startx, int starty, int endx, int endy, struct Color value){
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

void drawRect(char (*arr)[x][z], struct Rect* rect, struct Color value){
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

void setRects(char (*arr)[x][z]){
    struct Rect* currentRect = firstRect;
    while (currentRect != NULL){
        struct Color fillColor = currentRect->color;
        int startx = round(currentRect->p1.x);
        int starty = round(currentRect->p1.y);
        int endx = round(currentRect->p3.x);
        int endy = round(currentRect->p3.y);
        drawRect(arr, currentRect, fillColor);

        currentRect = currentRect->next;
    }
}

////void moveRect(struct Rect* rect, int dx, int dy){
////    struct Vector2 pointList[4]; 
////    pointList[0] = rect->p1;
////    pointList[1] = rect->p2;
////    pointList[2] = rect->p3;
////    pointList[3] = rect->p4;
////
////    for (int i = 0; i < 4; i++){
////        pointList[i].x += dx;
////        pointList[i].y += dy;
////    }
////}

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
    char (*world)[x][z] = inputWorld;
    if (inputWorld == NULL) {
        world = malloc(sizeof(char[y][x][z]));
    }

    if (world == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    char colorValue[z];
    formatColor(BG, colorValue);

    setEntire(world, colorValue);

    setPosColor(world, (x - 2) / 2, (y - 1) / 2, PlayerColor);

    struct Vector2 P1 = {70, 7};
    struct Vector2 P2 = {90, 10};
    struct Vector2 P3 = {80, 16};

    struct Color C1 = {255, 0, 0};
    struct Color C2 = {0, 255, 0};
    struct Color C3 = {0, 0, 255};
    
    drawTriangle(world, P1, P2, P3, C1, C2, C3);

    setRects(world);

    return world;
}

int main(){
    x++;

    camera = set(0, 0);
    viewport = set(x - 2, y - 1);

    struct Rect* second = createRect(27, 3, 35, 8, wallColor);
    struct Rect* first = createRect(-3, -3, 3, 3, wallColor);
    //struct Rect* second = createRect(12, 12, 18, 18, wallColor, 3);

    char (*world)[x][z] = NULL;
    
    while (running){
        world = generateWorld(world);

        //oldPrint(world);
        fastPrint(world);

        getUserInput();

        Sleep(25);
    }

    free(world);
    freeRects();

    system("cls");
    return 0;
}

