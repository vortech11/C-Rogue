#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct {
    float x;
    float y;
} Vector2;

void printVector2(Vector2 vector);
Vector2 add(Vector2 vector1, Vector2 vector2);
Vector2 sub(Vector2 vector1, Vector2 vector2);
Vector2 scale(Vector2 vector1, float value);
Vector2 set(float value1, float value2);
Vector2 apply2x2Matrix(Vector2 vector0, Vector2 vector1, Vector2 vector2);

#endif