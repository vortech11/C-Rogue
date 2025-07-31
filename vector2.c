#include <stdio.h>
#include "vector2.h"

void printVector2(Vector2 vector) {
    printf("%f %f\n", vector.x, vector.y);
}

Vector2 add(Vector2 vector1, Vector2 vector2) {
    vector1.x += vector2.x;
    vector1.y += vector2.y;
    return vector1;
}

Vector2 sub(Vector2 vector1, Vector2 vector2) {
    vector1.x -= vector2.x;
    vector1.y -= vector2.y;
    return vector1;
}

Vector2 scale(Vector2 vector1, float value) {
    vector1.x *= value;
    vector1.y *= value;
    return vector1;
}

Vector2 set(float value1, float value2) {
    Vector2 vector = {value1, value2};
    return vector;
}

Vector2 apply2x2Matrix(Vector2 vector0, Vector2 vector1, Vector2 vector2) {
    return set((vector1.x * vector0.x) + (vector2.x * vector0.y),
               (vector1.y * vector0.x) + (vector2.y * vector0.y));
}